/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 21:39:58 by gehebert          #+#    #+#             */
/*   Updated: 2023/02/28 13:17:55 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"
// map parse 


// static	void	start_set(t_minimap *map, int flag)
// {	
// 	// if (flag == 0)
// 	// {
// 		if (map->tab[map->pos_x][map->pos_y] != '0' && map->tab[map->pos_x][map->pos_y] != '1')
// 			error("6. Map contains unrecognized characters", map);
// }

t_minimap	*init_mini(t_minimap *mini)
{
	int *tmp; 	// pre_split form
	int color;	// ref_ color by dot_char	
    
	mini->pos_x = 1;		// start pos
	mini->pos_y = 1;		// start pos
	mini->width = 0;		// longest row
	mini->height = 0;	// how many row
    mini->file = NULL;
    mini->tab = NULL;
	
	return (mini);
}

// static	void	wall_check(t_minimap *m)
// {
// 	while (++c->h < m->h)
// 	{
// 		c->w = -1;
// 		while (++c->w < m->w)
// 		{
// 			if (c->h == 0 || c->h == m->h -1 || c->w == 0 || c->w == m->w -1)
// 			{
// 				if (m->map[c->h][c->w] != '1')
// 					error("4. Your map isn't surrounded by walls", m);
// 			}
// 			else
// 			{
// 				if (m->map[c->h][c->w] == 'C')
// 					m->c = ++c->c;
// 				else if (m->map[c->h][c->w] == 'E')
// 					c->e++;
// 				else if (m->map[c->h][c->w] == 'P')
// 					start_set(m, c, 1);
// 				else
// 					start_set(m, c, 0);
// 			}		
// 		}
// 	}
// 	if (c->p != 1 || c->c < 1 || c->e != 1)
// 		error("7. Your map is uncomplete, please verify", m);
// }


void	print_map(t_minimap *m)
{
	int	j;
	int	i;

	i = -1;
	while (++i < m->height / 36)
	{
		if (i <= (m->pos_x / 36) + 1) 
			
			// mlx_put_image_to_window(m->mlx.mlx, m->mlx.win,
				// m->img.player, m->y, m->x);
		j = -1;
		while (++j < m->width / 36)
		{
			if (m->tab[i][j] == '1' && (i == 0 || j == 0
			|| i == (m->height / 36) - 1 || j == (m->width / 36) - 1))
				mlx_pixel_put();
				// mlx_put_image_to_window(\
				// m->mlx.mlx, m->mlx.win, m->img.wall, j * 36, i * 36);
			else if (m->map[i][j] == '1')
				// mlx_put_image_to_window(\
				// m->mlx.mlx, m->mlx.win, m->img.tile, j * 36, i * 36);
			// else
			// 	print_map_next(m, i, j);
		}
	}
}

static	void	transcribe(t_minimap *map, char *file)
{
	int		i;
	int		fd;

	i = -1;
	fd = open(file, O_RDONLY);
	while (++i < map->height)
		map->tab[i] = get_next_line(fd);
	map->tab[i] = NULL;
	close(fd);
	// wall_check(map, game);
	// map->h = game->h * 36;
	// map->w = game->w * 36;
}

void	map_checker(t_minimap *mini, t_cub cub, char *file)
{
	int		fd;
	char	*temp;

	fd = open(file, O_DIRECTORY);
	if (fd > 0)
		error("1. You are trying to open a dir.", mini);
	fd = open(file, O_RDONLY);
	if (fd < 0 || ft_strfcmp(".cub", file, 4))
		error("2. Wrong file name or extention", mini);
	temp = get_next_line(fd);
	mini->height = 0;
	mini->width = int_strlen(temp);
	while (temp)
	{
		if (mini->width != int_strlen(temp))
			error("3. There is an error in your map, Please verify", mini);
		free(temp);
		temp = get_next_line(fd);
		mini->height++;
	}
	free(temp);
	mini->tab = (char **)malloc(sizeof(char *) * (mini->height + 1));
	close(fd);
	transcribe(mini, file);
}
