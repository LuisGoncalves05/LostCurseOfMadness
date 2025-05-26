#include "level.h"

struct Level {
    uint8_t number;
    Maze *maze;
    Player *player;
    double delta; // atan2 of the player - mouse_position
    double fov_angle;
    Mob **mobs;
    Bullet *bullets[MAX_BULLETS];
    int bullet_count;
};

Level *create_level(uint8_t number) {
    Level *level = (Level *) malloc(sizeof(Level));
    if (!level)
        return NULL;

    level->number = number;
    uint8_t mob_count = 5 * (number + 1);
    level->maze = create_maze(10 + 2 * (number + 1) + 1, 10 + 2 * (number + 1) + 1, mob_count);
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
    level->fov_angle = 60.0;
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
        Sprite *mob_sprite = create_sprite((xpm_map_t) cross, position->x * CELL_SIZE, position->y * CELL_SIZE, 0, 0);
        level->mobs[i] = create_mob(mob_sprite);
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
        free_maze(level->maze);
        for (int i = 0; i < get_mob_count(get_maze(level)); i++) {
            destroy_mob(level->mobs[i]);
        }
        free(level);
    }
}

Player *get_player(Level *level) {
    if (!level)
        return NULL;
    return level->player;
}

Maze *get_maze(Level *level) {
    if (!level)
        return NULL;
    return level->maze;
}

double get_delta(Level *level) {
    if (!level)
        return 0;
    return level->delta;
}

Mob **get_mobs(Level *level) {
    return level->mobs;
}

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

void update_delta(Level *level, double mouse_x, double mouse_y) {
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
    printf("player speed: %d, %d\n", player_get_sprite(level->player)->xspeed, player_get_sprite(level->player)->yspeed);
}

void level_shoot(Level *level) {
    if (level == NULL || level->player == NULL)
        return;
    if (level->bullet_count >= MAX_BULLETS)
        return;

    Sprite *sprite = player_get_sprite(level->player);
    double delta = level->delta;

    // Compute bounding box of rotated sprite
    double rotated_width, rotated_height;
    get_rotated_bounds(sprite->width, sprite->height, delta, &rotated_width, &rotated_height);
    int rot_w = (int) rotated_width;
    int rot_h = (int) rotated_height;

    // Compute top-left corner of where to draw the rotated image so it's centered
    int draw_origin_x = sprite->x + sprite->width / 2 - rot_w / 2;
    int draw_origin_y = sprite->y + sprite->height / 2 - rot_h / 2;

    level->bullets[level->bullet_count++] = create_bullet(draw_origin_x, draw_origin_y, delta);
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

    if (check_rectangle_line_collision(get_maze(level), sprite->x, sprite->y, sprite->width, sprite->height)) {
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

static int player_update_position(Level *level) {
    if (!level)
        return 1;

    Sprite *player_sprite = player_get_sprite(level->player);
    Maze *maze = level->maze;
    uint16_t new_x = player_sprite->x + player_sprite->xspeed;
    uint16_t new_y = player_sprite->y + player_sprite->yspeed;
    if (!check_rectangle_line_collision(maze, new_x, new_y, player_sprite->width, player_sprite->height)) {
        if (!check_mob_collisions(level)) {
            printf("moving!\n");
            player_sprite->x = new_x;
            player_sprite->y = new_y;
        } else {
            player_set_health(level->player, player_get_health(level->player) - 1);
        }
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
    Mob **mobs = get_mobs(level);
    Maze *maze = get_maze(level);
    for (int i = 0; i < get_mob_count(maze);) {
        Mob *mob = mobs[i];
        update_mob_state(mob);
        if (mob_get_health(mob) == 0) {
            destroy_mob(mob);
            set_mob_count(maze, get_mob_count(maze) - 1);
            mobs[i] = mobs[get_mob_count(maze)];
        } else {
            i++;
        }
    }
}

static int draw_mobs(Level *level) {
    extern int frame_counter;
    Mob **mobs = get_mobs(level);
    uint8_t mob_count = get_mob_count(level->maze);
    for (int i = 0; i < mob_count; i++) {
        Mob *mob = mobs[i];
        Sprite *mob_sprite = mob_get_sprite(mob);
        Sprite *new_sprite;
        if (frame_counter > 16) {
            frame_counter = 0;
        }
        switch (mob_get_state(mob)) {
            case MOB_IDLE:
                if (frame_counter <= 4) {
                    new_sprite = create_sprite((xpm_map_t) pic1, mob_sprite->x, mob_sprite->y, mob_sprite->xspeed, mob_sprite->yspeed);
                } else if (frame_counter <= 8) {
                    new_sprite = create_sprite((xpm_map_t) pic2, mob_sprite->x, mob_sprite->y, mob_sprite->xspeed, mob_sprite->yspeed);
                } else {
                    new_sprite = create_sprite((xpm_map_t) cross, mob_sprite->x, mob_sprite->y, mob_sprite->xspeed, mob_sprite->yspeed);
                }

                break;
            case MOB_WALKING:
                if (frame_counter <= 4) {
                    new_sprite = create_sprite((xpm_map_t) penguin, mob_sprite->x, mob_sprite->y, mob_sprite->xspeed, mob_sprite->yspeed);
                } else if (frame_counter <= 8) {
                    new_sprite = create_sprite((xpm_map_t) pic1, mob_sprite->x, mob_sprite->y, mob_sprite->xspeed, mob_sprite->yspeed);
                } else if (frame_counter <= 12) {
                    new_sprite = create_sprite((xpm_map_t) pic1, mob_sprite->x, mob_sprite->y, mob_sprite->xspeed, mob_sprite->yspeed);
                } else {
                    new_sprite = create_sprite((xpm_map_t) pic1, mob_sprite->x, mob_sprite->y, mob_sprite->xspeed, mob_sprite->yspeed);
                }
                break;
            case MOB_ATTACKING:
                new_sprite = create_sprite((xpm_map_t) pic1, mob_sprite->x, mob_sprite->y, mob_sprite->xspeed, mob_sprite->yspeed);
                break;
            case MOB_DYING:
                new_sprite = create_sprite((xpm_map_t) pic1, mob_sprite->x, mob_sprite->y, mob_sprite->xspeed, mob_sprite->yspeed);
                break;
        }
        mob_set_sprite(mob, new_sprite);
        draw_sprite_rotated(mob_sprite, 0, maze_buffer);
    }
    return 0;
}

static void draw_fov_cone(Level *level) {
    double delta = level->delta;
    Sprite *player_sprite = player_get_sprite(level->player);
    Maze *maze = level->maze;
    if (!player_sprite || !maze)
        return;

    double x = player_sprite->x + player_sprite->width / 2.0;
    double y = player_sprite->y + player_sprite->height / 2.0;

    double fov_radius = FOV_RADIUS;
    double cone_half_angle = level->fov_angle * M_PI / 360.0; // Metade do ângulo do cone em radianos

    // Precompute direção do vetor unitário
    double dir_x = cos(delta);
    double dir_y = sin(delta);

    // Precompute cosseno para o produto escalar
    double cos_half_angle = cos(cone_half_angle);
    double cos_half_angle_sq = cos_half_angle * cos_half_angle;

    // Apenas verifica pixels num quadrado ao redor do jogador (otimização massiva)
    int x_start = (int) (x - fov_radius);
    int x_end = (int) (x + fov_radius);
    int y_start = (int) (y - fov_radius);
    int y_end = (int) (y + fov_radius);

    // Limita valores às bordas da tela
    x_start = (x_start < 0) ? 0 : x_start;
    x_end = (x_end >= x_res) ? x_res - 1 : x_end;
    y_start = (y_start < 0) ? 0 : y_start;
    y_end = (y_end >= y_res) ? y_res - 1 : y_end;

    for (int y_pixel = y_start; y_pixel <= y_end; y_pixel++) {
        for (int x_pixel = x_start; x_pixel <= x_end; x_pixel++) {
            if (x_pixel == (int) x && y_pixel == (int) y)
                continue;

            double dx = x_pixel - x;
            double dy = y_pixel - y;

            // Apenas multiplicações em vez de sqrt para melhor desempenho
            double dist_sq = dx * dx + dy * dy;
            if (dist_sq > fov_radius * fov_radius)
                continue;

            double dot_product = dx * dir_x + dy * dir_y;
            if (dot_product < 0)
                continue; // Atrás do jogador

            // Verifica se está dentro do cone usando comparação de quadrados
            if ((dot_product * dot_product) >= (dist_sq * cos_half_angle_sq)) {
                uint32_t index = (x_res * y_pixel + x_pixel) * bytes_per_pixel;

                if (index < frame_size) {
                    memcpy(&sec_frame_buffer[index], &maze_buffer[index], bytes_per_pixel);
                }
            }
        }
    }
}

static void draw_all_bullets(Level *level, uint8_t *frame_buffer) {
    for (int i = 0; i < level->bullet_count; i++) {
        Bullet *bullet = level->bullets[i];
        if (bullet_get_active(bullet)) {
            draw_bullet(bullet, frame_buffer);
        }
    }
}

void draw_level(Level *level, struct packet pp) {
    if (!level)
        return;

    clear(maze_buffer);
    draw_maze(level->maze, maze_buffer);

    level_update_all_mobs(level);
    draw_mobs(level);

    player_update_position(level);
    update_player_state(level->player, pp);
    draw_fov_cone(level);
    draw_player(level->player, level->delta, sec_frame_buffer);

    if (level->bullet_count > 0) {
        level_update_all_bullets(level);
        draw_all_bullets(level, sec_frame_buffer);
    }
}
