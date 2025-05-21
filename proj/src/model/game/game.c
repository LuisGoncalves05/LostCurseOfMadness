#include "game.h"

extern uint8_t *sec_frame_buffer;
extern uint8_t *main_frame_buffer;

extern uint8_t scan_code;
extern struct packet pp;

struct Game {
    State state;
    uint8_t level_number;
    uint8_t score;
    struct Level *level;
    union {
        struct GameOver *game_over;
    } menu;
};

static void menu_init(Game *game) {
    printf("in menu\n");
}

static void level_init(Game *game) {
    game->level = create_level(game->level_number);
}

static void victory_init(Game *game) {
    game->score = game->level_number;
    game->level_number++;
}

static void game_over_init(Game *game) {
    game->score = game->level_number;    
    game->level_number = 0;

    game->menu.game_over = create_game_over();
}

static void exit_init(Game *game) {
    printf("exiting game\n");
}

static void (*game_init[])(Game *game) = {
    menu_init,
    level_init,
    victory_init,
    game_over_init,
    exit_init
};

static void state_init(Game* game) {
    game_init[game->state](game);
}

static void menu_destroy(Game* game) {
    printf("exiting menu\n");
}

static void level_destroy(Game* game) {
    printf("exiting level\n");
    destroy_level(game->level);
}

static void victory_destroy(Game* game) {
    printf("exiting victory state\n");    
}

static void game_over_destroy(Game* game) {
    destroy_game_over(game->menu.game_over);
}

static void exit_destroy(Game* game) {
    printf("This function should never be called\n");    
}

static void (*game_destroy[])(Game *game) = {
    menu_destroy,
    level_destroy,
    victory_destroy,
    game_over_destroy,
    exit_destroy
};

static void state_destroy(Game* game) {
    game_destroy[game->state](game);
}

static void set_state(Game* game, State new_state) {
    // printf("Changing state from %d to %d\n", game->state, new_state);
    state_destroy(game);
    game->state = new_state;
    state_init(game);
}

static void draw_cursor() {
    draw_xpm_at_pos((xpm_map_t) cross, (int) x_mouse, (int) y_mouse, sec_frame_buffer);
}

static void menu_timer_handler(Game* game) {
}

static void level_timer_handler(Game* game) {
    Level* current_level = game->level;
    if (!current_level) return;
    
    Player* player = get_player(game->level);
    Maze* maze = get_maze(current_level);
    if (!player || !maze) return;
    
    // Limpa o buffer secundário
    cursor_check_bound();
    clear(sec_frame_buffer);
    
    clear(maze_buffer);
    draw_maze(maze, maze_buffer);
    game_draw_fov_cone(player, maze);
    draw_player(player);
    draw_player(get_player(game->level));
    update_player_state(get_player(game->level), pp);
    game_draw_cursor();
    draw_cursor();
    
    // Copia o buffer secundário para o buffer principal
    copy_frame_buffer();
}

static void victory_timer_handler(Game* game) {
    printf("Win!\n");    
}

static void game_over_timer_handler(Game* game) {
    clear(sec_frame_buffer);
    draw_game_over(game->menu.game_over, sec_frame_buffer);
    draw_cursor();

    copy_frame_buffer();
}

static void exit_timer_handler(Game* game) {
    printf("This function should never be called\n");    
}

static void (*game_timer_handlers[])(Game *game) = {
    menu_timer_handler,
    level_timer_handler,
    victory_timer_handler,
    game_over_timer_handler,
    exit_timer_handler
};

void game_timer_handler(Game* game) {
    game_timer_handlers[game->state](game);
}

static void menu_keyboard_handler(Game* game) {
    if (scan_code == ESC_BREAK_CODE) set_state(game, EXIT);
}

static void level_keyboard_handler(Game* game) {
    keyboard_handler(get_player(game->level), get_maze(game->level));
    if (scan_code == ESC_BREAK_CODE) set_state(game, EXIT);
}

static void victory_keyboard_handler(Game* game) {
    printf("victory_keyboard_handler: To be implemented\n");    
}

static void game_over_keyboard_handler(Game* game) {
    if (scan_code == ESC_BREAK_CODE) set_state(game, EXIT);  
    if (scan_code == KEY_W || scan_code == KEY_S || scan_code == KEY_A || scan_code == KEY_D) {
        if (game_over_get_button(game->menu.game_over) == BUTTON_MENU) {
            game_over_set_button(game->menu.game_over, BUTTON_EXIT);
        } else {
            game_over_set_button(game->menu.game_over, BUTTON_MENU);
        }
    }
    if (scan_code == KEY_ENTER) {
        if (game_over_get_button(game->menu.game_over) == BUTTON_MENU) {
            set_state(game, MENU);
        } else if (game_over_get_button(game->menu.game_over) == BUTTON_EXIT) {
            set_state(game, EXIT);
        }
    }
}

static void exit_keyboard_handler(Game* game) {
    printf("This function should never be called\n");    
}

static void (*game_keyboard_handlers[])(Game *game) = {
    menu_keyboard_handler,
    level_keyboard_handler,
    victory_keyboard_handler,
    game_over_keyboard_handler,
    exit_keyboard_handler
};

void game_keyboard_handler(Game* game) {
    game_keyboard_handlers[game->state](game);
}

static void menu_mouse_handler(Game* game) {
    printf("menu_mouse_handler: To be implemented\n");    
}

static void level_mouse_handler(Game* game) {
    player_mouse_handler(get_player(game->level), pp);
}

static void victory_mouse_handler(Game* game) {
    printf("victory_mouse_handler: To be implemented\n");    
}

static void game_over_mouse_handler(Game* game) {
    game_over_set_button(game->menu.game_over, BUTTON_NONE);
    GameOverButton button = BUTTON_NONE;
    if (pp.lb) button = game_over_click_handler(game->menu.game_over, x_mouse, y_mouse); 
    if (button == BUTTON_MENU) {
        set_state(game, MENU);
    } else if (button == BUTTON_EXIT) {
        set_state(game, EXIT);
    }
}

static void exit_mouse_handler(Game* game) {
    printf("This function should never be called\n");    
}

static void (*game_mouse_handlers[])(Game *game) = {
    menu_mouse_handler,
    level_mouse_handler,
    victory_mouse_handler,
    game_over_mouse_handler,
    exit_mouse_handler
};

void game_mouse_handler(Game* game) {
    x_mouse += pp.delta_x * 0.5;
    y_mouse -= pp.delta_y * 0.5;
    game_mouse_handlers[game->state](game);
}

/* public functions */

Game *create_game() {
    Game *game = (Game *)malloc(sizeof(Game));
    if (!game) return NULL;

    game->level_number = 0;
    game->score = 0;

    game->state = GAME_OVER;
    state_init(game);

    return game;
}

void destroy_game(Game *game) {
    if (game) {
        free(game);
    }
}

State get_state(Game *game) {
    return game->state;
}
