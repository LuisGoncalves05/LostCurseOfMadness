#include "button.h"
#include "../utils/sprite.h"

/**
 * @brief Represents a Button instance
 *
 * The Button struct is opaque.
 */
struct Button {
    bool selected;           /**< Indicates whether the button is currently selected */
    Sprite *sprite;          /**< Pointer to the sprite representing the button */
    Sprite *selected_sprite; /**< Pointer to the sprite representing the button when selected */
    uint16_t x;              /**< X-coordinate of the button */
    uint16_t y;              /**< Y-coordinate of the button */
    uint16_t width;          /**< Width of the button */
    uint16_t height;         /**< Height of the button */
};

/* Create and destroy section */

Button *create_button(xpm_map_t xpm, xpm_map_t xpm_selected, uint16_t x, uint16_t y) {
    Button *button = (Button *) malloc(sizeof(Button));
    if (button == NULL) {
        printf("create_button: malloc failed\n");
        return NULL;
    }

    button->selected = false;
    button->x = x;
    button->y = y;
    button->width = BUTTON_WIDTH;
    button->height = BUTTON_HEIGHT;

    button->sprite = create_sprite(xpm, x, y, 0, 0);
    if (button->sprite == NULL) {
        printf("create_button: create_sprite failed\n");
        free(button);
        return NULL;
    }

    button->selected_sprite = create_sprite(xpm_selected, x, y, 0, 0);
    if (!button->selected_sprite) {
        printf("create_button: create_sprite failed\n");
        destroy_sprite(button->sprite);
        free(button);
        return NULL;
    }

    return button;
}

void destroy_button(Button *button) {
    if (button == NULL) {
        printf("destroy_button: NULL pointer provided\n");
        return;
    }
    destroy_sprite(button->sprite);
    destroy_sprite(button->selected_sprite);
    free(button);
}

/* Getter and setter section */

bool button_get_selected(Button *button) {
    if (button == NULL) {
        printf("button_get_selected: NULL pointer provided\n");
        return false;
    }

    return button->selected;
}

void button_set_selected(Button *button, bool selected) {
    if (button == NULL) {
        printf("button_get_selected: NULL pointer provided\n");
        return;
    }

    button->selected = selected;
}

/* Statics section */

/* Others section */

bool button_is_selected(Button *button, uint16_t x, uint16_t y) {
    bool clicked = (x >= button->x && x <= button->x + button->width &&
                    y >= button->y && y <= button->y + button->height);
    if (clicked) {
        button->selected = true;
    }

    return clicked;
}

/* Draw section */

void draw_button(Button *button, uint8_t *frame_buffer) {
    if (button == NULL || frame_buffer == NULL) {
        printf("draw_button: NULL pointer provided");
        return;
    }

    if (button->selected) {
        draw_sprite(button->selected_sprite, frame_buffer);
    } else {
        draw_sprite(button->sprite, frame_buffer);
    }
}
