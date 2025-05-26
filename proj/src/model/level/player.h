/**
 * @file player.h
 *
 * @brief This file contains the declarations of the functions and "class" for the player module.
 * The player module is responsible for managing the player logic and handling changes in the internal state of the player.
 */

/** @defgroup player Player
 *  @brief player module for managing player logic and user input.
 *  @{
 */
#ifndef PLAYER_H
#define PLAYER_H

#include <lcom/lcf.h>
#include <stdlib.h>
#include <math.h>
#include "model/utils/sprite.h"
#include "model/utils/keys.h"
#include "drivers/video/gpu.h"

#include "assets/xpm/level/player/idle/idle1.xpm"
#include "assets/xpm/level/player/idle/idle2.xpm"

#define HEALTH 3
#define PLAYER_HEALTH 3 // Health of the player
#define PLAYER_DEFAULT_SPEED 3 // Default speed of the player

typedef struct Player Player;

typedef enum {
	PLAYER_IDLE,
	PLAYER_WALKING,
	PLAYER_AIMING,
	PLAYER_SHOOTING,
	PLAYER_DYING,
} PlayerState;

/**
 * @brief constructs a player "object".
 */
Player *create_player();
/**
 * @brief Destroys the player "object" and frees its memory.
 */
void destroy_player(Player *player);


PlayerState get_player_state(Player *player);

void set_player_state(Player *player, PlayerState state);

/**
 * @brief returns the sprite associated with the player.
 */
Sprite *player_get_sprite(Player *player);
/**
 * @brief returns the current health of the player.
 */
unsigned char player_get_health(Player *player);
/**
 * @brief returns the speed in x;
 */
int player_get_dx(Player *player);
/**
 * @brief returns the speed in y;
 */
int player_get_dy(Player *player);

/**
 * @brief updates the player's speed 
 */
void player_update_speed(Player *player, uint8_t scan_code);

/**
 * @brief sets the player's health.
 */
void player_set_health(Player *player, unsigned char health);
/**
 * @brief updates the player state based on the packet received.
 */
void update_player_state(Player *player, struct packet pp);
/**
 * @brief sets the player with a new sprite.
 */
void player_set_sprite(Player *player, Sprite *sprite);

/**
 * @brief draws the player
 */
void draw_player(Player* player, double delta, uint8_t *frame_buffer);

#endif

/** @} */
