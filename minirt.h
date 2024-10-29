/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszilas <bszilas@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 16:34:08 by victor            #+#    #+#             */
/*   Updated: 2024/10/29 19:39:10 by bszilas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "minilibx-linux/mlx.h"
# include "minilibx-linux/mlx_int.h"
# include <unistd.h>
# include <math.h>
# include <stdlib.h>
# include <errno.h>
# include <stdio.h>
# include <fcntl.h>
# include "libft/libft.h"
# include "memory/memory.h"
# include <stdbool.h>
# include <X11/keysym.h>
# include <sys/time.h>
# include <X11/XKBlib.h>
# include <X11/extensions/XShm.h>

# ifndef WI
#  define WI 1920
# endif

# ifndef HI
#  define HI 1080
# endif

# define MAX_BODY_INIT 16
# define READ_BUFFER_SIZE 2048

# define SCENE_START_RESOLUTION_X 1
# define SCENE_START_RESOLUTION_Y 1

# define CHECKER_GRID_SIZE 16

# define THREAD_COUNT 32

# define DROPOFF_DISTANCE 5
# define SHADOW_BIAS 1e-5
# define COS_10 0.98480775301
# define SIN_10 0.17364817766
# define RAD_TO_DEG 57.2957795131

# define MAIN_MENU 0x00beef00
# define ITEM_HEIGHT 30
# define ITEM_MARGIN 5

# define CONTAINER_ITEM_COUNT 32
# define CONTAINER_TITLE_LEN 1024
# define CONTAINER_PADDING 6

# define ITEM_PADDING 2

enum	e_id_groups
{
	ID_GROUP_MENU_MAIN,
	ID_GROUP_SPHERE,
	ID_GROUP_PLANE,
	ID_GROUP_CYLINDER,
	ID_GROUP_DISC,
	ID_GROUP_CONE,
	ID_GROUP_ITEM,
};

enum	e_mouse
{
	LEFT_CLICK = 1,
	RIGHT_CLICK = 3,
	SCROLL_UP = 4,
	SCROLL_DOWN = 5,
};

enum	e_menu_bg
{
	MAIN_MENU_BG = 0x202020,
	MENU_ITEM_BG = 0x404040,
	HELP_MENU_BG = 0x0a0a0a,
};

enum e_menu_type
{
	MENU_MAIN,
	MENU_SPHERE,
	MENU_PLANE,
	MENU_CYLINDER,
};

typedef enum e_type
{
	BODY_END = 0,
	BODY_DELETED,
	BODY_SPHERE,
	BODY_PLANE,
	BODY_CYLINDER,
	BODY_DISK,
	BODY_CONE,
}	t_type;

typedef enum e_format
{
	CONTAINER_GRID,
	CONTAINER_LIST
}	t_format;

# define GLYPH_COUNT 32
# define GLYPH_ROW 7
# define GLYPH_COL 6
# define PATH_LENGTH 128

typedef struct s_rect
{
	uint	x;
	uint	y;
	uint	width;
	uint	height;
}	t_rect;

typedef struct s_item
{
	uint	id;
	uint	color;
	void	*param;
	void	(*func_ptr)(void *, void *);
	char	description[1024];
	t_rect	attr;
}	t_item;

typedef struct s_container
{
	uint			id;
	t_rect			attr;
	t_format		format;
	char			title[CONTAINER_TITLE_LEN];
	char			buffer[1024];
	uint			item_count;
	uint			item_width;
	uint			item_height;
	t_item			item[CONTAINER_ITEM_COUNT];
	struct s_data	*data;
}	t_container;

typedef struct s_mouse
{
	bool			left_is_pressed;
	bool			right_is_pressed;
	bool			is_moving;
	struct s_body	*grabbed;
	uint			color_store;
	int				prev_x;
	int				prev_y;
	struct s_data	*data;
}	t_mouse;

typedef struct s_vector
{
	double	x;
	double	y;
	double	z;
}	t_vector;

typedef struct s_pixel
{
	uint	id;
	double	dist;
	int		*color;
}	t_pixel;

typedef struct s_line
{
	char	buffer[READ_BUFFER_SIZE + 1];
	uint	length;
	uint	count;
}	t_line;

typedef struct s_sphere
{
	double		radius;
	t_vector	center;
}	t_sphere;

typedef struct s_plane
{
	t_vector	point;
	t_vector	normal;
	t_vector	inverse_normal;
}	t_plane;

typedef struct s_disk
{
	t_vector	point;
	t_vector	normal;
	t_vector	inverse_normal;
	double		radius;
}	t_disk;

typedef struct s_cylinder
{
	t_vector	center;
	t_vector	top;
	t_vector	bottom;
	t_vector	normal;
	t_vector	coeffs;
	double		radius;
	double		height;
	double		hit_h;
}	t_cylinder;

typedef struct s_cone
{
	t_vector	vertex;
	t_vector	normal;
	t_vector	coeffs;
	double		radius;
	double		height;
	double		hit_h;
	double		tan_a2p1;
	double		t[2];
	t_disk		bottom_cap;
}	t_cone;

typedef struct s_texture
{
	uint	*pixel;
	int		width;
	int		height;
}	t_texture;

typedef struct s_body
{
	int				id;
	t_type			type;
	uint			color;
	bool			reflect;
	bool			checker_board;
	bool			textured;
	t_texture		*texture;
	union
	{
		t_sphere	sphere;
		t_plane		plane;
		t_cylinder	cylinder;
		t_disk		disk;
		t_cone		cone;
	};
}	t_body;

typedef struct s_light
{
	t_vector	position;
	float		obj_distance;
	float		intensity;
	uint		color;
}	t_light;

typedef double	t_matrix4[4][4];

typedef struct s_camera
{
	t_vector	*ray;
	t_vector	position;
	t_vector	normal;
	t_vector	right;
	t_vector	up;
	t_matrix4	to_world;
	double		tilt;
	double		fov;
}	t_camera;

typedef struct s_scene
{
	uint		current_body_max;
	uint		body_cursor;
	uint		resolution_x;
	uint		resolution_y;
	t_light		light;
	t_light		ambient;
	t_camera	camera;
	t_vector	*focus;
	t_body		*focus2;
	t_pixel		*pixel;
	t_body		*body;
	t_texture	*texture;
}	t_scene;

typedef struct s_data
{
	void				*mlx;
	void				*win;
	t_img				image;
	t_pixel				*pixel;
	t_scene				scene;
	t_mouse				mouse;
	t_container			*menu;
	void				*param;
	void				(*func_ptr)(void *, void *);
}	t_data;

/* ft_atod.c */
double		ft_atod(char *n);
void		item_double_inc(void *value, void *null);
void		item_double_dec(void *value, void *null);

/* Utils */
void		ft_open(int *fd, const char *path, int flag, int permissons);
int			ft_close(int fd);
int			ft_read(int fd, char *character, unsigned int size_to_read);
void		glyph_print(uint begin_x, uint begin_y, \
						char const *text, t_pixel *pixel);
void		ft_putnbrf_fd(double f, int fd, int precision);
void		data_destroy_func(void *data_ptr);
void		data_function_pointer_set(void *data_ptr, void *func_ptr);

/* Drawing */
void		rt_draw_rect(t_rect rect, t_pixel *pixel, uint id, uint color);
void		rt_draw_rect_blend(t_rect rect, t_pixel *pixel, \
								uint id, uint color);

/* Generics */
bool		collect_param_comma(char **entry_position, \
								char *params[], uint count);
bool		collect_param_single(char **entry_position, char *params[]);
uint		id_set(uint group, int id);
uint		id_group_get(int id);
void		err(char *body_type, uint line);

/* Colors */
uint		set_color(uint r, uint g, uint b);
void		get_color_reflect(t_vector new_center, t_vector normal, \
								t_scene *scene, t_pixel *pixel);
double		get_color_attenuation(t_vector p, t_vector surface_n, \
									t_light l, t_scene *sc);
uint		get_color(uint obj, uint light, double attn);
uint		color_blend(uint	color1, uint color2);
/* !!! This function PRINTS a SPACE at the BEGINNING and a NEWLINE
character at the END !!! */
void		color_print(uint color, int fd);
uint		add_color(uint color1, uint color2);
uint		parse_body_color(char *params[], int *error);

/* Sphere */
bool		parse_sphere(char *entry, uint line_count, \
							t_body *body, uint body_count);
void		pixel_sphere_set(t_pixel *pixel, t_vector camera_ray, \
								t_body *body, t_scene *scene);
void		body_sphere_print(t_body *body);
void		sphere_save(t_sphere sphere, uint color, int fd);
double		sphere_hit_distance(t_vector ray, t_vector dlt_centr, \
								t_sphere sphere, int *flip);

/* Plane */
bool		parse_plane(char *entry, uint line_count, \
						t_body *body, uint body_count);
void		body_plane_print(t_body *body);
void		pixel_plane_set(t_pixel *pixel, t_vector camera_ray, \
							t_body *body, t_scene *scene);
void		plane_save(t_plane plane, uint color, int fd);
bool		move_plane(int keycode, t_plane *plane);

/* Dear FBI we mean the geometric kind of plane, not the flying vehicle. */
double		plane_hit_distance(t_plane pl, t_vector cam, \
								t_vector camera_ray, int *flip);

/* Cylinder */
void		body_cylinder_print(t_body *body);
void		cylinder_save(t_cylinder cylinder, uint color, int fd);
bool		parse_cylinder(char *entry, uint line_count, \
							t_body *body, uint body_count);
void		trace_cyl(t_pixel *pixel, t_vector camera_ray, \
						t_body *body, t_scene *scene);
void		calc_cyl_data(t_cylinder *cy);
/* The coefficients a, b, c in a quadratic equation f(x) = ax² + bx + c are
saved in a t_vector coeffs, so that coeffs.x = a, .y = b, .z = c */
void		cyl_equation_coefficients(t_cylinder *cy, t_vector ray, \
										t_vector cam_delta);
double		cyl_hit_distance(t_cylinder *cy, t_vector ray, \
								t_vector cam, int *flip);
double		cyl_components_shadow(t_cylinder cy, t_vector ray, t_vector p);
bool		move_cylinder(int keycode, t_cylinder *cyl);

/* Disk */
void		trace_disk(t_pixel *pixel, t_vector ray, \
						t_body *body, t_scene *scene);
void		parse_cyl_caps(t_cylinder *cy);
bool		parse_disk(char *entry, uint line_count, \
						t_body *body, uint body_count);
double		disk_hit_distance(t_disk disk, t_vector ray, \
						t_vector cam, int *flip);
void		print_disk(t_body *body);
bool		move_disk(int keycode, t_disk *disk);

/* Cone */
bool		move_cone(int keycode, t_cone *cone);
bool		parse_cone(char *entry, uint line_count, t_body *body, \
uint body_count);
void		calc_cone_data(t_cone *cn);
/* The coefficients a, b, c in a quadratic equation f(x) = ax² + bx + c are
saved in a t_vector coeffs, so that coeffs.x = a, .y = b, .z = c */
void		cone_equation_coefficients(t_cone *cn, t_vector ray, \
t_vector cam_delta);
void		solve_cone_equation(t_cone *cn, t_vector ray, t_vector cam_delta);
double		cone_hit_distance(t_cone *cn, t_vector ray, t_vector cam, \
int *flip);
void		trace_cone(t_pixel *pixel, t_vector ray, t_body *body, \
t_scene *sc);
void		trace_cone_bottom(t_pixel *px, t_vector ray, t_body *cone, \
t_scene *sc);
double		cone_components_shadow(t_cone cn, t_vector ray, t_vector p);

/* Image */
t_img		image_create(void *mlx, uint width, uint height);
void		img_pixel_put(t_img *image, unsigned int x, \
						unsigned int y, uint color);

/* Vector */
void		normalize_vector(t_vector *vec);
double		dot_product(t_vector a, t_vector b);
double		vector_length(t_vector vec);
t_vector	vector_subtract(t_vector vec1, t_vector vec2);
t_vector	set_vector(double x, double y, double z);
t_vector	scale_vector(t_vector vec, double scale);
t_vector	add_vector(t_vector vec, t_vector add);
void		vector_print(t_vector vector, int fd);
t_vector	get_normal(t_vector from, t_vector to);
t_vector	cross_product(t_vector a, t_vector b);
t_vector	rot_x(t_vector vec, int dir);
t_vector	rot_y(t_vector vec, int dir);
t_vector	rot_z(t_vector vec, int dir);

/* Camera */
t_vector	*camera_ray_create(void);
void		define_camera_rays(t_pixel *pixel, t_camera *camera, \
								t_scene *scene);
void		reposition_camera(t_vector *array, t_vector new_pos, \
t_scene *scene);
bool		key_rotate_cam(int key, t_scene *scene);
void		set_world_matrix(t_camera *camera);
/*camera->right in this function is used as cam normal projection onto the
xz plane to save one less t_vector type */
void		calc_camera_tilt(t_camera *camera);
bool		parse_camera(char *entry, uint line_count, t_camera *camera);
void		body_camera_print(t_camera camera);
void		camera_save(t_camera *camera, int fd);

/* Pixel */
void		pixels_put_to_image(t_img *image, t_pixel *pixel, uint resolution);
t_pixel		*pixel_plane_create(void);
void		set_pixel_distances(t_pixel *array, uint size, double dist);
void		pixels_clear(t_pixel *pixel);
void		set_hit_pixel(t_scene *sc, t_pixel *px, double attn, double dist);
void		pixel_clear_id(t_pixel *pixel);

/* Ray Utils */
double		ray_distance_from_point_squared(t_vector ray, t_vector point);
double		smaller_non_negative(double a, double b);

/* Image */
t_img		image_create(void *mlx_ptr, uint width, uint height);


/* Scene */
void		scene_print(t_scene *scene);
void		scene_create(const char *filepath, t_scene *scene);
char		*buffer_read_chunk(t_line *line, uint fd, bool *file_end_reached);
bool		body_determine(char *entry, uint line_count, t_scene *scene);
void		scene_body_add(t_body body, t_scene *scene);
void		scene_add_cylinder(void *data_ptr, void *null);
void		scene_add_sphere_func(void *scene_ptr, void *null);
void		scene_add_plane_func(void *data_ptr, void *null);
t_body		*body_get_by_id(int id, t_scene *scene);
void		scene_save(t_scene *scene);
void		pixels_image_syncronize(t_img *image, t_pixel *pixel);

/* Rendering */
/*t_img		image_render(void *mlx, const char *filepath);*/
/*void		render_pixel_bodys_set(t_scene *scene, uint pixel_count);*/
/*void		render_pixel_bodys_set_performance(t_scene *scene, t_pixel *pixel);*/
/*void		scene_render(t_scene *scene, t_img *image);*/
uint		rendering_loop(t_data *data);
void		pixel_fill(t_pixel *pixel, t_scene *scene);
void		ray_check_bodys(t_pixel *pixel, t_vector ray, t_scene *scene);

/* Light */
bool		parse_light(char *entry_light, uint line_count, t_light *light);
void		body_light_print(t_light light);
void		light_save(t_light light, int fd);
bool		parse_ambient(char *entry_light, uint line_count, t_light *light);

/* Keys */
int			key_press(int keycode, void *data);
uint		key_change_res(int keycode, t_scene *scene);
bool		key_move_focused(int keycode, t_vector *focus);
bool		key_change_fov(int keycode, t_camera *camera);
bool		key_rotate_cam(int key, t_scene *scene);
int			move_body(int keycode, t_body *body);
bool		key_move_light(int keycode, t_scene *scene);
uint		key_misc_function(int keycode, t_scene *scene, t_data *data);

/* Menu */
void		container_draw(void *menu, void *pixel);
t_container	main_menu_create(t_data *data);
void		container_item_get_by_id(t_container *container, uint id);
void		help_menu_draw(void *data_ptr, void *null);
t_container	container_create(const char *title, t_rect *attr, uint format);
void		container_item_add(t_container *container, t_item *item);
t_item		container_item_create(const char *title, void *param, \
									void (*func_ptr)(void *, void *));

void		disk_menu_map(t_container *menu, t_body *body, uint *color);
void		sphere_menu_map(t_container *menu, t_body *body, uint *color);
void		cylinder_menu_map(t_container *menu, t_body *body, uint *color);
void		plane_menu_map(t_container *menu, t_body *body, uint *color);
void		cone_menu_map(t_container *menu, t_body *body, uint *color);
void		menu_body_map_float(t_item *item, double *value);
void		menu_body_map_vector(t_item *item, t_vector *vector);
void		menu_body_map_color(t_item *item, uint *color);
void		menu_body_colors_add(t_container *menu);
void		menu_body_vector_position_add(t_container *menu);
void		menu_body_vector_normal_add(t_container *menu);
void		menu_body_float_add(t_container *menu, char *name_prefix);
void		cylinder_menu_create(t_container *menu);
void		cone_menu_create(t_container *menu);
void		disc_menu_create(t_container *menu);
void		sphere_menu_create(t_container *menu);
void		plane_menu_create(t_container *menu);

/* Mouse */
int			mouse_press(int button, int x, int y, t_data *data);
int			mouse_release(int button, int x, int y, t_data *data);
int			mouse_move(int x, int y, t_data *data);

/* Parsing */
char		temporary_terminate_string(char *end);
void		restore_string(char *end, char tmp);
int64_t		get_one_tenth(char *nbr);
char		get_last_char(char *str);
size_t		count_digits(int64_t n, int base);
bool		positive_overflow(int64_t max, char *str);
bool		negative_overflow(int64_t min, char *str);
bool		is_a_number(char *str);
char		*skip_whitespace(char *str);
int64_t		set_signed_int(const char *str, int64_t min, \
							int64_t max, int *error);
int			mlx_flush_event(t_xvar *xvar);

/* PPM READER */
bool		ppm_check(int fd, int *width, int *height);
void		ppm_pixels_read(int fd, t_texture *texture);
int			ppm_read_number(int fd, bool *eof);
t_texture	ppm_image_read(const char *path);

#endif
