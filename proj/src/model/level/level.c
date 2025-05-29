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
    int bullet_count;
};

/* Create and destroy section */

Level *create_level(uint8_t number) {
    Level *level = (Level *) malloc(sizeof(Level));
    if (!level)
        return NULL;

    level->number = number;
    uint8_t mob_count = MOB_MULTIPLIER * (number + 1);
    level->maze = create_maze(2 + 4 * (number + 1), 2 + 3 * (number + 1), mob_count);
    if (!level->maze) {
        free(level);
        return NULL;
    }

    level->player = create_player();
    if (!level->player) {
        free(level->maze);
        free(level);
        return NULL;
    }

    level->delta = 0;
    level->fov_angle = FOV_ANGLE;
    level->mobs = malloc(sizeof(Mob *) * mob_count);
    if (!level->mobs) {
        free(level->maze);
        free(level->player);
        free(level);
        return NULL;
    }

    Point **mob_positions = get_mob_positions(level->maze);
    for (int i = 0; i < mob_count; i++) {
        Point *position = mob_positions[i];
        level->mobs[i] = create_mob(position->x * CELL_SIZE, position->y * CELL_SIZE);
        if (!level->mobs[i]) {
            for (int j = i - 1; j >= 0; j--) {
                destroy_mob(level->mobs[j]);
                free(level->maze);
                free(level->player);
                free(level);
                return NULL;
            }
        }
    }

    for (int i = 0; i < MAX_BULLETS; i++)
        level->bullets[i] = NULL;
    level->bullet_count = 0;

    return level;
}

void destroy_level(Level *level) {
    if (level != NULL) {
        destroy_player(level->player);
        destroy_maze(level->maze);
        for (int i = 0; i < get_mob_count(get_maze(level)); i++) {
            destroy_mob(level->mobs[i]);
        }
        free(level);
    }
}

/* Getter and setter section */

Maze *get_maze(Level *level) {
    if (!level)
        return NULL;
    return level->maze;
}

Player *get_player(Level *level) {
    if (!level)
        return NULL;
    return level->player;
}

double get_delta(Level *level) {
    if (!level)
        return 0;
    return level->delta;
}

Mob **get_mobs(Level *level) {
    return level->mobs;
}

/* Statics section */

static bool(check_mob_collisions)(Level *level) {
    uint8_t mob_count = get_mob_count(get_maze(level));
    Mob **mobs = get_mobs(level);
    for (int i = 0; i < mob_count; i++) {
        Sprite *player = player_get_sprite(get_player(level));
        if (check_sprite_collision(mob_get_sprite(mobs[i]), player))
            return true;
    }

    return false;
}

static void update_bullet(Bullet *b, Level *level) {
    if (!bullet_get_active(b))
        return;

    Sprite *sprite = bullet_get_sprite(b);

    sprite->x += bullet_get_dx(b);
    sprite->y += bullet_get_dy(b);

    if (sprite->x < 0 || sprite->x > x_res ||
        sprite->y < 0 || sprite->y > y_res) {
        bullet_set_active(b, false);
    }

    if (check_wall_collision(get_maze(level), sprite)) {
        bullet_set_active(b, false); // Deactivate if it hits a wall
    }

    Mob **mobs = get_mobs(level);
    int mob_count = get_mob_count(get_maze(level));
    for (int i = 0; i < mob_count; i++) {
        Mob *mob = mobs[i];
        if (check_sprite_collision(sprite, mob_get_sprite(mob))) {
            mob_set_health(mob, mob_get_health(mob) - 1);
            bullet_set_active(b, false);
            break;
        }
    }
}

static bool check_win(Sprite *sprite, Maze *maze) {
    return check_sprite_collision(sprite, get_key_sprite(maze));
}

static int player_update_position(Level *level) {
    if (!level)
        return 1;

    Player *player = level->player;
    Sprite *player_sprite = player_get_sprite(player);
    Maze *maze = level->maze;

    player_sprite->x += player_sprite->xspeed;
    if (check_wall_collision(maze, player_sprite)) {
        player_sprite->x -= player_sprite->xspeed;
    }

    player_sprite->y += player_sprite->yspeed;
    if (check_wall_collision(maze, player_sprite)) {
        player_sprite->y -= player_sprite->yspeed;
    }

    if (check_mob_collisions(level)) {
        player_lose_health(player);
    }

    if (player_get_state(player) != PLAYER_DYING && check_win(player_sprite, maze)) {
        player_set_state(player, PLAYER_WIN);
    }

    return 0;
}

static void level_update_all_bullets(Level *level) {
    for (int i = 0; i < level->bullet_count;) {
        update_bullet(level->bullets[i], level);
        if (!bullet_get_active(level->bullets[i])) {
            destroy_bullet(level->bullets[i]);
            level->bullet_count--;
            level->bullets[i] = level->bullets[level->bullet_count];
        } else {
            i++;
        }
    }
}

static void level_update_all_mobs(Level *level) {
    Mob **mobs = level->mobs;
    Maze *maze = level->maze;
    Player *player = level->player;
    for (int i = 0; i < get_mob_count(maze);) {
        Mob *mob = mobs[i];
        mob_update_state(mob, player_get_x(player) + player_get_width(player) / 2, player_get_y(player) + player_get_heigth(player) / 2);
        if (mob_get_health(mob) == 0) {
            destroy_mob(mob);
            set_mob_count(maze, get_mob_count(maze) - 1);
            mobs[i] = mobs[get_mob_count(maze)];
        } else {
            i++;
        }

        if (mob_get_state(mob) == MOB_ATTACKING) {
            Sprite *mob_sprite = mob_get_sprite(mob);
            mob_sprite->x += mob_sprite->xspeed;
            if (check_wall_collision(maze, mob_sprite)) {
                mob_sprite->x -= mob_sprite->xspeed;
            }
            mob_sprite->y += mob_sprite->yspeed;
            if (check_wall_collision(maze, mob_sprite)) {
                mob_sprite->y -= mob_sprite->yspeed;
            }
        }
    }
}

static int draw_mobs(Level *level) {
    extern int frame_counter;
    Mob **mobs = get_mobs(level);
    uint8_t mob_count = get_mob_count(level->maze);
    for (int i = 0; i < mob_count; i++) {
        Mob *mob = mobs[i];
        draw_mob(mob, secondary_frame_buffer);
    }
    return 0;
}

static void draw_fov_cone(Level *level) {
    double delta = level->delta;
    Sprite *player_sprite = player_get_sprite(level->player);
    Maze *maze = level->maze;
    if (!player_sprite || !maze)
        return;

    // Player center coordinates
    double cx = player_sprite->x + player_sprite->width / 2.0;
    double cy = player_sprite->y + player_sprite->height / 2.0;

    double fov_radius = FOV_RADIUS;
    double cone_half_angle = level->fov_angle * M_PI / 360.0;

    // Angles that bound the circular sector
    double angle1 = delta - cone_half_angle;
    double angle2 = delta + cone_half_angle;

    // Normalize to [0, 2pi)
    while (angle1 < 0)
        angle1 += 2 * M_PI;
    while (angle2 < 0)
        angle2 += 2 * M_PI;
    while (angle1 >= 2 * M_PI)
        angle1 -= 2 * M_PI;
    while (angle2 >= 2 * M_PI)
        angle2 -= 2 * M_PI;

    // Ensure angle1 <= angle2
    if (angle2 < angle1)
        angle2 += 2 * M_PI;

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

    // Directions to consider: 0, π/2, π, 3π/2
    double angles[] = {0, M_PI_2, M_PI, 3 * M_PI_2};
    for (int i = 0; i < 4; ++i) {
        double a = angles[i];
        double a_check = a;
        if (a_check < angle1)
            a_check += 2 * M_PI;
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
    int box_min_x = (int) fmax(0, floor(min_x));
    int box_max_x = (int) fmin(x_res - 1, ceil(max_x));
    int box_min_y = (int) fmax(0, floor(min_y));
    int box_max_y = (int) fmin(y_res - 1, ceil(max_y));

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
    for (int i = 0; i < level->bullet_count; i++) {
        Bullet *bullet = level->bullets[i];
        if (bullet_get_active(bullet)) {
            draw_bullet(bullet, frame_buffer);
        }
    }
}

/* Others section */

void level_update_delta(Level *level, double mouse_x, double mouse_y) {
    Sprite *player_sprite = player_get_sprite(level->player);
    double player_center_x = player_sprite->x + player_sprite->width / 2.0;
    double player_center_y = player_sprite->y + player_sprite->height / 2.0;

    double dx = mouse_x - player_center_x;
    double dy = mouse_y - player_center_y;

    level->delta = atan2(dy, dx);
}

void level_update_position(Level *level, uint8_t scan_code) {
    if (!level)
        return;
    player_update_speed(level->player, scan_code);
}

void level_shoot(Level *level) {
    if (level == NULL || level->player == NULL)
        return;
    if (level->bullet_count >= MAX_BULLETS)
        return;

    Sprite *sprite = player_get_sprite(level->player);
    Direction direction = player_get_direction(level->player);
    double draw_origin_y = sprite->y + sprite->height / 2;
    double draw_origin_x;
    if (direction == RIGHT || direction == UP) {
        draw_origin_x = sprite->x + sprite->width;
    } else {
        draw_origin_x = sprite->x;
    }

    double bullet_randomizer = ((rand() % BULLET_DEVIANCE) - BULLET_DEVIANCE) * 2 * M_PI / 360.0;
    level->bullets[level->bullet_count++] = create_bullet(draw_origin_x, draw_origin_y, level->delta + bullet_randomizer);
}

/* Draw section */

void draw_level(Level *level, struct packet pp) {
    if (!level)
        return;

    // Maze logic
    clear_frame_buffer(secondary_frame_buffer, IN_FOV_COLOR);
    draw_maze(level->maze, secondary_frame_buffer);
    draw_maze_outer(level->maze, secondary_frame_buffer);

    // Mob logic
    level_update_all_mobs(level);
    draw_mobs(level);

    // Player logic
    player_update_position(level);
    player_update_state(level->player, pp);
    draw_fov_cone(level);
    draw_player(level->player, level->delta, secondary_frame_buffer);

    // Level logic
    level_update_all_bullets(level);
    draw_all_bullets(level, secondary_frame_buffer);
}
