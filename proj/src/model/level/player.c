#include "player.h"

struct Player {
  PlayerState state;
  Sprite *sprite;
  unsigned char health;
  int max_speed;
  int acceleration;
  bool moved;
};

void playerIsAtMaxSpeed(Player *player) {
  if (player->sprite->xspeed >= player->max_speed) {
    player->sprite->xspeed = player->max_speed;
  }
  if (player->sprite->yspeed >= player->max_speed) {
    player->sprite->yspeed = player->max_speed;
  }
}

void playerStopped(Player *player) {
  if (player->moved == 0) {
    player->sprite->xspeed = PLAYER_DEFAULT_SPEED;
    player->sprite->yspeed = PLAYER_DEFAULT_SPEED;
  }
}

// Create a new player
Player *create_player() {
  Player *player = (Player *) malloc(sizeof(Player));
  if (!player) return NULL;
  player->state = PLAYER_IDLE; 
  player->health = PLAYER_HEALTH;
  player->sprite = create_sprite((xpm_map_t) cross, 400, 400, 3, 3);
  player->moved = 0;
  player->max_speed = PLAYER_MAX_SPEED;
  player->acceleration = PLAYER_ACCELERATION;
  return player;
}

// Destroy the player and free memory
void destroy_player(Player *player) {
  if (!player) return;
  destroy_sprite(player->sprite);
  free(player);
}

Sprite *get_sprite(Player *player) {
  return player->sprite;
}

void set_sprite(Player *player, Sprite *sprite) {
  if (player == NULL) return;
  if (player->sprite != NULL) {
    destroy_sprite(player->sprite);
  }
  player->sprite = sprite;
}

unsigned char get_health(Player *player) {
  return player->health;
}

int get_max_speed(Player *player) {
  return player->max_speed;
}

int get_acceleration(Player *player) {
  return player->acceleration;
}

bool get_moved(Player *player) {
  return player->moved;
}

void set_moved(Player *player, bool moved) {
  player->moved = moved;
}

void update_player_state(Player *player, struct packet pp) {
  if (player == NULL)
    return;

  if (player->moved == 1) {
    player->state = PLAYER_WALKING;
  }
  else {
    player->state = PLAYER_IDLE;
  }

  if (pp.lb == 0 && pp.rb == 1) {
    player->state = PLAYER_AIMING;
  }
  else if (pp.lb == 1 && pp.rb == 1) {
    player->state = PLAYER_SHOOTING;
  }
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
  set_sprite(player, new_sprite);
  draw_sprite_rotated(get_sprite(player), delta, frame_buffer);
}
