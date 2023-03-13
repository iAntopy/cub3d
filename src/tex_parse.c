/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tex_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 08:03:53 by gehebert          #+#    #+#             */
/*   Updated: 2023/03/12 23:09:36 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"
// #include "../include/textur.h"

	
/// color_to_int ...
int 	str_to_color(int r, int g, int b, int t)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

int get_t (int trgb)
{
	return((trgb >> 24) & 0xFF);
}

int get_b (int trgb)
{
	return((trgb >> 16) & 0xFF);
}

int get_g (int trgb)
{
	return((trgb >> 8) & 0xFF);
}

int get_r (int trgb)
{
	return(trgb & 0xFF);
}

int create_trgb(unsigned char t, unsigned char r, unsigned char g, unsigned char b)
{
	return (*(int *)(unsigned char [4]){b, g, r, t});
}
unsigned char get_ut(int trgb)
{
	return (((unsigned char *)&trgb)[3]);
}

unsigned char get_ur(int trgb)
{
	return (((unsigned char *)&trgb)[2]);
}
unsigned char get_ug(int trgb)
{
	return (((unsigned char *)&trgb)[1]);
}
unsigned char get_ub(int trgb)
{
	return (((unsigned char *)&trgb)[0]);
}
	

static int	setup_wall_textures(t_cub *cub)
{
	char		**src;
	mlx_texture_t	**dst;
//	int		i;

	src = cub->tex.tex_n;
	dst = cub->tex.walls;
	if (src[W_SIDE] && src[N_SIDE] && src[E_SIDE] && src[S_SIDE])
	{
		printf("All textures available :  \n- %s- %s- %s- %s\n", src[W_SIDE], src[N_SIDE], src[E_SIDE], src[S_SIDE]);
		dst[W_SIDE] = mlx_load_png(src[W_SIDE]);
		dst[N_SIDE] = mlx_load_png(src[N_SIDE]);
		dst[E_SIDE] = mlx_load_png(src[E_SIDE]);
		dst[S_SIDE] = mlx_load_png(src[S_SIDE]);
	}
	return (cub->tex.walls[W_SIDE] && cub->tex.walls[N_SIDE] 
		&& cub->tex.walls[E_SIDE] && cub->tex.walls[S_SIDE]);
}


static int	error_clear(char *err, t_map *map, char ***txtr)
{
	if (txtr)
		strtab_clear(txtr);
	return (error(err, map));
}
/// get input frm file
int	tex_parse(t_cub *cub, t_map *map, int fd)
{
	char **txtr;
	char *line;
	char	**color;
	int nb;
	int id;
	
	
	printf("DEBUG:Z. JOURNEY'S INTO TEX_PARSE\n");	
	nb = 0;
	while (nb < 6)
	{
		line = get_next_line(fd);
		while (*line == '\n' || ft_strlen(line) < 2)
		{
			map->lines_to_map++;
			free(line);
			line = get_next_line(fd);
		}
		if (line)
		{			
			map->lines_to_map++;
			txtr = ft_split(line, ' ');
			if (txtr[1][ft_strlen(txtr[1])-1] == '\n')
				txtr[1][ft_strlen(txtr[1])-1] = '\0';
			if (ft_strlen(txtr[0]) > 2)	
				return (error_clear("7, Texture mapping Name error !\n", map, &txtr));
			if (!txtr[1])	
				return (error_clear("8, Texture mapping Path error !\n", map, &txtr));
			id = 0;
			id = ft_in_set((const char *)txtr[0], (const char *)"WNESCF");
			if ( id < 0)
				return (error_clear("9, Texture Name unmatching error !\n", map, &txtr));
			else if (id < 4)
			{
				// cub->tex.tex_n[id] = txtr[1];//
				if (id == 0)
					cub->tex.walls[W_SIDE] = mlx_load_png(txtr[1]);
				else if (id == 1)
					cub->tex.walls[N_SIDE] = mlx_load_png(txtr[1]);
				else if (id == 2)
					cub->tex.walls[E_SIDE] = mlx_load_png(txtr[1]);
				else if (id == 3)
					cub->tex.walls[S_SIDE] = mlx_load_png(txtr[1]);
				
				printf("DEBUG:  tex_id: %d :: tex_name: %s :: \n", id, cub->tex.tex_n[id]); 
			}
			else if (id < 6)
			{
				if (*txtr[1] < '0' || *txtr[1] > '9')
				{
					 if (id == 4)
						cub->tex.floor = mlx_load_png(txtr[1]);
					 if (id == 5)
						cub->tex.skymap = mlx_load_png(txtr[1]);
				}
				else
				{
					color = ft_split(txtr[1], ',');
					printf("DEBUG:  ID: %d :: color_num[0]: R = %s :: \n", id, color[0]); 
					printf("DEBUG:  ID: %d :: color_num[1]: G = %s :: \n", id, color[1]); 
					printf("DEBUG:  ID: %d :: color_num[2]: B = %s :: \n", id, color[2]); 
					if (id == 4)
						cub->tex.color[0] = str_to_color(ft_atoi(color[0]), ft_atoi(color[1]),
							ft_atoi(color[2]), 0xff);
					else if (id == 5)
						cub->tex.color[1] = str_to_color(ft_atoi(color[0]), ft_atoi(color[1]),
							ft_atoi(color[2]), 0xff);
				}
						// cub->tex.color[1] = str_to_color(color[0], color[1],color[2]);
						// cub->tex.color[1] = str_to_color(cub->tex.rgbx);
						// cub->tex.color[1] = str_to_color(ft_split(txtr[1], ','),1);
			}
		}
		else 
		{
			printf("DEBG: UNABLE tex_parse!");
		}
		free(line);	
		nb++;
	}
	printf("DEBUG:Z. JOURNEY'S end TEX_PARSE\n");	
	return (setup_wall_textures(cub));
//	return (0);
}
