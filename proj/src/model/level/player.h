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

// #include "game.h"
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
 * @brief function to prevent the player from moving faster than the maximum speed.
 */
void playerIsAtMaxSpeed(Player *player);
/**
 * @brief function to update the player velocity to default if he stops moving.
 */
void playerStopped(Player *player);
/**
 * @brief constructs a player "object".
 */
Player *create_player(Sprite *sprite);
/**
 * @brief Destroys the player "object" and frees its memory.
 */
void destroy_player(Player *player);
/**
 * @brief returns the sprite associated with the player.
 */
Sprite *get_sprite(Player *player);
/**
 * @brief returns the current health of the player.
 */
unsigned char get_health(Player *player);
/**
 * @brief returns the maximum speed of the player.
 */
int get_max_speed(Player *player);
/**
 * @brief returns the acceleration of the player.
 */
int get_acceleration(Player *player);
/**
 * @brief returns true if the player moved, false otherwise.
 */
bool get_moved(Player *player);
/**
 * @brief sets the flag if the player moved or not.
 */
void set_moved(Player *player, bool moved);
/**
 * @brief updates the player state based on the packet received.
 */
void update_player_state(Player *player, struct packet pp);
/**
 * @brief sets the player with a new sprite.
 */
void set_sprite(Player *player, Sprite *sprite);

#endif


/** @} */
