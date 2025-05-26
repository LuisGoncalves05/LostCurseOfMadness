/**
 * @file mob.h
 *
 * @brief This file contains the declarations of the functions and "class" for the mob module.
 * The mob module is responsible for managing the mob logic and handling changes in the internal state of the mob.
 */

/** @defgroup mob Mob
 *  @brief mob module for managing mob logic
 *  @{
 */
#ifndef MOB_H
#define MOB_H

#include "drivers/video/gpu.h"
#include "model/utils/sprite.h"
#include <lcom/lcf.h>

#define MOB_MAX_SPEED 5     // Maximum speed of the mob
#define MOB_ACCELERATION 1  // Acceleration of the mob
#define MOB_HEALTH 1        // Health of the mob
#define MOB_DEFAULT_SPEED 2 // Default speed of the mob

typedef struct Mob Mob;

enum mob_state {
    MOB_IDLE,
    MOB_WALKING,
    MOB_ATTACKING,
    MOB_DYING,
};

/**
 * @brief function to prevent the mob from moving faster than the maximum speed.
 */
void mobIsAtMaxSpeed(Mob *mob);
/**
 * @brief function to update the mob velocity to default if he stops moving.
 */
void mobStopped(Mob *mob);
/**
 * @brief constructs a mob "object".
 */
Mob *create_mob(Sprite *sprite);
/**
 * @brief Destroys the mob "object" and frees its memory.
 */
void destroy_mob(Mob *mob);
/**
 * @brief returns the sprite associated with the mob.
 */
Sprite *mob_get_sprite(Mob *mob);
/**
 * @brief returns the current health of the mob.
 */
unsigned char mob_get_health(Mob *mob);
/**
 * @brief sets the current health of the mob.
 */
void mob_set_health(Mob *mob, unsigned char health);
/**
 * @brief returns the maximum speed of the mob.
 */
int mob_get_max_speed(Mob *mob);
/**
 * @brief returns the acceleration of the mob.
 */
int mob_get_acceleration(Mob *mob);
/**
 * @brief returns true if the mob moved, false otherwise.
 */
bool mob_get_moved(Mob *mob);
/**
 * @brief sets the flag if the mob moved or not.
 */
void mob_set_moved(Mob *mob, bool moved);
/**
 * @brief updates the mob state.
 */
void update_mob_state(Mob *mob);
/**
 * @brief sets the mob with a new sprite.
 */
void mob_set_sprite(Mob *mob, Sprite *sprite);
/**
 * @brief returns the mob state.
 */
enum mob_state mob_get_state(Mob *mob);

#endif
/** @} */
