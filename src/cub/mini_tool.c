/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_tool.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 22:02:25 by gehebert          #+#    #+#             */
/*   Updated: 2023/02/28 00:08:57 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"



int	error(char *error, t_minimap *mini)
{
	int	i;

	i = -1;
	write(1, "Error\n", 6);
	write(1, error, ft_strlen(error));
	if (mini->tab == NULL)
		exit(MLX_ERROR);
	while (mini->tab[++i])
		free(mini->tab[i]);
	exit(MLX_ERROR);
}

int	int_strlen(const char *s)
{
	int	i;

	i = 0;
	if (s == NULL)
		return (0);
	while (s[i] != '\0' && s[i] != '\n')
		i++;
	return (i);
}

int	ft_strfcmp(const char	*s1, const char	*s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;
	size_t			i;
	size_t			j;

	i = ft_strlen(s1) - n;
	j = ft_strlen(s2) - n;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while ((str1[i] != '\0' || str2[j] != '\0'))
	{
		if (str1[i] != str2[j])
			return (str1[i] - str2[j]);
		i++;
		j++;
	}
	return (0);
}

// void	print_map(t_minimap *m)
    // {
    // 	int	j;
    // 	int	i;

    // 	i = -1;
    // 	while (++i < m->h / 36)
    // 	{
    // 		if (i <= (m->x / 36) + 1)
    // 			mlx_put_image_to_window(m->mlx.mlx, m->mlx.win,
    // 				m->img.player, m->y, m->x);
    // 		j = -1;
    // 		while (++j < m->w / 36)
    // 		{
    // 			if (m->map[i][j] == '1' && (i == 0 || j == 0
    // 			|| i == (m->h / 36) - 1 || j == (m->w / 36) - 1))
    // 				mlx_put_image_to_window(\
    // 				m->mlx.mlx, m->mlx.win, m->img.wall, j * 36, i * 36);
    // 			else if (m->map[i][j] == '1')
    // 				mlx_put_image_to_window(\
    // 				m->mlx.mlx, m->mlx.win, m->img.tile, j * 36, i * 36);
    // 			else
    // 				print_map_next(m, i, j);
    // 		}
    // 	}
// }

