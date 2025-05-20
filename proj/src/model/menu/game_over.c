#include "game_over.h"

struct GameOver {
    Button button;
};

GameOver *create_game_over() {
    GameOver *game_over = (GameOver *) malloc(sizeof(GameOver));
    if (game_over == NULL) {
        return NULL;
    }
    game_over->button = BUTTON_NONE;

    return game_over;
}

void destroy_game_over(GameOver *game_over) {
    if (game_over == NULL) return;
    free(game_over);
}

void draw_game_over(GameOver *game_over, uint8_t *frame_buffer) {
    draw_xpm_at_pos_with_color((xpm_map_t) game_over_xpm, 154, 262, 4, frame_buffer);
    draw_button(BUTTON_MENU, game_over->button == BUTTON_MENU, BUTTON_MENU_START_X, BUTTON_MENU_START_Y, frame_buffer);
    draw_button(BUTTON_EXIT, game_over->button == BUTTON_EXIT, BUTTON_EXIT_START_X, BUTTON_EXIT_START_Y, frame_buffer);
}

void game_over_set_button(GameOver *game_over, Button button) {
    if (!game_over) return;
    game_over->button = button;
}

Button game_over_get_button(GameOver *game_over) {
    if (!game_over) return BUTTON_EXIT;
    return game_over->button;
}

Button game_over_click_handler(GameOver *game_over, double x_mouse, double y_mouse) {
    if (x_mouse >= BUTTON_MENU_START_X && x_mouse <= BUTTON_MENU_START_X + BUTTON_WIDTH &&
        y_mouse >= BUTTON_MENU_START_Y && y_mouse <= BUTTON_MENU_START_Y + BUTTON_HEIGHT) {
        game_over->button = BUTTON_MENU;
        return BUTTON_MENU;
    } else if (x_mouse >= BUTTON_EXIT_START_X && x_mouse <= BUTTON_EXIT_START_X + BUTTON_WIDTH &&
               y_mouse >= BUTTON_EXIT_START_Y && y_mouse <= BUTTON_EXIT_START_Y + BUTTON_HEIGHT) {
        game_over->button = BUTTON_EXIT;
        return BUTTON_EXIT;
    }
    return BUTTON_NONE;
}
