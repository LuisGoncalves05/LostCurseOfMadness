#include "game_over.h"


struct GameOver {
    int button;
};

GameOver *create_game_over() {
    GameOver *game_over = (GameOver *) malloc(sizeof(GameOver));
    if (game_over == NULL) {
        return NULL;
    }
    game_over->button = 0;

    return game_over;
}

void destroy_game_over(GameOver *game_over) {
    if (game_over == NULL) return;
    free(game_over);
}

void draw_game_over(GameOver *game_over, uint8_t *frame_buffer) {
    draw_xpm_at_pos((xpm_map_t) game_over_indexed_xpm, 0, 0, frame_buffer);
}
