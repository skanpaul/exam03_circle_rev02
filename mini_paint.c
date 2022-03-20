/* ************************************************************************** */
#define NO_ERROR	0
#define ERROR		1

/* ************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

/* ************************************************************************** */
typedef struct s_zone
{
	int		width;
	int		height;
	int		total;
	char	back;
}	t_zone;
/* ************************************************************************** */
typedef struct s_circle
{
	char	type;
	float	ctr_x;
	float	ctr_y;
	float	radius;
	char	char_drawn;
}	t_circle;
/* ************************************************************************** */
int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}
/* ************************************************************************** */
char *get_zone(t_zone *z, FILE *ptr_file)
{
	char *map;
	
	
	// lire la 1ere ligne	
	if ((fscanf(ptr_file, "%d %d %c\n", &z->width, &z->height, &z->back)) != 3)
		return (NULL);

	// checker les limite
	if ( z->width <= 0 || z->width > 300 || z->height <= 0 || z->height > 300)
		return (NULL);

	// calculer le total
	z->total = z->width * z->height;

	// malloquer la map
	if ((map = (char *)malloc(z->total * sizeof(char))) == NULL)
		return (NULL);

	// remplire le backround de la map
	memset(map, z->back, z->total);

	return (map);
}

/* ************************************************************************** */
void ft_free_null(char **map)
{
	if (*map)
		free(*map);
	*map = NULL;
}

/* ************************************************************************** */
bool is_on_edge(int x, int y, t_circle *c)
{
	float distance;
	float delta_x;
	float delta_y;

	delta_x = (float)x - c->ctr_x;
	delta_y = (float)y - c->ctr_y;
	
	distance = sqrtf(powf(delta_x, 2) + powf(delta_y, 2));

	if (c->radius - distance < 1)
		return (true);

	return (false);
}


/* ************************************************************************** */
bool is_in_disc(int x, int y, t_circle *c)
{
	float distance;
	float delta_x;
	float delta_y;

	delta_x = (float)x - c->ctr_x;
	delta_y = (float)y - c->ctr_y;
	
	distance = sqrtf(powf(delta_x, 2) + powf(delta_y, 2));

	if (distance <= c->radius)
		return (true);

	return (false);
}


/* ************************************************************************** */
void draw_circle(char *map, t_circle *c, t_zone *z)
{
	int x;
	int y;

	y = 0;
	while (y < z->height)
	{
		x = 0;
		while (x < z->width)
		{
			if (is_in_disc(x, y, c))
			{
				// doit dessinner le cercle uniquement
				if ((c->type == 'c') && (is_on_edge(x, y, c)))
					map[x + y * z->width] = c->char_drawn;
				// doit dessiner le disque uniquement
				if (c->type == 'C')
					map[x + y * z->width] = c->char_drawn;
			}
			x++;
		}
		y++;
	}
}

/* ************************************************************************** */
char *fill_map(char *map, t_zone *z, FILE *ptr_file, t_circle *c)
{

	int ret;

	while (1)
	{
		// read commande
		ret = fscanf(ptr_file, "%c %f %f %f %c\n", &c->type, &c->ctr_x, &c->ctr_y, &c->radius, &c->char_drawn);
		if (ret == EOF)
			break ;

		if(ret != 5)
			return (NULL);

		// check commande
		if (!(c->type == 'c' || c->type == 'C'))
			return (NULL);

		// draw circle
		draw_circle(map, c, z);

	}

	return (map);
}
/* ************************************************************************** */
void print_map(char *map, t_zone *z)
{
	int y;

	y = 0;
	while (y < z->height)
	{
		write(1, &map[y * z->width], z->width);
		write(1, "\n", 1);
		y++;
	}
}

/* ************************************************************************** */
/* ************************************************************************** */
/* ************************************************************************** */
int main(int argc, char **argv)
{
	FILE *ptr_file;

	t_zone		z;
	t_circle	c;
	char		*map;

	map = NULL;
	// -----------------------------------------------------------
	if (argc == 2)
	{
		//open file
		if ((ptr_file = fopen(argv[1], "r")) == NULL)
		{
			write(1, "Error: Operation file corrupted\n", ft_strlen("Error: Operation file corrupted\n"));
			return (ERROR);
		}

		//get_zone
		if ((map = get_zone(&z, ptr_file)) == NULL)
		{
			ft_free_null(&map);
			fclose(ptr_file);
			return (ERROR);
		}

		//fill map
		if ((fill_map(map, &z, ptr_file, &c)) == NULL)
		{
			ft_free_null(&map);
			fclose(ptr_file);
			return (ERROR);
		}

		//print map on terminal
		print_map(map, &z);
	}
	else
	{
		write(1, "Error: argument\n", ft_strlen("Error: argument\n"));
	}

	ft_free_null(&map);
	fclose(ptr_file);
	return (NO_ERROR);
}




/* ************************************************************************** */