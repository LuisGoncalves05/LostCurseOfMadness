#include "button.h"
#include "../utils/sprite.h"

struct Button {
    bool selected;
    Sprite *sprite;
    Sprite *selected_sprite;
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
};

/* Create and destroy section */

Button *create_button(xpm_map_t xpm, xpm_map_t xpm_selected, uint16_t x, uint16_t y) {
    Button *button = (Button *) malloc(sizeof(Button));
    if (!button)
        return NULL;

    button->selected = false;
    button->x = x;
    button->y = y;
    button->width = BUTTON_WIDTH;
    button->height = BUTTON_HEIGHT;

    button->sprite = create_sprite(xpm, x, y, 0, 0);
    if (!button->sprite) {
        free(button);
        return NULL;
    }

    button->selected_sprite = create_sprite(xpm_selected, x, y, 0, 0);
    if (!button->selected_sprite) {
        destroy_sprite(button->sprite);
        free(button);
        return NULL;
    }

    return button;
}

void destroy_button(Button *button) {
    if (button == NULL)
        return;
    destroy_sprite(button->sprite);
    destroy_sprite(button->selected_sprite);
    free(button);
}

/* Getter and setter section */

bool button_get_selected(Button *button) {
    if (button == NULL)
        return false;
    return button->selected;
}

void button_set_selected(Button *button, bool selected) {
    if (button == NULL)
        return;
    button->selected = selected;
}

/* Statics section */

/* Others section */

bool button_is_clicked(Button *button, uint16_t x, uint16_t y) {
    bool clicked = (x >= button->x && x <= button->x + button->width &&
                    y >= button->y && y <= button->y + button->height);
    if (clicked)
        button->selected = true;
    return clicked;
}

/* Draw section */

void draw_button(Button *button, uint8_t *frame_buffer) {
    if (button->selected) {
        draw_transparent_sprite(button->selected_sprite, frame_buffer);
    } else {
        draw_transparent_sprite(button->sprite, frame_buffer);
    }
}
