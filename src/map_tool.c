/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_tool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 22:02:25 by gehebert          #+#    #+#             */
/*   Updated: 2023/03/07 18:24:45 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// error display / free array : chk_fail
int	error(char *error, t_map *map)
{
	int	i;

	i = -1;
	write(1, "Error\n", 6);
	write(1, error, ft_strlen(error));
	if (map->tab == NULL)
		return(1);
	while (map->tab[++i])
		free(map->tab[i]);
	return (1);
}
	//error("1. You are trying to open a dir.\n", map);
	//error("2. Wrong file name or extention\n", map);
	//error("3. There is an error in your map, Please verify\n", map);
	// error("4. Your map isn't surrounded by walls", m);
	//error("5. Your map is uncomplete, please verify", m);
	// error("6. Map contains unrecognized characters", map);
	// error("7, Texture mapping Name error !\n", map);
	// error("8, Texture mapping Path error !\n", map);


// line_len with "1":"0":" "
int	int_strlen(const char *s)
{
	int	i;

	i = 0;
	if (s == NULL)
		return (0);
	while (s[i] != '\0' && s[i] != '\n')
		i++;
	return (i);
}

// mapping:  cub_file  &&  tex_name 
int	ft_strfcmp(const char	*s1, const char	*s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;
	size_t			i;
	size_t			j;

	i = ft_strlen(s1) - n;
	j = ft_strlen(s2) - n;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while ((str1[i] != '\0' || str2[j] != '\0'))
	{
		if (str1[i] != str2[j])
			return (str1[i] - str2[j]);
		i++;
		j++;
	}
	return (0);
}

// cmp *char , *charset return index found else -1;
int	ft_in_set(const char *c, const char *set)
{
	int	i;
	int j;
	
	j = 0;
	while(c[j])
	{
		i = 0;
		write(1, &c[j], 1);
		while (set[i])
		{
			if (c[j] == set[i])
				return (i);
			i++;
		}
		j++;
	}
	return (-1);
}

// load frame with map value at index wuth len ...
char	*ft_strncpy_i(char *dst, const char *src, size_t len, unsigned int i)
{

	while (src[i] != '\0' && i < len)
	{
		write(1, &src[i], 1);
		dst[i] = src[i];
		i++;
	}
	// while (i < len)
	// {
	// 	dst[i] = '\0';
	// 	i++;
	// }
	return (dst);
}


