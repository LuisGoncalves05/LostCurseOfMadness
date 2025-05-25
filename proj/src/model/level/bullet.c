#include "bullet.h"
#include "drivers/video/gpu.h" // for draw_sprite_rotated()
#include "lcom/lcf.h"          // for xpm_map_t
#include <math.h>
#include <stdlib.h>

Bullet *bullets[MAX_BULLETS];
int bullet_count = 0;
extern uint16_t x_res, y_res;

Bullet *create_bullet(int x, int y, double angle) {
  if (bullet_count >= MAX_BULLETS)
    return NULL;
  Bullet *b = malloc(sizeof(Bullet));
  if (!b)
    return NULL;

  b->sprite = create_sprite((xpm_map_t) cross, x, y, BULLET_SPEED, BULLET_SPEED);
  b->dx = (int) (cos(angle) * BULLET_SPEED);
  b->dy = (int) (sin(angle) * BULLET_SPEED);
  b->active = true;

  bullets[bullet_count++] = b;
  return b;
}

void update_bullet(Bullet *b, Maze *maze) {
  if (!b->active)
    return;
  b->sprite->x += b->dx;
  b->sprite->y += b->dy;

  if (b->sprite->x < 0 || b->sprite->x > x_res ||
      b->sprite->y < 0 || b->sprite->y > y_res) {
    b->active = false;
  }

  if (check_rectangle_line_collision(maze, b->sprite->x, b->sprite->y, b->sprite->width, b->sprite->height)) {
    b->active = false; // Deactivate if it hits a wall
  }
}

// Advances all bullets: moves them, deactivates off-screen, and cleans up
void update_all_bullets(Maze *maze) {
  for (int i = 0; i < bullet_count;) {
    update_bullet(bullets[i], maze);
    if (!bullets[i]->active) {
      destroy_bullet(bullets[i]);
      bullets[i] = bullets[--bullet_count];
    }
    else {
      i++;
    }
  }
}

// Draws every active bullet, rotated by theta
void draw_all_bullets(uint8_t *frame_buffer, double theta) {
  for (int i = 0; i < bullet_count; i++) {
    Bullet *b = bullets[i];
    if (b->active) {
      draw_sprite_rotated(b->sprite, theta, frame_buffer);
    }
  }
}

void destroy_bullet(Bullet *b) {
  if (!b)
    return;
  destroy_sprite(b->sprite);
  free(b);
}

void reset_bullets(void) {
  for (int i = 0; i < bullet_count; i++)
    destroy_bullet(bullets[i]);
  bullet_count = 0;
}
