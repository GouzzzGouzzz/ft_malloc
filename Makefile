ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME=libft_malloc_$(HOSTTYPE).so
CFLAGS=-fPIC -g -Wall -Werror -Wextra
LIBFT=-Llibft/ -lft
LIBMALLOC=-L. -lft_malloc
CC=gcc
SYMLINK_NAME=libft_malloc.so
OBJ_DIR=obj

MANDATORY= malloc.c\
	realloc.c\
	free.c\
	show_mem.c\
	utils.c

OBJ=$(MANDATORY:%.c=$(OBJ_DIR)/%.o)

all: $(SYMLINK_NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $(LIBFT) -o $@ $<

$(SYMLINK_NAME): $(NAME)
	rm -f $(SYMLINK_NAME)
	ln -s $(NAME) $(SYMLINK_NAME)

$(NAME): $(OBJ)
	$(MAKE) -C ./libft/ all
	$(CC) $(CFLAGS) -shared -o $(NAME) $(OBJ)

clean:
	$(MAKE) -C ./libft/ clean
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C ./libft/ fclean
	rm -f $(NAME) $(SYMLINK_NAME) main.o main

test: re
	$(CC) $(CFLAGS) -c main.c -o main.o
	$(CC) main.o -o main $(LIBMALLOC) $(LIBFT) -Wl,-rpath,./

re: fclean all

.PHONY: clean fclean all re test
