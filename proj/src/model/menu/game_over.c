#include "game_over.h"

struct GameOver {
    Button *menuButton;
    Button *exitButton;
};

GameOver *create_game_over() {
    GameOver *game_over = (GameOver *) malloc(sizeof(GameOver));
    if (game_over == NULL) return NULL;

    game_over->menuButton = create_button((xpm_map_t) menu_button, (xpm_map_t) menu_button_selected, BUTTON_MENU_START_X, GAME_OVER_BUTTON_START_Y);
    if (game_over->menuButton == NULL) {
        free(game_over);
        return NULL;
    }

    game_over->exitButton = create_button((xpm_map_t) exit_button, (xpm_map_t) exit_button_selected, BUTTON_EXIT_START_X, GAME_OVER_BUTTON_START_Y);
    if (game_over->exitButton == NULL) {
        destroy_button(game_over->menuButton);
        free(game_over);
        return NULL;
    }

    return game_over;
}

void destroy_game_over(GameOver *game_over) {
    if (game_over == NULL) return;
    destroy_button(game_over->menuButton);
    destroy_button(game_over->exitButton);
    free(game_over);
}

void game_over_change_button(GameOver *game_over) {
    if (game_over == NULL) return;
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

ButtonType game_over_get_button(GameOver *game_over) {
    if (game_over == NULL) return BUTTON_NONE;
    if (button_get_selected(game_over->menuButton)) return BUTTON_MENU;
    if (button_get_selected(game_over->exitButton)) return BUTTON_EXIT;
    return BUTTON_NONE;
}

ButtonType game_over_click_handler(GameOver *game_over, double x_mouse, double y_mouse) {
    if (game_over == NULL) return BUTTON_NONE;
    printf("here!\n");
    if (button_is_clicked(game_over->menuButton, x_mouse, y_mouse)) return BUTTON_MENU;
    if (button_is_clicked(game_over->exitButton, x_mouse, y_mouse)) return BUTTON_EXIT;
    return BUTTON_NONE;
}

void draw_game_over(GameOver *game_over, uint8_t *frame_buffer) {
    draw_xpm_at_pos_with_color((xpm_map_t) game_over_xpm, 154, 262, 4, frame_buffer);
    draw_button(game_over->menuButton, frame_buffer);
    draw_button(game_over->exitButton, frame_buffer);
}
