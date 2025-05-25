#include "player.h"

enum player_state state = PLAYER_IDLE;

struct Player {
  unsigned char health;
  int max_speed;
  int acceleration;
  Sprite *sprite;
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
Player *create_player(Sprite *sprite) {
  Player *player = (Player *) malloc(sizeof(Player));
  if (!player)
    return NULL;
  player->health = PLAYER_HEALTH;
  player->sprite = sprite;
  player->moved = 0;
  player->max_speed = PLAYER_MAX_SPEED;       // Set default max speed
  player->acceleration = PLAYER_ACCELERATION; // Set default acceleration
  return player;
}

// Destroy the player and free memory
void destroy_player(Player *player) {
  if (!player) return;
  destroy_sprite(player->sprite);
  free(player);
}

Sprite *player_get_sprite(Player *player) {
  return player->sprite;
}

void player_set_sprite(Player *player, Sprite *sprite) {
  if (player == NULL) return;
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

bool player_get_moved(Player *player) {
  return player->moved;
}

void player_set_moved(Player *player, bool moved) {
  player->moved = moved;
}

void update_player_state(Player *player, struct packet pp) {
  if (player == NULL)
    return;

  if (player->moved == 1) {
    state = PLAYER_WALKING;
  }
  else {
    state = PLAYER_IDLE;
  }

  if (pp.lb == 0 && pp.rb == 1) {
    state = PLAYER_AIMING;
  }
  else if (pp.lb == 1 && pp.rb == 1) {
    state = PLAYER_SHOOTING;
  }
}
