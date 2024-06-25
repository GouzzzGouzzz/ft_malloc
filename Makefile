ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME=libft_malloc_$(HOSTTYPE).so
CFLAGS=-fPIC #-Wall -Werror -Wextra
LIBFT=-Llibft/ -lft
CC=gcc
SYMLINK_NAME=libft_malloc.so
OBJ_DIR=obj

MANDATORY= malloc.c\
	realloc.c\
	free.c

OBJ=$(MANDATORY:%.c=$(OBJ_DIR)/%.o)

all: $(SYMLINK_NAME)

$(OBJ_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(LIBFT) -o $@ $<

$(SYMLINK_NAME): $(NAME)
	rm -f $(SYMLINK_NAME)
	ln -s $(NAME) $(SYMLINK_NAME)

$(NAME): $(OBJ)
	$(MAKE) -C ./libft/ all
	$(CC) -shared -o $(NAME) $(OBJ)

clean:
	$(MAKE) -C ./libft/ clean
	rm -f $(OBJ_DIR)/*.o

fclean: clean
	$(MAKE) -C ./libft/ fclean
	rm -f $(NAME) $(SYMLINK_NAME) main.o main

test: all
	$(CC) $(CFLAGS) -c main.c -o main.o
	gcc main.o -o main -L. -lft_malloc $(LIBFT) -Wl,-R.

re: fclean all

.PHONY: clean fclean all re test
