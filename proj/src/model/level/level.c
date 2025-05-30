#include "level.h"
#include "config.h"

struct Level {
    uint8_t number;
    Maze *maze;
    Player *player;
    double delta;
    double fov_angle;
    Mob **mobs;
    Bullet *bullets[MAX_BULLETS];
    uint8_t bullet_count;
};

/* Create and destroy section */

Level *create_level(uint8_t number) {
    Level *level = malloc(sizeof(Level));
    if (level == NULL) {
        printf("create_level: NULL pointer providede\n");
        return NULL;
    }

    level->number = number;
    uint8_t mob_count = MOB_MULTIPLIER * number;
    level->maze = create_maze(4 * (number + 2), 3 * (number + 2), mob_count);
    if (level->maze == NULL) {
        printf("create_level: create_maze failed\n");
        free(level);
        return NULL;
    }

    level->player = create_player();
    if (level->player == NULL) {
        printf("create_level: create_player failed\n");
        free(level->maze);
        free(level);
        return NULL;
    }

    level->delta = 0;
    level->fov_angle = FOV_ANGLE;
    level->mobs = malloc(sizeof(Mob *) * mob_count);
    if (level->mobs == NULL) {
        printf("create_level: malloc failed\n");
        free(level->maze);
        free(level->player);
        free(level);
        return NULL;
    }

    Point **mob_positions = get_mob_positions(level->maze);
    if (mob_positions == NULL) {
        printf("create_level: get_mob_positions failed\n");
        free(level->maze);
        free(level->player);
        free(level);
        return NULL;
    }

    for (int i = 0; i < mob_count; i++) {
        Point *position = mob_positions[i];
        level->mobs[i] = create_mob(position->x * CELL_SIZE, position->y * CELL_SIZE);
        if (level->mobs[i] == NULL) {
            printf("create_level: create_mob failed\n");
            for (int j = i - 1; j >= 0; j--) {
                destroy_mob(level->mobs[j]);
                free(level->maze);
                free(level->player);
                free(level);
                return NULL;
            }
        }
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        level->bullets[i] = NULL;
    }
    level->bullet_count = 0;

    return level;
}

void destroy_level(Level *level) {
    if (level == NULL) {
        printf("destroy_level: NULL pointer provided\n");
        return;
    }

    destroy_player(level->player);
    destroy_maze(level->maze);
    for (int i = 0; i < get_mob_count(get_maze(level)); i++) {
        destroy_mob(level->mobs[i]);
    }
    free(level->mobs);
    free(level);
}

/* Getter and setter section */

Maze *get_maze(Level *level) {
    if (level == NULL) {
        printf("get_maze: NULL pointer provided\n");
        return NULL;
    }

    return level->maze;
}

Player *get_player(Level *level) {
    if (level == NULL) {
        printf("get_player: NULL pointer provided\n");
        return NULL;
    }

    return level->player;
}

double get_delta(Level *level) {
    if (level == NULL) {
        printf("get_delta: NULL pointer provided\n");
        return 0;
    }

    return level->delta;
}

Mob **get_mobs(Level *level) {
    if (level == NULL) {
        printf("get_mobs: NULL pointer provided\n");
        return NULL;
    }

    return level->mobs;
}

PlayerState level_get_player_state(Level *level) {
    if (!level) {
        printf("level_get_player_state: NULL pointer provided\n");
        return PLAYER_DEAD;
    }
    return player_get_state(level->player);
}

/* Statics section */

static bool(check_mob_collisions)(Level *level) {
    if (level == NULL){
        printf("check_mob_collisions: NULL pointer provided\n");
        return false;
    }

    uint8_t mob_count = get_mob_count(get_maze(level));
    Mob **mobs = get_mobs(level);
    if (mobs == NULL) {
        printf("check_mob_collisions: get_mobs failed\n");
        return false;
    }

    Sprite *player = player_get_sprite(get_player(level));
    for (int i = 0; i < mob_count; i++) {
        if (player == NULL) {
            printf("check_mob_collisions: player_get_sprite failed\n");
            return false;
        }
        if (check_sprite_collision(mob_get_sprite(mobs[i]), player)) {
            return true;
        }
    }

    return false;
}

static void update_bullet(Bullet *b, Level *level) {
    if (b == NULL || level == NULL) {
        printf("update_bullet: NULL pointer provided\n");
        return;
    }

    if (!bullet_get_active(b)) {
        printf("update_bullet: inactive bullet\n");
        return;
    }

    Sprite *sprite = bullet_get_sprite(b);
    if (sprite == NULL) {
        printf("update_bullet: bullet_get_sprite failed\n");
        return;
    }

    sprite->x += (int) round(bullet_get_xspeed(b));
    sprite->y += (int) round(bullet_get_yspeed(b));

    if (sprite->x < 0 || sprite->x > x_res ||
        sprite->y < 0 || sprite->y > y_res) {
        bullet_set_active(b, false);
        return;
    }

    if (check_wall_collision(get_maze(level), sprite)) {
        bullet_set_active(b, false);
        return;
    }

    Mob **mobs = get_mobs(level);
    if (mobs == NULL) {
        printf("update_bullet: get_mobs failed\n");
        return;
    }
    
    int mob_count = get_mob_count(get_maze(level));
    for (int i = 0; i < mob_count; i++) {
        Mob *mob = mobs[i];
        Sprite *mob_sprite = mob_get_sprite(mob);
        if (check_sprite_collision(sprite, mob_sprite)) {
            mob_set_health(mob, mob_get_health(mob) - 1);
            bullet_set_active(b, false);
            break;
        }
    }
}

static bool check_win(Sprite *sprite, Maze *maze) {
    if (!sprite || maze == NULL) {
        printf("check_win: NULL pointer provided\n");
        return false;
    }

    return check_sprite_collision(sprite, get_key_sprite(maze));
}

static int player_update_position(Level *level) {
    if (level == NULL) {
        printf("player_update_position: NULL pointer provided\n");
        return 1;
    }

    Player *player = level->player;
    Maze *maze = level->maze;

    uint16_t x = player_get_x(player);
    uint16_t y = player_get_y(player);
    double xspeed = player_get_xspeed(player);
    double yspeed = player_get_yspeed(player);

    player_set_x(player, x + xspeed);
    if (check_wall_collision(maze, player_get_sprite(player))) {
        player_set_x(player, x);
    }

    player_set_y(player, y + yspeed);
    if (check_wall_collision(maze, player_get_sprite(player))) {
        player_set_y(player, y);
    }

    if (check_mob_collisions(level)) {
        player_lose_health(player);
    }

    if (player_get_state(player) != PLAYER_DEAD && check_win(player_get_sprite(player), maze)) {
        player_set_state(player, PLAYER_WIN);
    }

    return 0;
}

static void level_update_all_bullets(Level *level) {
    if (level == NULL) {
        printf("level_update_all_bullets: NULL pointer provided\n");
        return;
    }

    for (int i = 0; i < level->bullet_count;) {
        update_bullet(level->bullets[i], level);
        if (!bullet_get_active(level->bullets[i])) {
            destroy_bullet(level->bullets[i]);
            level->bullet_count--;
            level->bullets[i] = level->bullets[level->bullet_count];
            level->bullets[level->bullet_count] = NULL;
        } else {
            i++;
        }
    }
}

static void level_update_all_mobs(Level *level) {
    if (level == NULL) {
        printf("level_update_all_mobs: NULL pointer provided\n");
        return;
    }

    Mob **mobs = level->mobs;
    Maze *maze = level->maze;
    Player *player = level->player;
    for (int i = 0; i < get_mob_count(maze);) {
        Mob *mob = mobs[i];
        mob_update_state(mob, player_get_x(player) + player_get_width(player) / 2, player_get_y(player) + player_get_height(player) / 2);
        if (mob_get_state(mob) == MOB_DEAD) {
            destroy_mob(mob);
            set_mob_count(maze, get_mob_count(maze) - 1);
            mobs[i] = mobs[get_mob_count(maze)];
        } else {
            i++;
        }

        if (mob_get_state(mob) == MOB_ATTACKING) {
            Sprite *mob_sprite = mob_get_sprite(mob);
            int16_t x = mob_get_x(mob);
            int16_t y = mob_get_y(mob);
            double xspeed = mob_get_xspeed(mob);
            double yspeed = mob_get_yspeed(mob);
            
            mob_set_x(mob, x + xspeed);
            if (check_wall_collision(maze, mob_sprite)) {
                mob_set_x(mob, x);
            }
            mob_set_y(mob, y + yspeed);
            if (check_wall_collision(maze, mob_sprite)) {
                mob_set_y(mob, y);
            }
        }
    }
}

static int draw_mobs(Level *level) {
    if (level == NULL) {
        printf("draw_mobs: NULL pointer provided\n");
        return 1;
    }

    Mob **mobs = get_mobs(level);
    uint8_t mob_count = get_mob_count(level->maze);
    for (int i = 0; i < mob_count; i++) {
        Mob *mob = mobs[i];
        draw_mob(mob, secondary_frame_buffer);
    }

    return 0;
}

static void draw_fov_cone(Level *level) {
    if (level == NULL) {
        printf("draw_fov_cone: NULL pointer provided\n");
        return;
    }
    double delta = level->delta;
    Player *player = level->player;
    Maze *maze = level->maze;
    if (!player || !maze) {
        printf("draw_fov_cone: NULL player or maze\n");
        return;
    }

    // Player center coordinates
    double cx = player_get_x(player) + player_get_width(player) / 2.0;
    double cy = player_get_y(player) + player_get_height(player) / 2.0;

    double fov_radius = FOV_RADIUS;
    double cone_half_angle = level->fov_angle * M_PI / 360.0;

    // Angles that bound the circular sector
    double angle1 = delta - cone_half_angle;
    double angle2 = delta + cone_half_angle;

    // Normalize to [0, 2pi)
    while (angle1 < 0) {
        angle1 += 2 * M_PI;
    }
    while (angle2 < 0) {
        angle2 += 2 * M_PI;
    }
    while (angle1 >= 2 * M_PI) {
        angle1 -= 2 * M_PI;
    }
    while (angle2 >= 2 * M_PI) {
        angle2 -= 2 * M_PI;
    }

    // Ensure angle1 <= angle2
    if (angle2 < angle1) {
        angle2 += 2 * M_PI;
    }

    // Candidate points
    double min_x = cx, max_x = cx;
    double min_y = cy, max_y = cy;

    // Add endpoints of cone
    double x1 = cx + cos(angle1) * fov_radius;
    double y1 = cy + sin(angle1) * fov_radius;
    double x2 = cx + cos(angle2) * fov_radius;
    double y2 = cy + sin(angle2) * fov_radius;

    min_x = fmin(min_x, fmin(x1, x2));
    max_x = fmax(max_x, fmax(x1, x2));
    min_y = fmin(min_y, fmin(y1, y2));
    max_y = fmax(max_y, fmax(y1, y2));

    double angles[] = {0, M_PI_2, M_PI, 3 * M_PI_2};
    for (int i = 0; i < 4; i++) {
        double a = angles[i];
        double a_check = a;
        if (a_check < angle1) {
            a_check += 2 * M_PI;
        }
        if (a_check >= angle1 && a_check <= angle2) {
            double x = cx + cos(a) * fov_radius;
            double y = cy + sin(a) * fov_radius;
            min_x = fmin(min_x, x);
            max_x = fmax(max_x, x);
            min_y = fmin(min_y, y);
            max_y = fmax(max_y, y);
        }
    }

    // Expand bounding box to fully contain CLOSE_RADIUS circle
    min_x = fmin(min_x, cx - CLOSE_RADIUS);
    max_x = fmax(max_x, cx + CLOSE_RADIUS);
    min_y = fmin(min_y, cy - CLOSE_RADIUS);
    max_y = fmax(max_y, cy + CLOSE_RADIUS);

    // Check screen bounds
    int box_min_x = (int)(min_x);
    if (box_min_x < 0) box_min_x = 0;

    int box_max_x = (int)(max_x + 1);
    if (box_max_x >= x_res) box_max_x = x_res - 1;

    int box_min_y = (int)(min_y);
    if (box_min_y < 0) box_min_y = 0;

    int box_max_y = (int)(max_y + 1);
    if (box_max_y >= y_res) box_max_y = y_res - 1;


    // Unit vector direction
    double dir_x = cos(delta);
    double dir_y = sin(delta);

    double cos_half_angle = cos(cone_half_angle);
    double cos_half_angle_sq = cos_half_angle * cos_half_angle;

    for (int y_pixel = box_min_y; y_pixel < box_max_y; y_pixel++) {
        double dy = y_pixel - cy;
        double dy_sq = dy * dy;
        for (int x_pixel = box_min_x; x_pixel < box_max_x; x_pixel++) {
            double dx = x_pixel - cx;
            double dist_sq = dx * dx + dy_sq;
            double dot_product = dx * dir_x + dy * dir_y;

            if ((dist_sq > fov_radius * fov_radius ||                        // Outside the circle of radius fov_radius
                 dot_product < 0 ||                                          // Behind player
                 (dot_product * dot_product) < (dist_sq * cos_half_angle_sq) // Outside the cone
                 ) &&
                dist_sq > CLOSE_RADIUS * CLOSE_RADIUS) { // Outside the circle the fov cone would be in
                vga_draw_pixel(x_pixel, y_pixel, OUT_OF_FOV_COLOR, secondary_frame_buffer);
            }
        }
    }

    vga_draw_rectangle(0, 0, x_res, box_min_y, OUT_OF_FOV_COLOR, secondary_frame_buffer);
    vga_draw_rectangle(0, box_min_y, box_min_x, box_max_y, OUT_OF_FOV_COLOR, secondary_frame_buffer);
    vga_draw_rectangle(max_x, box_min_y, x_res, box_max_y, OUT_OF_FOV_COLOR, secondary_frame_buffer);
    vga_draw_rectangle(0, box_max_y, x_res, y_res, OUT_OF_FOV_COLOR, secondary_frame_buffer);
}

static void draw_all_bullets(Level *level, uint8_t *frame_buffer) {
    if (level == NULL || frame_buffer == NULL) {
        printf("draw_all_bullets: NULL pointer provided\n");
        return;
    }

    for (int i = 0; i < level->bullet_count; i++) {
        Bullet *bullet = level->bullets[i];
        if (bullet_get_active(bullet)) {
            draw_bullet(bullet, frame_buffer);
        }
    }
}

/* Others section */

void level_update_delta(Level *level, double mouse_x, double mouse_y) {
    if (level == NULL) {
        printf("level_update_delta: NULL pointer provided\n");
        return;
    }
    Player *player = level->player;
    double player_center_x = player_get_x(player) + player_get_width(player) / 2.0;
    double player_center_y = player_get_y(player) + player_get_height(player) / 2.0;

    double dx = mouse_x - player_center_x;
    double dy = mouse_y - player_center_y;

    level->delta = atan2(dy, dx);
}

void level_update_position(Level *level, uint8_t scan_code) {
    if (level == NULL)
        return;
    player_update_speed(level->player, scan_code);
}

void level_shoot(Level *level) {
    if (level == NULL || level->player == NULL)
        return;
    if (level->bullet_count >= MAX_BULLETS)
        return;

    Player *player = level->player;
    double draw_origin_x = player_get_x(player) + player_get_width(player) / 2.0;
    double draw_origin_y = player_get_y(player) + player_get_height(player) / 2.0;

    double bullet_randomizer = ((rand() % BULLET_DEVIANCE) - BULLET_DEVIANCE / 2) * 2 * M_PI / 360.0;
    Bullet *bullet = create_bullet(draw_origin_x, draw_origin_y, level->delta + bullet_randomizer);
    if (bullet == NULL) {
        printf("level_shoot: create_bullet failed\n");
        return;
    }
    level->bullets[level->bullet_count++] = bullet;
}

/* Draw section */

int draw_level(Level *level, struct packet pp) {
    if (level == NULL) {
        return 1;
    }

    // Maze logic
    clear_frame_buffer(secondary_frame_buffer, IN_FOV_COLOR);
    draw_maze(level->maze, secondary_frame_buffer);
    draw_maze_outer(level->maze, secondary_frame_buffer);

    // Mob logic
    level_update_all_mobs(level);
    if (draw_mobs(level)) {
        return 1;
    }

    // Player logic
    player_update_position(level);
    player_update_state(level->player, pp);
    draw_fov_cone(level);
    draw_player(level->player, level->delta, secondary_frame_buffer);

    // Level logic
    level_update_all_bullets(level);
    draw_all_bullets(level, secondary_frame_buffer);

    return 0;
}
