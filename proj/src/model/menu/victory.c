#include "victory.h"

struct Victory {
    Button *nextButton;
    Button *exitButton;
};

Victory *create_victory() {
    Victory *victory = (Victory *) malloc(sizeof(Victory));
    if (victory == NULL)
        return NULL;

    victory->nextButton = create_button((xpm_map_t) next_button, (xpm_map_t) next_button_selected, BUTTON_NEXT_START_X, VICTORY_BUTTON_START_Y);
    if (victory->nextButton == NULL) {
        free(victory);
        return NULL;
    }

    victory->exitButton = create_button((xpm_map_t) victory_exit_button, (xpm_map_t) victory_exit_button_selected, BUTTON_EXIT_START_X, VICTORY_BUTTON_START_Y);
    if (victory->exitButton == NULL) {
        destroy_button(victory->nextButton);
        free(victory);
        return NULL;
    }

    return victory;
}

void destroy_victory(Victory *victory) {
    if (victory == NULL)
        return;
    destroy_button(victory->nextButton);
    destroy_button(victory->exitButton);
    free(victory);
}

ButtonType victory_get_button(Victory *victory) {
    if (victory == NULL)
        return BUTTON_NONE;
    if (button_get_selected(victory->nextButton))
        return BUTTON_NEXT;
    if (button_get_selected(victory->exitButton))
        return BUTTON_EXIT;
    return BUTTON_NONE;
}

void victory_change_button(Victory *victory) {
    if (victory == NULL)
        return;
    bool next_selected = button_get_selected(victory->nextButton);
    bool exit_selected = button_get_selected(victory->exitButton);
    if (next_selected == exit_selected) { // no button selected
        button_set_selected(victory->nextButton, true);
        button_set_selected(victory->exitButton, false);
    } else {
        button_set_selected(victory->nextButton, !next_selected);
        button_set_selected(victory->exitButton, !exit_selected);
    }
}

ButtonType victory_click_handler(Victory *victory, uint16_t x, uint16_t y) {
    if (victory == NULL)
        return BUTTON_NONE;
    if (button_is_clicked(victory->nextButton, x, y))
        return BUTTON_NEXT;
    if (button_is_clicked(victory->exitButton, x, y))
        return BUTTON_EXIT;
    return BUTTON_NONE;
}

void draw_victory(Victory *victory, uint8_t *frame_buffer) {
    vga_draw_xpm((xpm_map_t)victory_xpm, x_res / 2 - 715 / 2, y_res / 2 - 129 / 2, frame_buffer);
    draw_button(victory->nextButton, frame_buffer);
    draw_button(victory->exitButton, frame_buffer);
}
