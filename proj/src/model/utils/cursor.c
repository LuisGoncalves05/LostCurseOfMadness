#include "cursor.h"

struct Cursor {
    Sprite *sprite;
    CursorMode mode;
};

/* Create and destroy section */

Cursor *create_cursor(CursorMode mode, int x, int y) {
    Cursor *cursor = (Cursor *) malloc(sizeof(Cursor));
    if (cursor == NULL) {
        return NULL;
    }
    
    if (mode == CURSOR_DEFAULT) {
        cursor->sprite = create_sprite((xpm_map_t) cursor_xpm, x, y, 0, 0);
    } else {
        cursor->sprite = create_sprite((xpm_map_t) crosshair_xpm, x_res / 2, y_res / 2, 0, 0);
    }
    if (cursor->sprite == NULL) {
        free(cursor);
        return NULL;
    }
    cursor->mode = mode;
    
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
    if (cursor->mode == CURSOR_CROSSHAIR)
        return cursor->sprite->x + cursor->sprite->width / 2;
    return cursor->sprite->x;
}

uint16_t cursor_get_y(Cursor *cursor) {
    if (cursor == NULL)
        return -1;
    if (cursor->mode == CURSOR_CROSSHAIR)
        return cursor->sprite->y + cursor->sprite->height / 2;
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
    draw_transparent_sprite(cursor->sprite, frame_buffer);
}
