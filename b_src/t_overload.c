/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_overload.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 19:37:30 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/21 22:01:29 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	*chrs_builder(t_cub *cub)
{
	int		i;
	int		j;
	char	**rawz;

	j = 0;
	i = 0;
	rawz = cub->map.raw;
	cub->box.chrs = (char *)malloc(sizeof(char) * cub->box.chrs_len + 2);
	while (*cub->map.raw && cub->map.raw[i] && j < cub->box.chrs_len)
	{
		if (rawz[i][0] > 32 && rawz[i][0] < 97 && rawz[i][1] == 32)
		{
			cub->box.chrs[j] = rawz[i][0];
			++j;
		}
		i++;
	}
	cub->box.chrs[j++] = '@';
	cub->box.chrs[j] = '\0';
	printf("\n-- -- --NEW CHRS {%s} len[%d]-- -- \n", cub->box.chrs, j);
	return (cub->box.chrs);
}

void	for_free(char **test)
{
	int	i;

	i = -1;
	while (++i < 8)
	{
		if (test[i])
			free(test[i]);
		test[i] = NULL;
	}
	free(test);
	test = NULL;
}

void	solo_free(char *test)
{
	if (test)
		free(test);
	test = NULL;
}
