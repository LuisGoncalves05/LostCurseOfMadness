# name of the program (Minix service)
PROG=lab3

# source code files to be compiled
SRCS = lab3.c kbc.c ../lab2/timer.c ../lab2/utils.c

OBJS = lab3.o kbc.o timer.o utils.o

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic
CPPFLAGS += -D LAB3

../lab2/timer.o: ../lab2/timer.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c ../lab2/timer.c -o ../lab2/timer.o

../lab2/utils.o: ../lab2/utils.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c ../lab2/utils.c -o ../lab2/utils.o

# list of library dependencies (for Lab 3, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
