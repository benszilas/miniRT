# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vvobis <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/17 14:23:19 by vvobis            #+#    #+#              #
#    Updated: 2024/09/17 14:23:21 by vvobis           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := libft.a

CC := cc

CFLAGS:= -Wall -Wextra -Werror

SRC := ft_bzero.c ft_isalnum.c ft_isalpha.c	ft_isdigit.c \
	   ft_isprint.c ft_memcpy.c ft_memmove.c ft_memset.c \
	   ft_strlcat.c ft_strlcpy.c ft_strlen.c \
	   ft_isascii.c ft_strchr.c ft_strrchr.c ft_strncmp.c \
	   ft_toupper.c ft_tolower.c ft_memchr.c ft_strnstr.c \
	   ft_atoi.c ft_atod.c ft_memcmp.c ft_calloc.c ft_strdup.c \
	   ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c \
	   ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c \
	   ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
	   ft_isspace.c ft_free.c

SRCGNL := gnl/get_next_line.c gnl/get_next_line_utils.c

SRCPRINT := printf/ft_fprintf.c printf/ft_printf.c printf/ft_putascii.c printf/ft_putptr.c printf/ft_puthex.c printf/ft_putfloat.c

SRCBON := ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c 

OBJBON := $(SRCBON:%.c=%.o)

OBJ := $(SRC:%.c=%.o)

OBJGNL := $(SRCGNL:%.c=%.o)

OBJPRINT := $(SRCPRINT:%.c=%.o)

LIBS := printf

all: $(NAME)

bonus: $(OBJBON) $(OBJ)
	ar rcs libft.a $(OBJ) $(OBJBON)

$(NAME): $(OBJ) $(OBJGNL) $(OBJPRINT) $(LIBS)
	ar rcs libft.a $(OBJ) $(OBJGNL) $(OBJPRINT)

$(LIBS):
	make -C $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

re: fclean all

so:
	$(CC) -fPIC $(CFLAGS) $(SRC) test.c
	gcc -shared -o libft.so $(OBJ) $(OBJBON)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBS)

clean:
	rm -f $(OBJ) $(OBJBON) $(OBJGNL)
	make clean -C $(LIBS)
