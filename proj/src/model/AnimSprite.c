#include "AnimSprite.h"
#include <stdlib.h>
#include "Sprite.h"
#include <lcom/lcf.h>

AnimSprite *create_animSprite(uint8_t no_pic, char ***pics) {
    if (pics == NULL || no_pic == 0) return NULL;

    // Allocate memory for the AnimSprite structure
    AnimSprite *asp = malloc(sizeof(AnimSprite));
    if (asp == NULL) return NULL;

    // Create the first sprite
    asp->sp = create_sprite(pics[0], 0, 0, 0, 0);
    if (asp->sp == NULL) {
        free(asp);
        return NULL;
    }

    // Allocate memory for the pixmap array
    asp->map = malloc(no_pic * sizeof(char *));
    if (asp->map == NULL) {
        destroy_sprite(asp->sp);
        free(asp);
        return NULL;
    }

    // Initialize first pixmap from the created sprite
    asp->map[0] = asp->sp->map;

    // Load the remaining images
    for (int i = 1; i < no_pic; i++) {
        xpm_image_t img;
        asp->map[i] = xpm_load(pics[i], XPM_INDEXED, &img);
        if (asp->map[i] == NULL || img.width != asp->sp->width || img.height != asp->sp->height) {
            // Cleanup on failure
            for (int j = 1; j < i; j++) {
                free(asp->map[j]); // Safe to free NULL
            }
            free(asp->map);
            destroy_sprite(asp->sp);
            free(asp);
            return NULL;
        }
    }

    return asp;
}
+
