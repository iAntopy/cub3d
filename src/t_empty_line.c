/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_empty_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 19:05:30 by gehebert          #+#    #+#             */
/*   Updated: 2023/04/16 20:28:24 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	is_empty_line(char *line)
{
	if (!line)
		return (0);
	while (*line && ft_isspace(*line))
		line++;
	return (*line == '\0');
}

char	*skip_file_lines(t_map *map, int fd, int nb_lines)
{
	char	*tmp;
	int		i;

	i = 0;
	tmp = get_next_line(fd);
	printf("line to map : %d, nb_lines : %d\n", map->lines_to_map, nb_lines);
	while ((!nb_lines && is_empty_line(tmp)) || (tmp && i < nb_lines))
	{
		printf("i : %d, skippy!\n", i);
		map->lines_to_map++;
		i++;
		ft_free_p((void **)&tmp);
		tmp = get_next_line(fd);
	}
	return (tmp);
}
