#include "level.h"

struct Level {
  uint8_t number;
  Maze *maze;
  Player *player;
  double delta; // atan2 of the player - mouse_position
  double fov_angle;
};

Level *create_level(uint8_t number) {
  Level *level = (Level *) malloc(sizeof(Level));
  if (!level)
    return NULL;

  level->number = number;
  level->maze = create_maze(31, 11);
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

  return level;
}

void destroy_level(Level *level) {
  if (level != NULL) {
    destroy_player(level->player);
    free_maze(level->maze);
    free(level);
  }
}

Player *get_player(Level *level) {
  if (!level) return NULL;
  return level->player;
}

Maze *get_maze(Level *level) {
  if (!level) return NULL;
  return level->maze;
}

double get_delta(Level* level) {
  if (!level) return 0;
  return level->delta;
}

void update_delta(Level *level, double mouse_x, double mouse_y) {
  Sprite *player_sprite = get_sprite(level->player);
  double player_center_x = player_sprite->x + player_sprite->width / 2.0;
  double player_center_y = player_sprite->y + player_sprite->height / 2.0;

  double dx = mouse_x - player_center_x;
  double dy = mouse_y - player_center_y;

  level->delta = atan2(dy, dx);
}

void draw_fov_cone(Level *level) {
  double delta = level->delta;
  Sprite *player_sprite = get_sprite(level->player);
  Maze *maze = level->maze;
  if (!player_sprite || !maze) return;

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
      if (x_pixel == (int) x && y_pixel == (int) y) continue;

      double dx = x_pixel - x;
      double dy = y_pixel - y;

      // Apenas multiplicações em vez de sqrt para melhor desempenho
      double dist_sq = dx * dx + dy * dy;
      if (dist_sq > fov_radius * fov_radius) continue;

      double dot_product = dx * dir_x + dy * dir_y;
      if (dot_product < 0) continue; // Atrás do jogador

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
