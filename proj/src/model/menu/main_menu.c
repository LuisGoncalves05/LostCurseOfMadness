#include "main_menu.h"

/**
 * @brief Represents a single MainMenu instance.
 *
 * The MainMenu struct is opaque.
 */
struct MainMenu {
    Button *playButton; /**< Pointer to the play button */
    Button *exitButton; /**< Pointer to the exit button */
    Sprite *title;      /**< Pointer to the game text sprite */
};

/* Create and destroy section */

MainMenu *create_main_menu() {
    MainMenu *main_menu = (MainMenu *) malloc(sizeof(MainMenu));
    if (main_menu == NULL) {
        printf("create_main_menu: malloc failed\n");
        return NULL;
    }

    main_menu->playButton = create_button((xpm_map_t) play_button, (xpm_map_t) play_button_selected, BUTTON_PLAY_START_X, MAIN_MENU_BUTTON_START_Y);
    if (main_menu->playButton == NULL) {
        printf("create_main_menu: create_button (playButton) failed\n");
        free(main_menu);
        return NULL;
    }

    main_menu->exitButton = create_button((xpm_map_t) main_exit_button, (xpm_map_t) main_exit_button_selected, BUTTON_EXIT_START_X, MAIN_MENU_BUTTON_START_Y);
    if (main_menu->exitButton == NULL) {
        printf("create_main_menu: create_button (exitButton) failed\n");
        destroy_button(main_menu->playButton);
        free(main_menu);
        return NULL;
    }

    main_menu->title = create_sprite(lcom_xpm, x_res / 2 - 801 / 2, y_res / 2 - 300, 0, 0);
    if (!main_menu->title) {
        printf("create_main_menu: create_sprite failed\n");
        destroy_button(main_menu->playButton);
        destroy_button(main_menu->exitButton);
        free(main_menu);
        return NULL;
    }

    return main_menu;
}

void destroy_main_menu(MainMenu *main_menu) {
    if (main_menu == NULL) {
        printf("destroy_main_menu: NULL pointer provided\n");
        return;
    }
    destroy_sprite(main_menu->title);
    destroy_button(main_menu->playButton);
    destroy_button(main_menu->exitButton);
    free(main_menu);
}

/* Getter and setter section */

ButtonType main_menu_get_button(MainMenu *main_menu) {
    if (main_menu == NULL) {
        return BUTTON_NONE;
    }
    if (button_get_selected(main_menu->playButton)) {
        return BUTTON_PLAY;
    }
    if (button_get_selected(main_menu->exitButton)) {
        return BUTTON_EXIT;
    }
    return BUTTON_NONE;
}

/* Statics section */

/* Others section */

void main_menu_change_button(MainMenu *main_menu) {
    if (main_menu == NULL) {
        printf("main_menu_change_button: NULL pointer provided\n");
        return;
    }
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
    if (main_menu == NULL) {
        printf("main_menu_click_handler: NULL pointer provided\n");
        return BUTTON_NONE;
    }
    if (button_is_selected(main_menu->playButton, x, y)) {
        return BUTTON_PLAY;
    }
    if (button_is_selected(main_menu->exitButton, x, y)) {
        return BUTTON_EXIT;
    }
    return BUTTON_NONE;
}

/* Draw section */

void draw_main_menu(MainMenu *main_menu, uint8_t *frame_buffer) {
    if (main_menu == NULL) {
        printf("draw_main_menu: NULL pointer provided\n");
        return;
    }
    draw_sprite(main_menu->title, frame_buffer);
    draw_button(main_menu->playButton, frame_buffer);
    draw_button(main_menu->exitButton, frame_buffer);
}
