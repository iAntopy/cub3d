/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_ref_name.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 08:22:23 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/20 22:16:31 by gehebert         ###   ########.fr       */
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

int	xform_builder(t_cub *cub, char *tex_name, char *tex_path, int j)
{
	cub->box.xform[j] = mlx_load_png(tex_path);
	if (!cub->box.xform[j])
		return (-1);
	printf(":: txtr {%s} >> OK \n", tex_path);
	if (tex_name[0] == 'z')
	{
		cub->box.sky = cub->box.xform[j];
		if (!cub->box.sky)
			return (-1);
		printf(":: Z >>");
	}
	j++;
	return (j);
}

t_cub	*e_mtrx_link(t_cub *cub, t_box *box, char **raw)
{
	char	*tex_path;
	char	*tex_name;
	int		i;
	int		j;

	i = -1;
	j = 0;
	while (++i < box->xnum + box->meta)
	{
		tex_name = ft_substr(raw[i], 0, 1);
		tex_path = ft_substr(raw[i], 2, ft_strlen(raw[i]) - 2);
		if ((ft_in_set(tex_name[0], (const char *)MCHR) != -1))
			meta_builder(cub, box, tex_name, &cub->objs);
		else if (ft_in_set(tex_name[0], (const char *)LCHR) != -1)
			j = xform_builder(cub, tex_name, tex_path, j);
		else if (ft_in_set(tex_name[0], (const char *)NCHR) != -1)
			if (!dual_builder(cub, ft_in_set(tex_name[0], (const char *)NCHR),
					tex_path))
				return (NULL);
		if (j == -1 || ((ft_in_set(tex_name[0], (const char *)MCHR) == -1)))
			return (NULL);
		free(tex_name);
		free(tex_path);
	}
	return (cub);
}

t_cub	*e_mtrx_count(t_cub *cub)
{
	int			i;
	const char	*rawz;

	i = -1;
	cub->box.chrs_len = 1;
	cub->box.meta = 1;
	while (*cub->map.raw && cub->map.raw[++i])
	{
		rawz = cub->map.raw[i];
		if (rawz[1] != ' ')
			break ;
		if (rawz[0] >= 32 && rawz[0] < 97 && rawz[1] == 32)
			++cub->box.chrs_len;
		if (ft_in_set(rawz[0], (const char *)MOD_SPEC) != -1)
			++cub->box.n_plyr;
		if (ft_in_set(rawz[0], (const char *)MCHR) != -1)
			++cub->box.meta;
		if (ft_in_set(rawz[0], (const char *)UCHR) != -1)
			cub->box.pset++;
		if (ft_in_set(rawz[0], (const char *)NCHR) != -1)
			cub->box.n_dual++;
		if (ft_strchr_set(rawz, ".png") != NULL)
			++cub->box.xnum;
	}
	return (cub);
}

t_cub	*e_list_txtr(t_cub *cub, t_box *box, t_map *map)
{
	box->xnum = 0;
	box->pnum = 0;
	cub = e_mtrx_count(cub);
	box->xform = (mlx_texture_t **)calloc(sizeof(mlx_texture_t *),
			box->xnum + 1);
	if (!box->xform)
		return (NULL);
	cub->dual = (t_matrx *)calloc(sizeof(t_matrx), cub->box.n_dual + 1);
	if (!cub->dual)
		return (NULL);
	cub = e_mtrx_link(cub, box, map->raw);
	if (!cub)
		return (NULL);
	cub->box.chrs = chrs_builder(cub);
	return (cub);
}
