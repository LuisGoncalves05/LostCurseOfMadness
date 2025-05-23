#include "main_menu.h"

struct MainMenu {
    Button *playButton;
    Button *exitButton;
};

MainMenu *create_main_menu() {
    MainMenu *main_menu = (MainMenu *) malloc(sizeof(MainMenu));
    if (main_menu == NULL) {
        return NULL;
    }

    main_menu->playButton = create_button((xpm_map_t) play_button, (xpm_map_t) play_button_selected, BUTTON_PLAY_START_X, MAIN_MENU_BUTTON_START_Y);
    if (main_menu->playButton == NULL) {
        free(main_menu);
        return NULL;
    }

    main_menu->exitButton = create_button((xpm_map_t) main_exit_button, (xpm_map_t) main_exit_button_selected, BUTTON_EXIT_START_X, MAIN_MENU_BUTTON_START_Y);
    if (main_menu->exitButton == NULL) {
        destroy_button(main_menu->playButton);
        free(main_menu);
        return NULL;
    }

    return main_menu;
}

void destroy_main_menu(MainMenu *main_menu) {
    if (main_menu == NULL) return;
    destroy_button(main_menu->playButton);
    destroy_button(main_menu->exitButton);
    free(main_menu);
}

ButtonType main_menu_get_button(MainMenu *main_menu) {
    if (main_menu == NULL) return BUTTON_NONE;
    if (button_get_selected(main_menu->playButton)) return BUTTON_PLAY;
    if (button_get_selected(main_menu->exitButton)) return BUTTON_EXIT;
    return BUTTON_NONE;
}

void main_menu_change_button(MainMenu *main_menu) {
    if (main_menu == NULL) return;
    bool play_selected = button_get_selected(main_menu->playButton);
    bool exit_selected = button_get_selected(main_menu->exitButton);
    if (play_selected == exit_selected) { // no button selected
        button_set_selected(main_menu->playButton, true);
        button_set_selected(main_menu->exitButton, false);
    } else { // one button selected
        button_set_selected(main_menu->playButton, !play_selected);
        button_set_selected(main_menu->exitButton, !exit_selected);
    }
}

ButtonType main_menu_click_handler(MainMenu *main_menu, uint16_t x, uint16_t y) {
    if (main_menu == NULL) return BUTTON_NONE;
    if (button_is_clicked(main_menu->playButton, x, y)) return BUTTON_PLAY;
    if (button_is_clicked(main_menu->exitButton, x, y)) return BUTTON_EXIT;
    return BUTTON_NONE;
}

void draw_main_menu(MainMenu *main_menu, uint8_t *frame_buffer) {
    draw_xpm_at_pos_with_color((xpm_map_t) lcom_xpm, 112, 184, 4, frame_buffer);
    draw_button(main_menu->playButton, frame_buffer);
    draw_button(main_menu->exitButton, frame_buffer);
}
