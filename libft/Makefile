# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nmorandi <nmorandi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/06 14:02:56 by nmorandi          #+#    #+#              #
#    Updated: 2024/07/05 14:23:52 by nmorandi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=libft.a
CFLAGS=-Wall -Werror -Wextra
CC=gcc
OBJ_DIR=obj
MANDATORY=ft_atoi.c	 	\
		ft_bzero.c	 	\
		ft_isalnum.c	\
		ft_isalpha.c	\
		ft_isascii.c	\
		ft_isdigit.c	\
		ft_isprint.c 	\
		ft_memchr.c 	\
		ft_memcmp.c 	\
		ft_memcpy.c 	\
		ft_memmove.c 	\
		ft_memset.c 	\
		ft_putchar_fd.c \
		ft_putendl_fd.c \
		ft_putnbr_fd.c 	\
		ft_putstr_fd.c 	\
		ft_strchr.c 	\
		ft_striteri.c 	\
		ft_strlcat.c 	\
		ft_strlcpy.c 	\
		ft_strlen.c 	\
		ft_strncmp.c 	\
		ft_strnstr.c 	\
		ft_strrchr.c	\
		ft_tolower.c 	\
		ft_toupper.c 	\


OBJ=$(MANDATORY:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) -o $@ $<

$(NAME): $(OBJ)
	ar rc $(NAME) $(OBJ)
	ranlib $(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
