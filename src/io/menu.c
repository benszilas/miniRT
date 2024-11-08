/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 23:58:26 by victor            #+#    #+#             */
/*   Updated: 2024/11/08 06:08:04 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minirt.h"

void	item_normal_inc(void *null, void *value)
{
	(void)null;
	*(double *)value += .1;
	if (*(double *)value > 1)
		*(double *)value = 1;
}

void	item_normal_dec(void *null, void *value)
{
	(void)null;
	*(double *)value -= .1;
	if (*(double *)value < -1)
		*(double *)value = -1;
}

void	item_double_inc(void *null, void *value)
{
	(void)null;
	*(double *)value += *(double *)value / 10;
}

void	item_double_dec(void *null, void *value)
{
	(void)null;
	*(double *)value -= *(double *)value / 10;
}

void	color_dec(void *null, void *color)
{
	(void)null;
	if (*(uint8_t *)color > 15)
		*(uint8_t *)color -= 15;
	else
		*(uint8_t *)color = 0;
}

void	color_inc(void *null, void *color)
{
	(void)null;
	if (*(uint8_t *)color < 255 - 16)
		*(uint8_t *)color += 15;
	else
		*(uint8_t *)color = 255;
}

void	item_bool_toggle(void *null, void *value)
{
	(void)null;
	*(bool *)value ^= 1;
}

void	menu_body_map_color(t_item *item, uint *color)
{
	item[0].param = (uint8_t *)color + 2;
	item[1].param = (uint8_t *)color + 2;
	item[2].param = (uint8_t *)color + 1;
	item[3].param = (uint8_t *)color + 1;
	item[4].param = (uint8_t *)color + 0;
	item[5].param = (uint8_t *)color + 0;
}

void	menu_body_map_vector(t_item *item, t_vector *vector)
{
	item[0].param = &vector->x;
	item[1].param = &vector->x;
	item[2].param = &vector->y;
	item[3].param = &vector->y;
	item[4].param = &vector->z;
	item[5].param = &vector->z;
}

void	menu_body_map_float(t_item *item, double *value)
{
	item[0].param = value;
	item[1].param = value;
}

void	menu_body_map_bool_toggle(t_item *item, bool *value)
{
	item[0].param = value;
}

void	sphere_menu_map(t_container *menu, t_body *body, uint *color)
{
	menu_body_map_vector(&menu->item[0], &body->sphere.center);
	menu_body_map_float(&menu->item[6], &body->sphere.radius);
	menu_body_map_color(&menu->item[8], color);
	menu_body_map_bool_toggle(&menu->item[14], &body->reflect);
	menu_body_map_bool_toggle(&menu->item[15], &body->checker_board);
	menu_body_map_bool_toggle(&menu->item[16], &body->textured);
}

void	disk_menu_map(t_container *menu, t_body *body, uint *color)
{
	menu_body_map_vector(&menu->item[0], &body->disk.point);
	menu_body_map_vector(&menu->item[6], &body->disk.normal);
	menu_body_map_float(&menu->item[12], &body->disk.radius);
	menu_body_map_color(&menu->item[14], color);
	menu_body_map_bool_toggle(&menu->item[20], &body->reflect);
}

void	plane_menu_map(t_container *menu, t_body *body, uint *color)
{
	menu_body_map_vector(&menu->item[0], &body->plane.point);
	menu_body_map_vector(&menu->item[6], &body->plane.normal);
	menu_body_map_color(&menu->item[12], color);
	menu_body_map_bool_toggle(&menu->item[18], &body->reflect);
	menu_body_map_bool_toggle(&menu->item[19], &body->checker_board);
	menu_body_map_bool_toggle(&menu->item[20], &body->textured);
}

void	cone_menu_map(t_container *menu, t_body *body, uint *color)
{
	menu_body_map_vector(&menu->item[0], &body->cone.vertex);
	menu_body_map_vector(&menu->item[6], &body->cone.normal);
	menu_body_map_float(&menu->item[12], &body->cone.radius);
	menu_body_map_float(&menu->item[14], &body->cone.height);
	menu_body_map_color(&menu->item[16], color);
	menu_body_map_bool_toggle(&menu->item[22], &body->reflect);
}

void	cylinder_menu_map(t_container *menu, t_body *body, uint *color)
{
	menu_body_map_vector(&menu->item[0], &body->cylinder.center);
	menu_body_map_vector(&menu->item[6], &body->cylinder.normal);
	menu_body_map_float(&menu->item[12], &body->cylinder.radius);
	menu_body_map_float(&menu->item[14], &body->cylinder.height);
	menu_body_map_color(&menu->item[16], color);
	menu_body_map_bool_toggle(&menu->item[22], &body->reflect);
	menu_body_map_bool_toggle(&menu->item[23], &body->checker_board);
}

void	menu_body_colors_add(t_container *menu)
{
	t_item	item;

	item = container_item_create("red++", NULL, color_inc);
	container_item_add(menu, &item);
	item = container_item_create("red--", NULL, color_dec);
	container_item_add(menu, &item);
	item = container_item_create("green++", NULL, color_inc);
	container_item_add(menu, &item);
	item = container_item_create("green--", NULL, color_dec);
	container_item_add(menu, &item);
	item = container_item_create("blue++", NULL, color_inc);
	container_item_add(menu, &item);
	item = container_item_create("blue--", NULL, color_dec);
	container_item_add(menu, &item);
}

void	menu_body_vector_position_add(t_container *menu)
{
	t_item	item;

	item = container_item_create("xpos++", NULL, item_double_inc);
	container_item_add(menu, &item);
	item = container_item_create("xpos--", NULL, item_double_dec);
	container_item_add(menu, &item);
	item = container_item_create("ypos++", NULL, item_double_inc);
	container_item_add(menu, &item);
	item = container_item_create("ypos--", NULL, item_double_dec);
	container_item_add(menu, &item);
	item = container_item_create("zpos++", NULL, item_double_inc);
	container_item_add(menu, &item);
	item = container_item_create("zpos--", NULL, item_double_dec);
	container_item_add(menu, &item);
}

void	menu_body_vector_normal_add(t_container *menu)
{
	t_item	item;

	item = container_item_create("xnorm++", NULL, item_normal_inc);
	container_item_add(menu, &item);
	item = container_item_create("xnorm--", NULL, item_normal_dec);
	container_item_add(menu, &item);
	item = container_item_create("ynorm++", NULL, item_normal_inc);
	container_item_add(menu, &item);
	item = container_item_create("ynorm--", NULL, item_normal_dec);
	container_item_add(menu, &item);
	item = container_item_create("znorm++", NULL, item_normal_inc);
	container_item_add(menu, &item);
	item = container_item_create("znorm--", NULL, item_normal_dec);
	container_item_add(menu, &item);
}

void	menu_body_float_add(t_container *menu, char *name_prefix)
{
	t_item	item;
	char	name[255];

	ft_strlcpy(name, name_prefix, ft_strlen(name_prefix) + 2);
	ft_strlcat(name, "++", ft_strlen(name_prefix) + 3);
	item = container_item_create(name, NULL, item_double_inc);
	container_item_add(menu, &item);
	ft_strlcpy(name, name_prefix, ft_strlen(name_prefix) + 2);
	ft_strlcat(name, "--", ft_strlen(name_prefix) + 3);
	item = container_item_create(name, NULL, item_double_dec);
	container_item_add(menu, &item);
}

void	menu_body_bool_add(t_container *menu, char *name)
{
	t_item	item;

	item = container_item_create(name, NULL, item_bool_toggle);
	container_item_add(menu, &item);
}

void	cylinder_menu_create(t_container *menu)
{
	*menu = container_create("cylinder", NULL, CONTAINER_GRID);
	menu_body_vector_position_add(menu);
	menu_body_vector_normal_add(menu);
	menu_body_float_add(menu, "r");
	menu_body_float_add(menu, "h");
	menu_body_colors_add(menu);
	menu_body_bool_add(menu, "reflect");
	menu_body_bool_add(menu, "checker");
}

void	cone_menu_create(t_container *menu)
{
	*menu = container_create("cone", NULL, CONTAINER_GRID);
	menu_body_vector_position_add(menu);
	menu_body_vector_normal_add(menu);
	menu_body_float_add(menu, "h");
	menu_body_float_add(menu, "r");
	menu_body_colors_add(menu);
	menu_body_bool_add(menu, "reflect");
}

void	disc_menu_create(t_container *menu)
{
	*menu = container_create("disc", NULL, CONTAINER_GRID);
	menu_body_vector_position_add(menu);
	menu_body_vector_normal_add(menu);
	menu_body_float_add(menu, "r");
	menu_body_colors_add(menu);
	menu_body_bool_add(menu, "reflect");
}

void	sphere_menu_create(t_container *menu)
{
	*menu = container_create("sphere", NULL, CONTAINER_GRID);
	menu_body_vector_position_add(menu);
	menu_body_float_add(menu, "r");
	menu_body_colors_add(menu);
	menu_body_bool_add(menu, "reflect");
	menu_body_bool_add(menu, "checker");
	menu_body_bool_add(menu, "texture");
}

void	plane_menu_create(t_container *menu)
{
	*menu = container_create("plane", NULL, CONTAINER_GRID);
	menu_body_vector_position_add(menu);
	menu_body_vector_normal_add(menu);
	menu_body_colors_add(menu);
	menu_body_bool_add(menu, "reflect");
	menu_body_bool_add(menu, "checker");
	menu_body_bool_add(menu, "texture");
}

void	help_menu_draw(void *data_ptr, void *pixel)
{
	t_rect	background;
	t_data	*data;

	background = (t_rect){.x = 5, .y = 5, .width = 400, .height = 220, };
	(void)pixel;
	data = data_ptr;
	rt_draw_rect_blend(background, data->pixel, 0, HELP_MENU_BG);
	glyph_print(10, 10, "navigation:\n\nrmouse: select object\n" \
						"r mouse on empty space: deselect\n" \
						"wasdqe: move camera or selected object\n" \
						"arrow keys: rotate camera or object\n" \
						"l mouse: drag camera or object\n\n" \
						"iohjkl: move spot light\n" \
						"n: select next light\n\n" \
						"r: resolution++ or anti-aliasing on\nt: resolution-- or anti-aliasing off\n\n" \
						"g: object glossiness on /off\n" \
						"b: sky sphere on/off\n\n" \
						"esc: exit\n", data->pixel);
	data->func_ptr = NULL;
}
