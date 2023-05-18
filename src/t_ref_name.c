/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_ref_name.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 08:22:23 by gehebert          #+#    #+#             */
/*   Updated: 2023/05/17 21:59:10 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"


char	*chrs_builder(t_cub *cub)
{
	int	i;
	int	j;
	char **rawz;

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


/// link' too too wide ++ doing too many thing!
t_cub	*e_mtrx_link(t_cub *cub, t_box *box, char **raw)
{
	int 	raw_len;
	char	*tex_path;
	char	*tex_name;
	int		i;
	int		j;
		int		d_id;

	i = - 1;
	j = 0;
	box->pnum = 0;
	box->xform = (mlx_texture_t **)calloc(sizeof(mlx_texture_t *), box->xnum + 1);
	if (!box->xform)
		return (NULL);
	cub->dual = (t_matrx *)malloc(sizeof(t_matrx) * cub->box.n_dual);
	if (!cub->dual)
		return (NULL);
	box->n_objs = 0;
	printf("LINK\n");
	while (++i < box->xnum + box->meta )
	{
		if (raw[i][0] > 32)
		{
			raw_len = ft_strlen(raw[i]);
			tex_name = ft_substr(raw[i], 0, 1);
			tex_path = ft_substr(raw[i], 2, raw_len - 2);
			
			/// meta << number 				// if (raw[i][0] < 48) /// meta << number 
			if ((ft_in_set(tex_name[0], (const char *)MAP_MCHR) != -1) && (raw[i][0] < 48) )
			{
				cub = meta_builder(cub, box, tex_name, &cub->objs);			
				box->n_objs++;
			}
			else if (ft_in_set(tex_name[0], (const char *)MAP_LCHR) != -1)
			{
				if (tex_name[0] == 'z')
				{
					cub->box.open_sky = 1;
					cub->box.sky = mlx_load_png(tex_path);
					if (!cub->box.sky)
						return (report_mlx_tex_load_failed(tex_path));
					printf("ZzZzZ XFORM:[%d]  CHRS{%c} path{{%s}} >>ptr%p ;;box.sky;;\n", j, raw[i][0], tex_path, cub->box.sky);
					cub->tex.skymap = cub->box.sky;
					cub->tex.sky_tex = cub->box.sky;
				}
				else
				{
					box->xform[j] = mlx_load_png(tex_path);
					if (!box->xform[j])
						return (report_mlx_tex_load_failed(tex_path));
					printf("XFORM:[%d]  CHRS{%c} path{{%s}} >>ptr%p\n", j, raw[i][0], tex_path, box->xform[j]);
					j++;
				}
			}
			else if (ft_in_set(tex_name[0], (const char *)MAP_NCHR) != -1)
			{
				d_id = ft_in_set(tex_name[0], (const char *)MAP_NCHR); 
				if(d_id != -1)
					cub = dual_builder(cub, d_id, tex_path);								
			}
		}
	}
	return (cub);
}
					// printf("sky tex after load : %p %p %p\n", cub->box.sky, cub->tex.skymap , cub->tex.sky_tex);
					// cub->tex.sky = cub->box.sky;
				

t_cub	*e_mtrx_count(t_cub *cub)
{
	int			i;
	const char	*rawz;

	i = -1;
	cub->box.chrs_len = 1;
	while (*cub->map.raw && cub->map.raw[++i])
	{
		rawz = cub->map.raw[i];
		if (rawz[1] != ' ')
			break ;
		if (rawz[0] > 32 && rawz[0] < 97 && rawz[1] == 32)
			++cub->box.chrs_len;
		if (ft_in_set(rawz[0], (const char *)MAP_MCHR) != -1)
			++cub->box.meta;
		if (ft_in_set(rawz[0], (const char *)MAP_UCHR) != -1)
			cub->box.pset++;
		if (ft_in_set(rawz[0], (const char *)MAP_NCHR) != -1)
			cub->box.n_dual++;
		if (ft_strchr_set(rawz, ".png") != NULL)
			++cub->box.xnum;
		if (rawz[0] == 'z')
			cub->box.open_sky = 1; // no  tile == skymap
	}

	return (cub);
}

t_cub	*e_list_txtr(t_cub *cub, t_box *box, t_map *map)
{
	box->xnum = 0;
	box->open_sky = 0;
	cub = e_mtrx_count(cub);

	printf("_LIST__meta[%d] xnum[%d]", cub->box.meta, cub->box.xnum);
	printf("_dual[%d]", cub->box.n_dual);
	printf("_pset[%d]_open_sky[%d]__\n\n", cub->box.pset, cub->box.open_sky);
	
	cub = e_mtrx_link(cub, box, map->raw);
	if (cub->box.open_sky != 0)
		cub->tex.skymap = cub->box.sky_tex;
	cub->box.chrs = chrs_builder(cub);
	return (cub);
}
