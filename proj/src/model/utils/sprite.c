#include <stdlib.h>
#include "sprite.h"

/* Create and destroy section */

Sprite *create_sprite(xpm_map_t xpm, int x, int y, double xspeed, double yspeed) {
    Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
    xpm_image_t img;
    if (sp == NULL)
        return NULL;
    sp->map = xpm_load(xpm, XPM_INDEXED, &img);
    if (sp->map == NULL) {
        free(sp);
        return NULL;
    }
    sp->x = x;
    sp->y = y;
    sp->width = img.width;
    sp->height = img.height;
    sp->xspeed = xspeed;
    sp->yspeed = yspeed;
    return sp;
}

void destroy_sprite(Sprite *sp) {
    if (sp == NULL)
        return;
    if (sp->map)
        free(sp->map);
    free(sp);
}

/* Getter and setter section */

/* Statics section */

/* Others section */

/* Draw section */
