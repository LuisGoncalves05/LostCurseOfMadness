#include "player.h"

struct Player {
  PlayerState state;
  Sprite *sprite;
  unsigned char health;
  int max_speed;
  int acceleration;
  bool moved;
  double direction;
  int new_x;
  int new_y;
};

// Create a new player
Player *create_player() {
  Player *player = (Player *) malloc(sizeof(Player));
  if (!player) return NULL;
  player->state = PLAYER_IDLE; 
  player->health = PLAYER_HEALTH;
  player->sprite = create_sprite((xpm_map_t) cross, 400, 400, 3, 3);
  player->max_speed = PLAYER_MAX_SPEED;
  player->acceleration = PLAYER_ACCELERATION;
  player->moved = 0;
  player->direction = 0;
  player->new_x = 0;
  player->new_y = 0;
  return player;
}

// Destroy the player and free memory
void destroy_player(Player *player) {
  if (!player)
    return;
  destroy_sprite(player->sprite);
  free(player);
}

Sprite *player_get_sprite(Player *player) {
  return player->sprite;
}

void player_set_sprite(Player *player, Sprite *sprite) {
  if (player == NULL)
    return;
  if (player->sprite != NULL) {
    destroy_sprite(player->sprite);
  }
  player->sprite = sprite;
}

unsigned char player_get_health(Player *player) {
  return player->health;
}

int player_get_max_speed(Player *player) {
  return player->max_speed;
}

int player_get_acceleration(Player *player) {
  return player->acceleration;
}

PlayerState get_player_state(Player *player) {
  return player->state;
}

void set_player_state(Player *player, PlayerState state) {
  if (player == NULL) return;
  player->state = state;
}

bool player_get_moved(Player *player, int *new_x, int *new_y) {
  if (player->moved) {
    *new_x = player->new_x;
    *new_y = player->new_y;
  }
  player->new_x = 0;
  player->new_y = 0;
  return player->moved;
}

void player_set_moved(Player *player, bool moved) {
  player->moved = moved;
}

double get_direction(Player *player) {
  return player->direction;
}

void player_limit_speed(Player *player) {
  if (player->sprite->xspeed >= player->max_speed) {
    player->sprite->xspeed = player->max_speed;
  }
  if (player->sprite->yspeed >= player->max_speed) {
    player->sprite->yspeed = player->max_speed;
  }
}

void player_set_health(Player *player, unsigned char health) {
  player->health = health;
}

void update_player_state(Player *player, struct packet pp) {
  if (player == NULL) return;
  if (player->moved == 1) {
    player->state = PLAYER_WALKING;
  } else {
    player->state = PLAYER_IDLE;
  }

  if (pp.lb == 1) {
    player->state = PLAYER_SHOOTING;
  }
  
  if (player->health == 0) {
    player->state = PLAYER_DYING;
  }
}

void update_player_position(Player *player, double delta, uint8_t scan_code) {
  double x_changer = 0;
  double y_changer = 0;
  bool moved = 0;
  double direction = player->direction;
  switch (scan_code) {
    case KEY_W:
      x_changer = cos(delta);
      y_changer = sin(delta);
      direction = delta;
      moved = 1;
      break;
    case KEY_A:
      x_changer = sin(delta);
      y_changer = -cos(delta);
      direction = delta - M_PI / 2;
      moved = 1;
      break;
    case KEY_S:
      x_changer = -cos(delta);
      y_changer = -sin(delta);
      direction = delta + M_PI;
      moved = 1;
      break;
    case KEY_D:
      x_changer = -sin(delta);
      y_changer = cos(delta);
      direction = delta + M_PI / 2;
      moved = 1;
      break;
    default:
      break;
  }

  player->moved = moved;
  player->direction = direction;
  printf("player direction: %d\n", player->direction);
  player->new_x = player->sprite->x + x_changer * player->sprite->xspeed;
  player->new_y = player->sprite->y + y_changer * player->sprite->yspeed;
}

void draw_player(Player *player, double delta, uint8_t *frame_buffer) {
  Sprite *player_sprite = player->sprite;
  Sprite *new_sprite;

  extern int frame_counter;
  if(frame_counter > 16) frame_counter = 0;

  switch (player->state) {
    case PLAYER_IDLE:
      if (frame_counter <= 4){
        new_sprite = create_sprite((xpm_map_t) pic1, player_sprite->x, player_sprite->y, player_sprite->xspeed, player_sprite->yspeed);
      } else if (frame_counter <= 8){
        new_sprite = create_sprite((xpm_map_t) pic2, player_sprite->x, player_sprite->y, player_sprite->xspeed, player_sprite->yspeed);
      } else {
        new_sprite = create_sprite((xpm_map_t) cross, player_sprite->x, player_sprite->y, player_sprite->xspeed, player_sprite->yspeed);
      }
      break;
    case PLAYER_WALKING:
      if (frame_counter <= 4){
      new_sprite = create_sprite((xpm_map_t) penguin, player_sprite->x, player_sprite->y, player_sprite->xspeed, player_sprite->yspeed);
      } else if (frame_counter <= 8){
      new_sprite = create_sprite((xpm_map_t) pic1, player_sprite->x, player_sprite->y, player_sprite->xspeed, player_sprite->yspeed);
      } else if (frame_counter <= 12){
      new_sprite = create_sprite((xpm_map_t) pic1, player_sprite->x, player_sprite->y, player_sprite->xspeed, player_sprite->yspeed);
      } else {
      new_sprite = create_sprite((xpm_map_t) pic1, player_sprite->x, player_sprite->y, player_sprite->xspeed, player_sprite->yspeed);
      }
      break;  
    case PLAYER_AIMING:
      new_sprite = create_sprite((xpm_map_t) pic1, player_sprite->x, player_sprite->y, player_sprite->xspeed, player_sprite->yspeed);
      break;
    case PLAYER_SHOOTING:
      new_sprite = create_sprite((xpm_map_t) pic1, player_sprite->x, player_sprite->y, player_sprite->xspeed, player_sprite->yspeed);
      break;
    case PLAYER_DYING:
      new_sprite = create_sprite((xpm_map_t) pic1, player_sprite->x, player_sprite->y, player_sprite->xspeed, player_sprite->yspeed);
      break;
    default:
      break;
  }
  destroy_sprite(player_sprite);
  player_set_sprite(player, new_sprite);
  draw_sprite_rotated(player_get_sprite(player), delta, frame_buffer);
}
