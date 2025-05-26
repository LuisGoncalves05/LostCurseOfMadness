#ifndef PLAYER_H
#define PLAYER_H

#include <lcom/lcf.h>
#include <math.h>
#include <stdlib.h>

#include "drivers/video/gpu.h"
#include "model/utils/keys.h"
#include "model/utils/sprite.h"

#define PLAYER_HEALTH 3        /**< Health of the player */
#define PLAYER_DEFAULT_SPEED 3 /**< Default speed of the player */

/**
 * @brief Represents a single Player instance.
 *
 * The Player struct is opaque.
 */
typedef struct Player Player;

/**
 * @brief Possible states of the player for animations and logic.
 */
typedef enum {
    PLAYER_IDLE,     /**< Player is idle */
    PLAYER_WALKING,  /**< Player is walking */
    PLAYER_AIMING,   /**< Player is aiming */
    PLAYER_SHOOTING, /**< Player is shooting */
    PLAYER_DYING     /**< Player is dying */
} PlayerState;

/**
 * @brief Creates a new player instance.
 *
 * @return Pointer to the created Player structure.
 */
Player *create_player();

/**
 * @brief Frees the memory associated with the player.
 *
 * @param player Pointer to the Player structure to destroy.
 */
void destroy_player(Player *player);

/**
 * @brief Returns the Sprite associated with the player.
 *
 * @param player Pointer to the Player structure.
 * @return Pointer to the Sprite structure.
 */
Sprite *player_get_sprite(Player *player);

/**
 * @brief Sets a new Sprite for the player.
 *
 * @param player Pointer to the Player structure.
 * @param sprite Pointer to the new Sprite.
 */
void player_set_sprite(Player *player, Sprite *sprite);

/**
 * @brief Returns the current health of the player.
 *
 * @param player Pointer to the Player structure.
 * @return The player's health.
 */
unsigned char player_get_health(Player *player);

/**
 * @brief Sets the current health of the player.
 *
 * @param player Pointer to the Player structure.
 * @param health New health value.
 */
void player_set_health(Player *player, unsigned char health);

/**
 * @brief Returns the current state of the player.
 *
 * @param player Pointer to the Player structure.
 * @return Current player state.
 */
PlayerState player_get_state(Player *player);

/**
 * @brief Updates the player's state based on the given mouse packet.
 *
 * @param player Pointer to the Player structure.
 * @param pp Mouse packet used to determine action.
 */
void player_update_state(Player *player, struct packet pp);

/**
 * @brief Updates the player's speed based on keyboard input.
 *
 * @param player Pointer to the Player structure.
 * @param scan_code Keyboard scan code.
 */
void player_update_speed(Player *player, uint8_t scan_code);

/**
 * @brief Renders the player on the screen.
 *
 * @param player Pointer to the Player structure.
 * @param delta Direction angle in radians.
 * @param frame_buffer Pointer to the frame buffer.
 */
void draw_player(Player *player, double delta, uint8_t *frame_buffer);

#endif /* PLAYER_H */
