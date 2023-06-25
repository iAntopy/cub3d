/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tex_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 08:03:53 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/24 21:37:02 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	flush_split_color(char ***split_color)
{
	printf("Flush [error_color] \n");//{::%s::} ERROR__ \n",  **split_color);
	strtab_clear(split_color);
}

static int	error_clr(char *err, t_map *map)
{
	if (map->raw)
		strtab_clear(&map->raw);
	if (map->txtr)
		strtab_clear(&map->txtr);
	printf(" Error:Code[%s]\n", err);
	if (err && err[0])
		return (error(err, map));
	return (-1);
}

static int	error_color(t_map *map, char ***split_color)
{
		flush_split_color(split_color);	// strtab_clear(split_color);
		return (error_clr("Failed to parse: 'error_color'.", map));
}


static int	colors_are_all_digits(char **split_col)
{
		const char	*s;

		while (split_col)
		{
			if (!*split_col)
				break;
			s = *split_col - 1;
			while (*(++s))
			{
				if (!ft_isdigit(*s))
				{
					printf(" END AT ZERO\n");
					return (0);
				}
			}
			printf(" \tlast checked  ::{__%s__}:: \n", *split_col);
			split_col++;
		}
		printf(" END AT ONE\n");
		return (1);
}


static int	color_split(t_map *map, char *col_str, int *ret_col)
{
	char		**color;
	int			rgb[3];

	// *ret_col = 0;
	color = ft_split_set(col_str + 1, ", ");
	if (strtab_len(color) != 3 || !colors_are_all_digits(color) || *ret_col != 0)
	{
		strtab_clear(&color);
		return (error_color(map, &color));
	}
	rgb[0] = ft_atoi(color[0]);
	rgb[1] = ft_atoi(color[1]);
	rgb[2] = ft_atoi(color[2]);
	printf(" rgb << Color_split [__%d,%d,%d__] \n", rgb[0],rgb[1],rgb[2]);
	if (strtab_len(color) != 3 || (rgb[0] < 0 || rgb[0] > 255)
		|| (rgb[1] < 0 || rgb[1] > 255) || (rgb[2] < 0 || rgb[2] > 255))
		return (error_color(map, &color));
	*ret_col = str_to_color(rgb[0], rgb[1], rgb[2], 0xff);
	strtab_clear(&color);
	return (*ret_col);
}

t_cub	*get_tex_by_id(t_cub *cub, int id, char *tex_str)
{
	char	*t;

	if (id < 0 || id > 3)
		return (NULL);
	if (!cub->tex.walls[id])
	{
		while (*(++tex_str) && ft_isspace(*tex_str))
			continue ;
		t = tex_str;
		while (*tex_str && !ft_isspace(*tex_str))
			tex_str++ ;
		*tex_str = '\0';
		cub->tex.walls[id] = mlx_load_png(t);
		if (!cub->tex.walls[id])
			return (report_mlx_tex_load_failed(t));
		cub->tex_id++;
	}
	else
	{
		ft_eprintf("Error\n\t- Trying to load texture id %d twice.\n", id);
		return (NULL);
	}	
	return (cub);
}



int	tex_parse(t_cub *cub, t_map *map)
{
	int		nb;
	int		id;

	nb = 0;
	while (nb < 6 && map->raw[nb])
	{
		id = ft_in_set(map->raw[nb][0], (const char *)"WNESCF");
		printf("id for tag %c : %d\n", map->raw[nb][0], id);
			// printf(" T_parse: ID[%d]:: Color_split >> Ret.{__%d__}  <<\n", nd, clr_chk);
 			// printf(" T_parse: Raw[%d]::{_%s_}  << ID:%d >>\n", nb, map->raw[nb], id);//, cub->tex.color + (id - 4));
		if (id < 0 || map->raw[nb][1] != ' ')
			return (error_clr("Invalid config label found!\n", map));
		else if (id < 4 && !get_tex_by_id(cub, id, map->raw[nb]))
			return (error_clr("Invalid Texture found!\n", map));
		else if (id == 4 || id == 5)
			if (!color_split(map,map->raw[nb], cub->tex.color + (id - 4)))
				return (error_clr("Missing Parsing textures.", map)); // return (-1);
		nb++;
	}
	if (cub->tex_id != 3)
		return (error_clr("Missing textures\n", map));
	return (0);
}


	
// static int	error_color(t_map *map, char ***split_color)
	// {
	// 	printf("Call-Flush [error_color] \n");
	// 	flush_split_color(split_color);	// strtab_clear(split_color);
	// 		// printf("[error_color] {::%s::} ERROR__ \n",  **split_color);
	// 		// return (error("Failed to parse a color argument from file", map));
	// 	return (error_clr("Failed to parse: 'error_color'.", map));
	// }
	


// static int	colors_are_all_digits(char **split_col)
	// {
	// 	const char	*s;

	// 	while (split_col)
	// 	{
	// 		s = *split_col - 1;
	// 		while (*(++s))
	// 			if (!ft_isdigit(*s))
	// 				return (0);
	// 		split_col++;
	// 	}
	// 	return (1);
	// }
// static int	color_split(t_map *map, char *col_str, int *ret_col)
	// {
		// 	char		**color;
		// 	int			rgb[3];

		// 	*ret_col = 0;
		// 	color = ft_split_set(col_str + 1, ", ");
		// 	if (strtab_len(color) != 3)
		// 		return (error_color(map, &color));
		// 	if( !colors_are_all_digits(color))
		// 		return (error_color(map, &color));
		// 	rgb[0] = ft_atoi(color[0]);
		// 	rgb[1] = ft_atoi(color[1]);
		// 	rgb[2] = ft_atoi(color[2]);
		// 	printf(" rgb << Color_split [__%d,%d,%d__] \n", rgb[0],rgb[1],rgb[2]);
		// 	if (strtab_len(color) != 3 || (rgb[0] < 0 || rgb[0] > 255)
		// 		|| (rgb[1] < 0 || rgb[1] > 255) || (rgb[2] < 0 || rgb[2] > 255))
		// 		return (error_color(map, &color));
		// 	*ret_col = str_to_color(rgb[0], rgb[1], rgb[2], 0xff);
		// 	flush_split_color(&color);	// strtab_clear(&color);
	// 	return (*ret_col);
	// }
	// 	// printf(" POST ft_atoi  Color_split {__%s__} \n", *color);
	// 	// printf(" Ret. << Color_split {__%d__} \n", *ret_col);

// t_cub	*get_tex_by_id(t_cub *cub, int id, char *tex_str)
	// {
		// 	char	*t;

		// 	if (id < 0 || id > 3)
		// 		return (NULL);
		// 	if (!cub->tex.walls[id])
		// 	{
		// 		while (*(++tex_str) && ft_isspace(*tex_str))
		// 			continue ;
		// 		t = tex_str;
		// 		while (*tex_str && !ft_isspace(*tex_str))
		// 			tex_str++ ;
		// 		*tex_str = '\0';
		// 		cub->tex.walls[id] = mlx_load_png(t);
		// 		if (!cub->tex.walls[id])
		// 			return (report_mlx_tex_load_failed(t));
		// 		cub->tex_id++;
		// 	}
		// 	else
		// 	{
		// 		ft_eprintf("Error\n\t- Trying to load texture id %d twice.\n", id);
		// 		return (NULL);
		// 	}	
	// 	return (cub);
	// }
// int	tex_parse(t_cub *cub, t_map *map)
	// {
		// 	int		nb;
		// 	int		id;
		// 	int clr_chk;

		// 	nb = 0;
		// 	clr_chk = 0;
		// 	while (nb < 6 && map->raw[nb])
		// 	{
		// 		id = ft_in_set(map->raw[nb][0], (const char *)"WNESCF");
		// 		printf("id for tag %c : %d\n", map->raw[nb][0], id);
		// 		if (id < 0 || map->raw[nb][1] != ' ')
		// 			return (error_clr("Invalid config label found!\n", map));
		// 		else if (id < 4 && !get_tex_by_id(cub, id, map->raw[nb]))
		// 			return (error_clr(map->raw[nb], map));
		// 		else if (id == 4 || id == 5)
		// 		{
		// 			clr_chk = color_split(map, map->raw[nb], cub->tex.color + (id - 4));
		// 			printf(" T_parse: ID[%d]:: Color_split >> Ret.{__%d__}  <<\n", id, clr_chk);
		// 			printf(" T_parse: Raw[%d]::{_%s_}  << >>\n", nb, map->raw[nb]);//, cub->tex.color + (id - 4));
					
		// 			if ( clr_chk < 0)
		// 				return (-1);	
		// 		}			
		// 		nb++;
		// 	}
		// 	if (cub->tex_id != 3)
		// 		return (error_clr("Missing textures. 
		// 			At least one wall texture was not loaded\n", map));
	// 	return (0);
	// }
