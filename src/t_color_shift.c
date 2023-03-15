/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_ref.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 19:34:59 by gehebert          #+#    #+#             */
/*   Updated: 2023/03/14 19:35:10 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int 	str_to_color(int r, int g, int b, int t)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

int get_t (int trgb)
{
	return((trgb >> 24) & 0xFF);
}

int get_r(int trgb)
{
	return((trgb >> 16) & 0xFF);
}

int get_g (int trgb)
{
	return((trgb >> 8) & 0xFF);
}

int get_b (int trgb)
{
	return(trgb & 0xFF);
}
