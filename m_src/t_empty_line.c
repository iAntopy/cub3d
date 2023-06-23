/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_empty_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 19:05:30 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/12 15:34:59 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	is_empty_line(char *line)
{
	if (!line)
		return (0);
	while (*line && ft_isspace(*line))
		line++;
	return (*line == '\0');
}

void	flush_empty_lines(char **raw)
{
	size_t	len;

	while (*raw)
	{
		if (is_empty_line(*raw))
		{
			ft_free_p((void **)raw);
			len = strtab_len(raw + 1);
			ft_memmove(raw, raw + 1, sizeof(char *) * len);
			raw[len] = NULL;
		}
		else
			raw++;
	}
}
/*

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
*/