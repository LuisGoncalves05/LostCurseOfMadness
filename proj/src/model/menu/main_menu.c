#include "main_menu.h"

struct MainMenu {
    Button button;
};

MainMenu *create_main_menu() {
    MainMenu *main_menu = (MainMenu *) malloc(sizeof(MainMenu));
    if (main_menu == NULL) {
        return NULL;
    }
    main_menu->button = BUTTON_NONE;

    return main_menu;
}

void destroy_main_menu(MainMenu *main_menu) {
    if (main_menu == NULL) return;
    free(main_menu);
}

void draw_main_menu(MainMenu *main_menu, uint8_t *frame_buffer) {
    draw_xpm_at_pos_with_color((xpm_map_t) lcom_xpm, 112, 184, 4, frame_buffer);
}

void main_menu_set_button(MainMenu *main_menu, Button button) {
    if (main_menu == NULL) return;
    main_menu->button = button;
}

Button main_menu_get_button(MainMenu *main_menu) {
    if (main_menu == NULL) return BUTTON_EXIT;
    return main_menu->button;
}
