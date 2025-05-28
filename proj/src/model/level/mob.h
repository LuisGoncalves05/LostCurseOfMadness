#ifndef MOB_H
#define MOB_H

#include <lcom/lcf.h>
#include <stdlib.h>

#include "drivers/video/gpu.h"
#include "model/utils/sprite.h"
#include "model/utils/animated_sprite.h"
#include "assets/xpm/level/mob/idle.xpm"

#define MOB_MAX_SPEED 5     /**< Maximum speed of the mob */
#define MOB_ACCELERATION 1  /**< Acceleration of the mob */
#define MOB_HEALTH 1        /**< Health of the mob */
#define MOB_DEFAULT_SPEED 2 /**< Default speed of the mob */

/**
 * @brief Represents a single Mob instance.
 *
 * The Mob struct is opaque.
 */
typedef struct Mob Mob;

/**
 * @brief Possible states of the mob for animations and logic.
 */
enum mob_state {
    MOB_IDLE,      /**< Mob is idle */
    MOB_WALKING,   /**< Mob is walking */
    MOB_ATTACKING, /**< Mob is attacking */
    MOB_DYING      /**< Mob is dying */
};
/**
 * @brief Creates a new mob instance with the specified sprite.
 *
 * @param sprite Pointer to the Sprite structure used for the mob.
 * @return Pointer to the created Mob structure.
 */
Mob *create_mob(uint16_t x, uint16_t y);

/**
 * @brief Frees the memory associated with the mob.
 *
 * @param mob Pointer to the Mob structure to destroy.
 */
void destroy_mob(Mob *mob);

/**
 * @brief Returns the Sprite associated with the given mob.
 *
 * @param mob Pointer to the Mob structure.
 * @return Pointer to the Sprite structure.
 */
Sprite *mob_get_sprite(Mob *mob);

/**
 * @brief Sets the mob's sprite to a new one.
 *
 * @param mob Pointer to the Mob structure.
 * @param sprite Pointer to the new Sprite.
 */
void mob_set_sprite(Mob *mob, Sprite *sprite);

/**
 * @brief Returns the current health of the mob.
 *
 * @param mob Pointer to the Mob structure.
 * @return The mob's health.
 */
unsigned char mob_get_health(Mob *mob);

/**
 * @brief Sets the current health of the mob.
 *
 * @param mob Pointer to the Mob structure.
 * @param health New health value.
 */
void mob_set_health(Mob *mob, unsigned char health);

/**
 * @brief Returns the maximum speed of the mob.
 *
 * @param mob Pointer to the Mob structure.
 * @return The mob's maximum speed.
 */
int mob_get_max_speed(Mob *mob);

/**
 * @brief Returns the acceleration of the mob.
 *
 * @param mob Pointer to the Mob structure.
 * @return The mob's acceleration.
 */
int mob_get_acceleration(Mob *mob);

/**
 * @brief Returns whether the mob has moved.
 *
 * @param mob Pointer to the Mob structure.
 * @return true if the mob moved, false otherwise.
 */
bool mob_get_moved(Mob *mob);

/**
 * @brief Sets whether the mob has moved.
 *
 * @param mob Pointer to the Mob structure.
 * @param moved Movement flag.
 */
void mob_set_moved(Mob *mob, bool moved);

/**
 * @brief Returns the current state of the mob.
 *
 * @param mob Pointer to the Mob structure.
 * @return Current mob state.
 */
enum mob_state mob_get_state(Mob *mob);

/**
 * @brief Updates the state of the mob based on its context.
 *
 * @param mob Pointer to the Mob structure.
 */
void update_mob_state(Mob *mob);

/**
 * @brief Caps the mob's velocity to its maximum speed.
 *
 * @param mob Pointer to the Mob structure.
 */
void mobIsAtMaxSpeed(Mob *mob);

/**
 * @brief Resets the mob's velocity to default if stopped.
 *
 * @param mob Pointer to the Mob structure.
 */
void mobStopped(Mob *mob);

/**
 * @brief Renders the mob on the screen.
 *
 * @param mob Pointer to the Mob structure.
 * @param frame_buffer Pointer to the frame buffer.
 */
void draw_mob(Mob *mob, uint8_t *frame_buffer);

#endif /* MOB_H */
