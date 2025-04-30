# name of the program (Minix service)
PROG=lab5

# source code files to be compiled
SRCS = lab5.c gpu.c ../lab3/keyboard.c ../lab3/kbc.c ../lab2/timer.c ../lab2/utils.c

OBJS = lab5.o gpu.o keyboard.o kbc.o timer.o utils.o

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

../lab3/keyboard.o: ../lab3/keyboard.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c ../lab3/keyboard.c -o ../lab3/keyboard.o

../lab3/kbc.o: ../lab3/kbc.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c ../lab3/kbc.c -o ../lab3/kbc.o

../lab2/utils.o: ../lab2/utils.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c ../lab2/utils.c -o ../lab2/utils.o

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
