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

static inline int clamp(int val, int min, int max) {
    return (val < min) ? min : (val > max) ? max : val;
}

static void cursor_check_bound(Cursor *cursor) {
    if (!cursor)
        return;

    Sprite *s = cursor->sprite;

    s->x = clamp(s->x, 0, x_res - s->width);
    s->y = clamp(s->y, 0, y_res - s->height);
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
