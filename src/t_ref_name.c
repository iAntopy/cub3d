/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_ref_name.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 08:22:23 by gehebert          #+#    #+#             */
/*   Updated: 2023/05/11 20:33:54 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

// t_cub	*mx_struct(t_map *m, t_cub *cub)
	// {
	// 	int			k;
	// 	int			p_box;
	// 	const char	*chrs;

	// 	printf("mx_struct start \n");
	// 	chrs = cub->box.chrs;
	// 	k = -1;
	// 	m->mx = (t_matrx ***)calloc(sizeof(t_matrx **), m->height);
	// 	while (++k < m->height)
	// 		m->mx[k] = (t_matrx **)calloc(sizeof(t_matrx *), m->width);
	// 	m->pos_y = 0;
	// 	while (m->pos_y < m->height)
	// 	{
	// 		m->pos_x = 0;
	// 		while (m->pos_x < m->width)
	// 		{
	// 			p_box = ft_in_set((m->m[m->pos_y][m->pos_x]), chrs);
	// 			if (p_box != -1 || p_box == (int)ft_strlen(chrs) - 1)
	// 				m->mx[m->pos_y][m->pos_x] = &cub->pset[p_box];
	// 			m->pos_x++;
	// 		}
	// 		m->pos_y++;
	// 	}
	// 	printf("mx_struct exit\n");
	// 	return (cub);
// }

// t_cub	*chsr_feed(t_cub *cub)
// {
	// 		int	i;
	// 		int	j;

	// 		j = 0;
	// 		i = -1;
	// 		cub->box.chrs = (char *)malloc(sizeof(char) * cub->box.chrs_len + 2);
	// 		while (*cub->map.raw && cub->map.raw[++i] && j < cub->box.chrs_len)
	// 		{
	// 			if (cub->map.raw[i][0] > 32 && cub->map.raw[i][0] < 97 &&
	// 				cub->map.raw[i][1] == 32)
	// 			{
	// 				cub->box.chrs[j] = cub->map.raw[i][0];
	// 				++j;
	// 			}
	// 		}
	// 		cub->box.chrs[j++] = '@';
	// 		cub->box.chrs[j] = '\0';
	// 		printf("NEW CHRS {%s} len[%d]\n\n", cub->box.chrs, j);
	// 		return (cub);
// }

char	*chrs_builder(t_cub *cub)
{
	int	i;
	int	j;
	char **rawz;

	j = 0;
	i = 0;
	printf("raw vision raw[0][0] = %c \n", *cub->map.raw[0] );
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

t_cub	*e_mtrx_link(t_cub *cub, t_box *box, char **raw)
{
	int 	raw_len;
	char	*tex_path;
	char	*tex_name;
	int		i;
		int		d_id;

	i = -1;
	box->pnum = 0;
	box->xform = (mlx_texture_t **)calloc(sizeof(mlx_texture_t *), box->xnum + 1);
	if (!box->xform)
		return (NULL);
	box->n_objs = 0;
	while (++i < box->xnum + box->n_dual )
	{
		if (raw[i][0] > 32)
		{


			raw_len = ft_strlen(raw[i]);
			tex_name = ft_substr(raw[i], 0, 1);
			tex_path = ft_substr(raw[i], 2, raw_len - 2);
			
			if(ft_in_set(tex_name[0], (const char *)MAP_MCHR) != -1)
			{
				if (raw[i][0] < 48) /// meta << number 
				{
					cub = meta_builder(cub, box, tex_name, &cub->objs);			
					box->n_objs++;
				}
			}
			else if (box->open_sky == 0)
			{
				d_id = ft_in_set(tex_name[0], (const char *)MAP_NCHR); 
				
				if(d_id != -1)
					cub = dual_builder(cub, d_id, tex_path);
				
				// box->pnum++;
			}				
			else if (raw[i][0] == 'z')
			{
				cub->box.open_sky = 1;
				cub->box.sky = mlx_load_png(tex_path);
				if (!cub->box.sky)
					return (report_mlx_tex_load_failed(tex_path));
				box->sky = box->xform[i];
			}
			// box->xform[i] = mlx_load_png(tex_path);
			// if (!box->xform[i])
			// 	return (report_mlx_tex_load_failed(tex_path));
			// printf("\nSky_tex:[%d]  <{%c}> {{%s}}\n", i, raw[i][0], tex_path);
				
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
	printf("_pset[%d]_open_sky[%d]__\n\n", cub->box.pset, cub->box.open_sky);
	
	cub = e_mtrx_link(cub, box, map->raw);
	// if (cub->box.open_sky != 0)
	// 	cub->tex.sky_tex = cub->box.sky;
	cub->box.chrs = chrs_builder(cub);
		// cub = chsr_feed(cub);

	return (cub);
}
