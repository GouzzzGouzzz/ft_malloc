ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME=libft_malloc_$(HOSTTYPE).so
CFLAGS=#-Wall -Werror -Wextra
CC=gcc
SYMLINK_NAME=libft_malloc.so
OBJ_DIR=obj

MANDATORY= malloc.c\
	realloc.c\
	free.c

OBJ=$(MANDATORY:%.c=$(OBJ_DIR)/%.o)

all: $(SYMLINK_NAME)

$(OBJ_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(SYMLINK_NAME): $(NAME)
ifeq ($(wildcard $(SYMLINK_NAME)),)
	ln -s $(NAME) $(SYMLINK_NAME)
endif

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
	$(CC) -c main.c -o main.o
	gcc main.o -o main -L. -lft_malloc

re: fclean all


.PHONY: all clean fclean re test
