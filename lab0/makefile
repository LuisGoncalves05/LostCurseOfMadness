CC = cc
CFLAGS = -Wall -Wextra -g
TARGET = my_program
SRC = main.c
OBJ = $(SRC:.c=.o)

STATIC_LIB = libhelper.a
DYNAMIC_LIB = libhelper.so

# Build Static Library
$(STATIC_LIB): helper.o
	ar rcs $(STATIC_LIB) helper.o

# Build Dynamic Library
$(DYNAMIC_LIB): helper.o
	$(CC) -shared -o $(DYNAMIC_LIB) helper.o

# Build Object Files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile main with the Static Library
static: $(OBJ) $(STATIC_LIB)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) -L. -lhelper

# Compile main with the Dynamic Library
dynamic: $(OBJ) $(DYNAMIC_LIB)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) -L. -lhelper

# Run with dynamic linking
run_dynamic:
	LD_LIBRARY_PATH=. ./$(TARGET)

.PHONY: clean
clean:
	rm -f $(OBJ) $(TARGET) helper.o $(STATIC_LIB) $(DYNAMIC_LIB)
