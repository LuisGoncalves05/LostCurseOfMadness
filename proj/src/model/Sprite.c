#include <stdlib.h>
#include "Sprite.h"
#include <lcom/lcf.h>

/**
* Creates a new sprite with pixmap "pic",
* with specified position (within the
* screen limits) and speed;
* Does not draw the sprite on the screen.
*
* @param pic lines of strings, same 
* as xpm_map_t (has const protectors)
* @return NULL on invalid pixmap.
*/
Sprite *create_sprite(const char *pic[], 
    int x, int y, int xspeed, int yspeed) {
    // allocate space for the "object"
    Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
    xpm_image_t img;
    if (sp == NULL)
    return NULL;
    // read the sprite pixmap
    sp->map = xpm_load(pic, XPM_INDEXED, &img);
    if (sp->map == NULL) {
        free(sp);
        return NULL;
    }
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
    // caller should put the pointer to NULL
}


