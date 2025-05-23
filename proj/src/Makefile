# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/assets/xpm
.PATH: ${.CURDIR}/controller
.PATH: ${.CURDIR}/model
.PATH: ${.CURDIR}/model/game
.PATH: ${.CURDIR}/model/menu
.PATH: ${.CURDIR}/controller/keyboard
.PATH: ${.CURDIR}/controller/mouse
.PATH: ${.CURDIR}/controller/timer
.PATH: ${.CURDIR}/controller/video
.PATH: ${.CURDIR}/view

# source code files to be compiled
SRCS = main.c utils.c timer.c keyboard.c mouse.c gpu.c Sprite.c kbc.c view.c game.c player.c level.c maze.c button.c main_menu.c game_over.c 

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>