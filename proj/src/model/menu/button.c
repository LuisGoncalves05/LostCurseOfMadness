#include "button.h"

void draw_button(Button button, bool selected, uint16_t x, uint16_t y, uint8_t *frame_buffer) {
    uint32_t color = selected? 4 : 28;
    switch (button) {
        case BUTTON_PLAY:
            draw_xpm_at_pos_with_color((xpm_map_t)play_button, x, y, color, frame_buffer);
            break;
        case BUTTON_MENU:
            draw_xpm_at_pos_with_color((xpm_map_t)menu_button, x, y, color, frame_buffer);
            break;
        case BUTTON_EXIT:
            draw_xpm_at_pos_with_color((xpm_map_t)exit_button, x, y, color, frame_buffer);
            break;
        default:
            return; // Invalid button
    }
}
