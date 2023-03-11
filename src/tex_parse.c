/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tex_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 08:03:53 by gehebert          #+#    #+#             */
/*   Updated: 2023/03/10 19:00:59 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
// #include "../include/textur.h"

	//      // tex_pars
			// arg[0] = tex_name char* { "W","N","E","S","F","C"} (upper || lower)
			// arg[1] = char_case: tex_path  (*char must begin by "./")
			// arg[1] = num_case : color (get RGB bit_shift << R[255],G[255],B[255])
			// rules        : chk one or more empty_line_separator
			//              : orderless mapping info -> attr name/value


	/// name_chk :: ft_strfcmp(arg[x], tex_name, tex_name_len);

	//        /// path_chk :: (start: ".\" , end: != "\") 
// static char	*path_from_line(int start, char const *line)
	// 	{
	// 			int		start_def;
	// 			int		end;
	// 			char	*path;

	// 			start_def = start;
	// 			if (!line)
	// 				return (NULL);
	// 			while (line[start] && line[start] == ' ')
	// 				start++;
	// 			end = ft_strlen(line);
	// 			while (line[end] == ' ')
	// 				end--;
	// 			if (start == start_def || end - start <= 0
	// 				|| !(path = ft_substr(line, start, end - start)))
	// 				return (NULL);
	// 			return (path);
		
				// int	i;
				// int	color;
				// int	tmp;
// }

/// color_to_int ...

 					// int	i;
 					// int	color;
					// int	tmp;

				// i = 0;
				// color = 0;
				// while (str)
				// {
				// 	// tmp = ft_atoi(str->content);
				// 	if (tmp < 0 || tmp > 255)
				// 		return (-1);
				// 	color = color | (tmp << (16 - (i++ * 8)));
				// 	str = str->next;
				// }
// }
					// i = 0;
					// color = 0;
					// while (str)
					// {
					// 	tmp = ft_atoi(str->content);
					// 	if (tmp < 0 || tmp > 255)
					// 		return (-1);
					// 	color = color | (tmp << (16 - (i++ * 8)));
					// 	str = str->next;
// 					// }



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
	/// should get file info about tex_color (sky/floor)
// int 	parse_color(t_config *config, int key, char const *line)
		// {
		// 	int			i;
		// 	unsigned	color;
		// 	t_str		*str[2];

		// 	i = 1;
		// 	while (line[i])
		// 		if (!ft_in_set(line[i++], " ,0123456789"))
		// 			return (0);
		// 	str[0] = NULL;
		// 	str[1] = NULL;
		// 	if (!(str[0] = ft_split(line, ' ')) || str_length(str[0]) != 2
		// 		|| !(str[1] = ft_split(str[0]->next->content, ','))
		// 		|| str_length(str[1]) != 3)
		// 		return (str_clear(&str[0]) || str_clear(&str[1]));
		// 	if ((int)((color = str_to_color(str[1]))) < 0)
		// 		return (str_clear(&str[0]) || str_clear(&str[1]));
		// 	config->c[(key == C_F) ? TEX_FLOOR : TEX_SKY] = color;
		// 	return ((str_clear(&str[0]) || str_clear(&str[1])) | 1);
// }
		//chk if arg = 12
		// txtr_name + path * 4 = 8
		// txtr_color_name + color_id * 2 = 4

	/// for each: chk_name ref AND chk_path
		//		if (yes && yes) attib name to path ... 
		//		also if (name is color_name) : str_to_color


static int	setup_wall_textures(t_cub *cub)
{
	char		**src;
	mlx_texture_t	**dst;
	int		i;

	src = cub->tex.tex_n;
	dst = cub->tex.walls;
	if (src[W_SIDE] && src[N_SIDE] && src[E_SIDE] && src[S_SIDE])
	{
		printf("All textures available :  \n- %s- %s- %s- %s\n", src[W_SIDE], src[N_SIDE], src[E_SIDE], src[S_SIDE]);
		i = -1;
		while (++i < 4)
		{
			if (ft_strnstr(src[i], ".png", PATH_MAX))
			{
				dst[i] = mlx_load_png(src[i]);
				printf("Tried to load png file %s : ptr : %p\n", src[i], cub->tex.walls);
			}
			else if (ft_strnstr(src[i], ".xpm", PATH_MAX))
			{
				printf("Before try to load xpm42 file %s : ptr : %p\n", src[i], cub->tex.xwalls[i]);
				cub->tex.xwalls[i] = mlx_load_xpm42(src[i]);
				printf("Tried to load xpm42 file %s : ptr : %p\n", src[i], cub->tex.xwalls[i]);
				dst[i] = &cub->tex.xwalls[i]->texture;
			}
		}
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
				cub->tex.tex_n[id] = txtr[1];//
				printf("DEBUG:  tex_id: %d :: tex_name: %s :: \n", id, cub->tex.tex_n[id]); 
			}
			else if (id < 6)
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
