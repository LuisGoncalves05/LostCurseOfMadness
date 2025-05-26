#include "cursor.h"

struct Cursor {
    Sprite *sprite;
};

/* Create and destroy section */

Cursor *create_cursor(xpm_map_t xpm) {
    Cursor *cursor = (Cursor *) malloc(sizeof(Cursor));
    if (cursor == NULL) {
        return NULL;
    }
    cursor->sprite = create_sprite(xpm, 0, 0, 0, 0);
    if (cursor->sprite == NULL) {
        free(cursor);
        return NULL;
    }
    return cursor;
}

void destroy_cursor(Cursor *cursor) {
    if (cursor == NULL)
        return;
    destroy_sprite(cursor->sprite);
    free(cursor);
}

/* Getter and setter section */

uint16_t cursor_get_x(Cursor *cursor) {
    if (cursor == NULL)
        return -1;
    return cursor->sprite->x;
}

uint16_t cursor_get_y(Cursor *cursor) {
    if (cursor == NULL)
        return -1;
    return cursor->sprite->y;
}

/* Statics section */

static void cursor_check_bound(Cursor *cursor) {
    if (cursor == NULL)
        return;
    if (cursor->sprite->x > x_res - cursor->sprite->width)
        cursor->sprite->x = x_res - cursor->sprite->width;
    if (cursor->sprite->y > y_res - cursor->sprite->height)
        cursor->sprite->y = y_res - cursor->sprite->height;
    if (cursor->sprite->x < 0)
        cursor->sprite->x = 0;
    if (cursor->sprite->y < 0)
        cursor->sprite->y = 0;
}

/* Others section */

void cursor_update(Cursor *cursor, double dx, double dy) {
    if (cursor == NULL)
        return;
    cursor->sprite->x += dx;
    cursor->sprite->y += dy;
    cursor_check_bound(cursor);
}

/* Draw section */

void draw_cursor(Cursor *cursor, uint8_t *frame_buffer) {
    if (cursor == NULL)
        return;
    draw_sprite(cursor->sprite, frame_buffer);
}
