ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME=libft_malloc_$(HOSTTYPE).so
CFLAGS=-Wall -Werror -Wextra
CC=gcc
SYMLINK_NAME=libft_malloc.so
OBJ_DIR=obj

MANDATORY= main.c
OBJ=$(MANDATORY:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(NAME): $(OBJ)
	$(MAKE) -C ./libft/ all
	$(CC) -shared -o $(NAME) $(OBJ)
	ln -s $(NAME) $(SYMLINK_NAME)

clean:
	$(MAKE) -C ./libft/ clean
	rm -f $(OBJ_DIR)/*.o

fclean: clean
	$(MAKE) -C ./libft/ fclean
	rm -f $(NAME)
	rm -f $(SYMLINK_NAME)

re: fclean all


.PHONY: all clean fclean re bonus
