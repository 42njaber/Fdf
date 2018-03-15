SRC = init.c \
	  main.c \
	  display.c \
	  key_hooks.c \
	  key_handler.c \
	  mouse_hooks.c \
	  motion_hooks.c \
	  loop_hooks.c

NAME = fdf
FLAGS = -Wall -Wextra -Werror

OBJ = $(SRC:.c=.o)

LIB = libgxns/libgxns.a
CFLAGS = -Llibgxns -lgxns -framework OpenGL -framework AppKit -framework OpenCL
CC = gcc
RM = @rm -fv
TEST_FILE = test.out
.PHONY: all, test, clean, fclean, re, force

all: $(NAME)

force:
	@true

libgxns/libgxns.a: force
	make -C libgxns/

%.o: %.c fdf.h
	$(CC) $(FLAGS) -o $@ -c $<

$(NAME): $(LIB) $(OBJ) fdf.h Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJ)

clean:
	@echo "Cleaning objects:"
	$(MAKE) -C libgxns/ fclean
	$(RM) $(OBJ)

fclean: clean
	@echo "Cleaning target:"
	$(RM) $(NAME)
	$(RM) $(TEST_FILE)

re: fclean all
