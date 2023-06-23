/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_ref_name.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghebert <ghebert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 08:22:23 by gehebert          #+#    #+#             */
/*   Updated: 2023/06/22 13:54:58 by ghebert          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "cub3d.h"

int	chrs_checker(t_cub *cub, int j, char *tex_name, char *tex_path)
{
	int		chk;
	t_box	*box;

	box = &cub->box;
	chk = 0;
	if ((ft_in_set(tex_name[0], (const char *)MCHR) != -1))
	{
		if (!meta_builder(cub, box, tex_name, &cub->objs))
			chk = -1;
	}
	else if (ft_in_set(tex_name[0], (const char *)LCHR) != -1)
		j = xform_builder(cub, tex_name, tex_path, j);
	else if (ft_in_set(tex_name[0], (const char *)NCHR) != -1)
		if (!dual_builder(cub, ft_in_set(tex_name[0],
					(const char *)NCHR), tex_path))
			chk = -1;
	if (chk == 0)
		chk = j;
	printf(":: txtr {%s} >> OK [[chk = %d]]\n", tex_path, chk);
	return (chk);
}

int	xform_builder(t_cub *cub, char *tex_name, char *tex_path, int j)
{
	cub->box.xform[j] = mlx_load_png(tex_path);
	if (!cub->box.xform[j])
		return (-1);
	if (tex_name[0] == 'z')
	{
		cub->box.sky = cub->box.xform[j];
		if (!cub->box.sky)
		{
			printf("Exit without sky\n");
			return (-1);
		}
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
	int		flg;

	flg = 0;
	i = -1;
	j = 0;
	while (++i < box->xnum + box->meta)
	{
		tex_name = ft_substr(raw[i], 0, 1);
		tex_path = ft_substr(raw[i], 2, ft_strlen(raw[i]) - 2);
		j = chrs_checker(cub, j, tex_name, tex_path);
		free(tex_name);
		free(tex_path);
		if (j == -1)
		{
			printf("___j = %d\n", j);
			return (NULL);
		}
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
	if (!cub || cub->box.pset == 0)
		return (NULL);
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
	if (!cub->box.chrs)
		return (NULL);
	return (cub);
}
