#include "game.h"

#include "controller/video/gpu.h"
#include "controller/keyboard/keyboard.h"
#include "controller/mouse/mouse.h"
#include "controller/timer/i8254.h"
#include "view/view.h"
#include "model/Sprite.h"

extern uint8_t *sec_frame_buffer;
extern uint8_t *main_frame_buffer;

#include "controller/keyboard/i8042.h"
extern uint8_t scan_code;

struct Game {
    State state;
    uint8_t level_number;
    Level *level;
};

Game *create_game() {
    Game *game = (Game *)malloc(sizeof(Game));
    if (!game) return NULL;

    game->state = LEVEL;
    game->level_number = 0;
    game->level = create_level(game->level_number);

    return game;
}

void destroy_game(Game *game) {
    if (game) {
        destroy_level(game->level);
        free(game);
    }
}

State get_state(Game *game) {
    return game->state;
}

void main_game_loop(){
    clear(sec_frame_buffer);
    draw_player();
    copy_frame_buffer();
}


/* static */ void menu_init() {
    printf("To be implemented\n");    
}

/* static */ void level_init() {
    printf("To be implemented\n");    
}

/* static */ void victory_init() {
    printf("To be implemented\n");    
}

/* static */ void game_over_init() {
    printf("To be implemented\n");    
}

/* static */ void exit_init() {
    printf("To be implemented\n");    
}

/* static */ void (*game_init[])() = {
    menu_init,
    level_init,
    victory_init,
    game_over_init,
    exit_init
};

// destroy state functions

/* static */ void set_state(Game* game, State new_state) {
    printf("Changing state from %d to %d\n", game->state, new_state);
}

/* static */ void menu_keyboard_handler(Game* game) {
    printf("To be implemented\n");    
}

/* static */ void level_keyboard_handler(Game* game) {
    if (scan_code == ESC_BREAK_CODE) set_state(game, EXIT);
}

/* static */ void victory_keyboard_handler(Game* game) {
    printf("To be implemented\n");    
}

/* static */ void game_over_keyboard_handler(Game* game) {
    printf("To be implemented\n");    
}

/* static */ void exit_keyboard_handler(Game* game) {
    printf("To be implemented\n");    
}

/* static */ void (*game_keyboard_handlers[])(Game *game) = {
    menu_keyboard_handler,
    level_keyboard_handler,
    victory_keyboard_handler,
    game_over_keyboard_handler,
    exit_keyboard_handler
};

static void menu_timer_handler(Game* game) {
    printf("in menu\n");    
}

static void level_timer_handler(Game* game) {
    main_game_loop();    
}

static void victory_timer_handler(Game* game) {
    printf("Win!\n");    
}

static void game_over_timer_handler(Game* game) {
    printf("GAME OVER\n");    
}

static void exit_timer_handler(Game* game) {
    printf("exiting\n");    
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
