#include "game_over.h"

struct GameOver {
    Button *menuButton;
    Button *exitButton;
    Sprite *title;
};

/* Create and destroy section */

GameOver *create_game_over() {
    GameOver *game_over = malloc(sizeof(GameOver));
    if (game_over == NULL) {
        printf("create_game_over: malloc failed\n");
        return NULL;
    }

    game_over->menuButton = create_button((xpm_map_t) menu_button, (xpm_map_t) menu_button_selected, BUTTON_MENU_START_X, GAME_OVER_BUTTON_START_Y);
    if (game_over->menuButton == NULL) {
        printf("create_game_over: create_button failed\n");
        free(game_over);
        return NULL;
    }

    game_over->exitButton = create_button((xpm_map_t) exit_button, (xpm_map_t) exit_button_selected, BUTTON_EXIT_START_X, GAME_OVER_BUTTON_START_Y);
    if (game_over->exitButton == NULL) {
        printf("create_game_over: create_button failed\n");
        destroy_button(game_over->menuButton);
        free(game_over);
        return NULL;
    }

    game_over->title = create_sprite(game_over_xpm, x_res / 2 - 715 / 2, y_res / 2 - 120, 0, 0);
    if (!game_over->title) {
        printf("create_game_over: create_sprite failed\n");
        destroy_button(game_over->menuButton);
        destroy_button(game_over->exitButton);
        free(game_over);
        return NULL;
    }

    return game_over;
}
void destroy_game_over(GameOver *game_over) {
    if (game_over == NULL) {
        printf("destroy_game_over: NULL pointer provided\n");
        return;
    }
    destroy_sprite(game_over->title);
    destroy_button(game_over->menuButton);
    destroy_button(game_over->exitButton);
    free(game_over);
}

/* Getter and setter section */

ButtonType game_over_get_button(GameOver *game_over) {
    if (game_over == NULL) {
        return BUTTON_NONE;
    }
    if (button_get_selected(game_over->menuButton)) {
        return BUTTON_MENU;
    }
    if (button_get_selected(game_over->exitButton)) {
        return BUTTON_EXIT;
    }
    return BUTTON_NONE;
}

/* Statics section */

/* Others section */

void game_over_change_button(GameOver *game_over) {
    if (game_over == NULL) {
        printf("game_over_change_button: NULL pointer provided\n");
        return;
    }

    bool menu_selected = button_get_selected(game_over->menuButton);
    bool exit_selected = button_get_selected(game_over->exitButton);
    if (menu_selected == exit_selected) { // no button selected
        button_set_selected(game_over->menuButton, true);
        button_set_selected(game_over->exitButton, false);
    } else { // one button selected
        button_set_selected(game_over->menuButton, !menu_selected);
        button_set_selected(game_over->exitButton, !exit_selected);
    }
}

ButtonType game_over_click_handler(GameOver *game_over, uint16_t x, uint16_t y) {
    if (game_over == NULL) {
        printf("game_over_click_handler: NULL pointer provided\n");
        return BUTTON_NONE;
    }
    if (button_is_selected(game_over->menuButton, x, y)) {
        return BUTTON_MENU;
    }
    if (button_is_selected(game_over->exitButton, x, y)) {
        return BUTTON_EXIT;
    }
    return BUTTON_NONE;
}

/* Draw section */

void draw_game_over(GameOver *game_over, uint8_t *frame_buffer) {
    if (game_over == NULL) {
        printf("draw_game_over: NULL pointer provided\n");
        return;
    }
    
    draw_sprite(game_over->title, frame_buffer);
    draw_button(game_over->menuButton, frame_buffer);
    draw_button(game_over->exitButton, frame_buffer);
}
