/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_color_creat.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 12:44:08 by gehebert          #+#    #+#             */
/*   Updated: 2023/03/29 12:44:11 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	create_trgb(unsigned char t, unsigned char r, unsigned char g,
		unsigned char b)
{
	return (*(int *)(unsigned char [4]){b, g, r, t});
}

unsigned char	get_ut(int trgb)
{
	return (((unsigned char *)&trgb)[3]);
}

unsigned char	get_ub(int trgb)
{
	return (((unsigned char *)&trgb)[2]);
}

unsigned char	get_ug(int trgb)
{
	return (((unsigned char *)&trgb)[1]);
}

unsigned char	get_ur(int trgb)
{
	return (((unsigned char *)&trgb)[0]);
}
