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

#define HEALTH 3
#define PLAYER_MAX_SPEED 7 // Maximum speed of the player
#define PLAYER_ACCELERATION 2 // Acceleration of the player
#define PLAYER_HEALTH 3 // Health of the player
#define PLAYER_DEFAULT_SPEED 5 // Default speed of the player

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
 * @brief returns the maximum speed of the player.
 */
int player_get_max_speed(Player *player);
/**
 * @brief returns the acceleration of the player.
 */
int player_get_acceleration(Player *player);
/**
 * @brief returns true if the player moved, false otherwise. If true, provides the new position.
 */
bool player_get_moved(Player *player, int *new_x, int *new_y);
/**
 * @brief sets the flag if the player moved or not.
 */
void player_set_moved(Player *player, bool moved);

double get_direction(Player *player);

/**
 * @brief function to prevent the player from moving faster than the maximum speed.
 */
void player_limit_speed(Player *player);
/**
 * @brief updates the player's next position 
 */
void update_player_position(Player *player, double delta, uint8_t scan_code);
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
