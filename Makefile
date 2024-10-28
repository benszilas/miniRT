# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/08 16:30:33 by victor            #+#    #+#              #
#    Updated: 2024/10/28 14:57:17 by bszilas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC := cc

CFLAGS := -Wall -Wextra -g3

LDFLAGS := -lm -lXext -lX11

NAME := miniRT

SRCDIR := src

SRC := 	minirt.c

SCENEDIR := scene
SRCSCENE := $(SCENEDIR)/camera.c $(SCENEDIR)/light.c \
			$(SCENEDIR)/scene.c $(SCENEDIR)/plane.c \
			$(SCENEDIR)/sphere.c $(SCENEDIR)/cylinder.c \
			$(SCENEDIR)/disk.c $(SCENEDIR)/add_body.c \
			$(SCENEDIR)/cone.c

RENDERDIR := rendering
SRCRENDER :=	$(RENDERDIR)/image.c $(RENDERDIR)/pixel.c \
				$(RENDERDIR)/ray_utils.c $(RENDERDIR)/rendering.c \
				$(RENDERDIR)/color.c  $(RENDERDIR)/drawing.c \

IODIR := io
SRCIO :=	$(IODIR)/mouse.c $(IODIR)/menu.c $(IODIR)/keys.c \
			$(IODIR)/parsing.c $(IODIR)/glyph.c \
			$(IODIR)/container.c

UTILDIR := utils
SRCUTIL :=	$(UTILDIR)/ft_atod.c \
		  	$(UTILDIR)/minirt_utils.c $(UTILDIR)/vector_math.c \
		  	$(UTILDIR)/vector.c $(UTILDIR)/detect_overflow_utils.c \
		 	$(UTILDIR)/detect_overflow.c

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
