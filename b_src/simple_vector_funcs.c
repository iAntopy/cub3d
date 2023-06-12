/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_vector_funcs.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 15:48:25 by iamongeo          #+#    #+#             */
/*   Updated: 2023/06/05 22:36:33 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// give map coord and a pointer to a int cell[2] return array to get cell 
// values for this position.
int	get_cell(float px, float py, int *cx, int *cy)
{
	static const float	inv_cw = 1.0f / CELL_WIDTH;

	*cx = (int)(inv_cw * px);
	*cy = (int)(inv_cw * py);
	return (1);
}

// res is a vec2 that receives the vector between the 2 points from and to.
void	find_vector_delta(float *from_, float *to_, float *res)
{
	res[0] = to_[0] - from_[0];
	res[1] = to_[1] - from_[1];
}

// Returns distance at dist ptr
float	normalize_vec2(float *v, float *dist_p)
{
	*dist_p = sqrtf(v[0] * v[0] + v[1] * v[1]);
	v[0] /= *dist_p;
	v[1] /= *dist_p;
	return (*dist_p);
}
