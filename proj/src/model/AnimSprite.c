#include "AnimSprite.h"
#include <stdlib.h>
#include "Sprite.h"
#include <lcom/lcf.h>

AnimSprite *create_animSprite(uint8_t no_pic, char *pics[][]) {
    AnimSprite *asp = malloc(sizeof(AnimSprite));
    // create a standard sprite with first pixmap
    asp->sp = create_sprite(pics[0], 0, 0, 0, 0);
    // allocate array of pointers to pixmaps
    asp->map = malloc((no_pic) * sizeof(char *));
    // initialize the first pixmap
    asp->map[0] = asp->sp->map;
    // load he other pics
    for (int i = 1; i < no_pic; i++) {
        char **tmp = pics[i];
        xpm_image_t img;
        asp->map[i] = xpm_load(tmp, XPM_INDEXED, &img);
        if (asp->map[i] == NULL || img.width != asp->sp->width || img.height != asp-> sp->height) { 
            for (int j = 1; j <= i; j++) // free (NULL) is OK
                free(asp->map[i]);
            free(asp->map);
            destroy_sprite(asp->sp);
            free(asp);
            return NULL;
        }
    }
    return asp;
}