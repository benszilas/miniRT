# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/08 16:30:33 by victor            #+#    #+#              #
#    Updated: 2024/12/17 12:58:10 by vvobis           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC := cc

CFLAGS := -Wall -Wextra

LDFLAGS := -lm -lXext -lX11

NAME := miniRT

SRCDIR := src

SRC := minirt.c

SCENEDIR := scene
SRCSCENE := $(addprefix $(SCENEDIR)/, $(addsuffix .c,\
			camera_info \
			light scene scene_create sphere sphere_calc \
			cylinder cylinder_info cylinder_calc cylinder_uv \
			disk disk_calc plane plane_calc add_body \
			cone_tracing cone_utils cone_info))

RENDERDIR := rendering
SRCRENDER :=	$(addprefix $(RENDERDIR)/, $(addsuffix .c,\
				rendering image pixel ray_utils color drawing threads \
				camera camera_ray_define color2 light_shadow phong))

IODIR := io
SRCIO :=	$(addprefix $(IODIR)/, $(addsuffix .c,\
			ppm id bump mouse_press mouse key_press keys parsing2 parsing glyph \
			item_value_manip items_create menu slider container \
			key_change_scene buffer menu_body_create menu_body_map \
			menu_value_add menu_value_map explorer explorer_helper \
			explorer_load_file menu_file_map))

UTILDIR := utils
SRCUTIL :=	$(addprefix $(UTILDIR)/, $(addsuffix .c,\
			minirt_utils vector_math2 vector_math vector \
			detect_overflow_utils detect_overflow))

LIBS := libft/libft.a memory/memory.a minilibx-linux/libmlx.a

MAP := mapgen

OBJDIR := obj
OBJ := $(SRC:%.c=$(OBJDIR)/%.o)
OBJRENDER := $(SRCRENDER:%.c=$(OBJDIR)/%.o)
OBJSCENE := $(SRCSCENE:%.c=$(OBJDIR)/%.o)
OBJIO := $(SRCIO:%.c=$(OBJDIR)/%.o)
OBJUTIL := $(SRCUTIL:%.c=$(OBJDIR)/%.o)

ifdef DEBUG
	CFLAGS += -D VALGRIND
	CFLAGS += -g3
else
	CFLAGS += -O3
endif

all: $(OBJDIR) $(NAME)

$(NAME): $(OBJ) $(LIBS) minirt.h $(OBJIO) $(OBJSCENE) $(OBJUTIL) $(OBJRENDER)
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

$(MAP): src/mapgeb/map_gen_buffer.c src/mapgeb/map.c
	make all
	$(CC) $(CFLAGS) $^ src/io/buffer.c $(LIBS) -o $@ -lm

$(LIBS):
	make -C libft
	make -C memory
	make -C libft/printf
	make -C minilibx-linux

$(OBJDIR):
	mkdir -p $(OBJDIR) $(OBJDIR)/$(SRCDIR) $(OBJDIR)/$(RENDERDIR) $(OBJDIR)/$(IODIR) $(OBJDIR)/$(UTILDIR) $(OBJDIR)/$(SCENEDIR)

clean:
	rm -rf $(OBJDIR)
	make clean -C libft
	make clean -C memory
	make clean -C libft/printf
	make clean -C minilibx-linux

fclean: clean
	rm -f $(NAME) $(MAP)
	make fclean -C libft
	make fclean -C memory
	make fclean -C libft/printf

re: fclean all

.PHONY: all clean fclean re debug