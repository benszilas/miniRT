# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: victor </var/spool/mail/victor>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/08 17:46:41 by victor            #+#    #+#              #
#    Updated: 2024/09/15 12:18:25 by victor           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC := cc

CFLAGS := -Wall -Wextra -Werror -g3

NAME := memory.a

SRC := memory.c list.c

OBJ := $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBS)
	ar rcs $(NAME) $(OBJ)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
