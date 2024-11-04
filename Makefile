# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/08 16:30:33 by victor            #+#    #+#              #
#    Updated: 2024/11/04 10:57:44 by bszilas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC := cc

CFLAGS := -Wall -Wextra -g3

LDFLAGS := -lm -lXext -lX11

NAME := miniRT

SRCDIR := src

SRC := 	minirt.c

SCENEDIR := scene
SRCSCENE :=	$(addprefix $(SCENEDIR)/, $(addsuffix .c,\
			camera light scene plane sphere \
			cylinder disk add_body cone))

RENDERDIR := rendering
SRCRENDER :=	$(addprefix $(RENDERDIR)/, $(addsuffix .c,\
				rendering image pixel ray_utils color drawing threads))

IODIR := io
SRCIO :=	$(addprefix $(IODIR)/, $(addsuffix .c,\
			mouse menu keys parsing glyph container))

UTILDIR := utils
SRCUTIL :=	$(addprefix $(UTILDIR)/, $(addsuffix .c,\
			ft_atod minirt_utils vector_math vector \
			detect_overflow_utils detect_overflow))

LIBS := libft/libft.a memory/memory.a minilibx-linux/libmlx.a

OBJDIR := obj
OBJ := $(SRC:%.c=$(OBJDIR)/%.o)
OBJRENDER := $(SRCRENDER:%.c=$(OBJDIR)/%.o)
OBJSCENE := $(SRCSCENE:%.c=$(OBJDIR)/%.o)
OBJIO := $(SRCIO:%.c=$(OBJDIR)/%.o)
OBJUTIL := $(SRCUTIL:%.c=$(OBJDIR)/%.o)

all: $(OBJDIR) $(NAME)

$(NAME): $(OBJ) $(LIBS) minirt.h $(OBJIO) $(OBJSCENE) $(OBJUTIL) $(OBJRENDER)
	make -C minilibx-linux
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(OBJIO) $(OBJSCENE) $(OBJUTIL) $(OBJRENDER) $(LIBS) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/$(SCENEDIR)/%.c minirt.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/$(RENDERDIR)/%.c minirt.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/$(UTILDIR)/%.c minirt.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/$(IODIR)/%.c minirt.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c minirt.h
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBS):
	make -C libft
	make -C memory

$(OBJDIR):
	mkdir -p $(OBJDIR) $(OBJDIR)/$(SRCDIR) $(OBJDIR)/$(RENDERDIR) $(OBJDIR)/$(IODIR) $(OBJDIR)/$(UTILDIR) $(OBJDIR)/$(SCENEDIR)

clean:
	rm -rf $(OBJDIR)
	make clean -C libft

fclean: clean
	rm -f $(NAME)
	make fclean -C libft
	make fclean -C memory

re: fclean all
