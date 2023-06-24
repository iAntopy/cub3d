/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_empty_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 19:05:30 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/23 16:36:25 by iamongeo         ###   ########.fr       */
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
