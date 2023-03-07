/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tex_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gehebert <gehebert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 08:03:53 by gehebert          #+#    #+#             */
/*   Updated: 2023/03/06 09:29:27 by gehebert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"
// #include "../include/textur.h"

	//      // tex_pars
			// arg[0] = tex_name char* { "W","N","E","S","F","C"} (upper || lower)
			// arg[1] = char_case: tex_path  (*char must begin by "./")
			// arg[1] = num_case : color (get RGB bit_shift << R[255],G[255],B[255])
			// rules        : chk one or more empty_line_separator
			//              : orderless mapping info -> attr name/value


	/// name_chk :: ft_strfcmp(arg[x], tex_name, tex_name_len);

	//        /// path_chk :: (start: ".\" , end: != "\") 
// static char	*path_from_line(int start, char const *line)
	// 	{
	// 			int		start_def;
	// 			int		end;
	// 			char	*path;

	// 			start_def = start;
	// 			if (!line)
	// 				return (NULL);
	// 			while (line[start] && line[start] == ' ')
	// 				start++;
	// 			end = ft_strlen(line);
	// 			while (line[end] == ' ')
	// 				end--;
	// 			if (start == start_def || end - start <= 0
	// 				|| !(path = ft_substr(line, start, end - start)))
	// 				return (NULL);
	// 			return (path);
// }

	/// color_to_int ...
// static int 	str_to_color(int r, int g, int b, int a)
// {
// 	return (r << 24 | g << 16 | b << 8 | a);
// 					// int	i;
// 					// int	color;
// 					// int	tmp;

// 					// i = 0;
// 					// color = 0;
// 					// while (str)
// 					// {
// 					// 	tmp = ft_atoi(str->content);
// 					// 	if (tmp < 0 || tmp > 255)
// 					// 		return (-1);
// 					// 	color = color | (tmp << (16 - (i++ * 8)));
// 					// 	str = str->next;
// 					// }
// }

	/// should get file info about tex_color (sky/floor)
// int 	parse_color(t_config *config, int key, char const *line)
		// {
		// 	int			i;
		// 	unsigned	color;
		// 	t_str		*str[2];

		// 	i = 1;
		// 	while (line[i])
		// 		if (!ft_in_set(line[i++], " ,0123456789"))
		// 			return (0);
		// 	str[0] = NULL;
		// 	str[1] = NULL;
		// 	if (!(str[0] = ft_split(line, ' ')) || str_length(str[0]) != 2
		// 		|| !(str[1] = ft_split(str[0]->next->content, ','))
		// 		|| str_length(str[1]) != 3)
		// 		return (str_clear(&str[0]) || str_clear(&str[1]));
		// 	if ((int)((color = str_to_color(str[1]))) < 0)
		// 		return (str_clear(&str[0]) || str_clear(&str[1]));
		// 	config->c[(key == C_F) ? TEX_FLOOR : TEX_SKY] = color;
		// 	return ((str_clear(&str[0]) || str_clear(&str[1])) | 1);
// }
		//chk if arg = 12
		// txtr_name + path * 4 = 8
		// txtr_color_name + color_id * 2 = 4

	/// for each: chk_name ref AND chk_path
		//		if (yes && yes) attib name to path ... 
		//		also if (name is color_name) : str_to_color
/// get input frm file
t_map	*tex_parse(t_cub cub, t_map *map, int fd)
{
	char **txtr;
	char *line;
	// int	color;
	int nb;
	int id;
	
	
	nb = 0;
	while (nb < 4)
	{
		line = get_next_line(fd);
		while(ft_strlen(line) < 2)
			line = get_next_line(fd);
		if(line)
		{			
			txtr = ft_split(line, ' ');
			if (ft_strlen(txtr[0]) > 2)	
				error("7, Texture mapping Name error !\n", map);
			if (!txtr[1])	
				error("8, Texture mapping Path error !\n", map);
				
			id = 0;
			id = ft_in_set((const char *)txtr[0], (const char *)"WNESCF");
				printf("DEBUG: line_num: %d :tex_ref : %d tex_ref : %s tex_name: %s \n", nb, id, txtr[0], txtr[1]);
			if ( id < 0)
			{
				error("9, Texture Name unmatching error !\n", map);
			}
			else if (id < 4)
				cub.tex.tex_n[id] = txtr[1];//"./tex/s_side.png";
			printf("DEBUG:  tex_id: %d :: tex_name: %s :: \n", id, cub.tex.tex_n[id]); 
					// else if (id < 1)
					// {
					// 	// cub.tex.walls[0] = mlx_load_png(txtr[1]);
					// 	// cub.tex.walls[0] = mlx_load_png("tex/w_side.png");
					// 	cub.tex.tex_n[id] = txtr[1];//"./tex/w_side.png"; 
					// }
					// else if (id < 2)
					// {
					//  	// cub.tex.walls[1] = mlx_load_png("tex/n_side.png");
					//  	// cub.tex.walls[1] = mlx_load_png(txtr[1]);
					// 	cub.tex.tex_n[id] =txtr[1];//"./tex/n_side.png"; 
					// }
					// else if (id < 3)
					// {
					// 	// cub.tex.walls[2] = mlx_load_png("./tex/e_side.png");
					// 	// cub.tex.walls[2] = mlx_load_png(txtr[1]);
					// 	cub.tex.tex_n[id] =txtr[1];//"./tex/e_side.png"; 	
					// }
				// {
					// cub.tex.walls[3] = mlx_load_png("./tex/s_side.png");
					// cub.tex.walls[3] = mlx_load_png(txtr[1]);
				// }
					// else if (id < 5)
					// {
					// 	color = str_to_color(ft_atoi(txtr[0]), ft_atoi(txtr[1]), ft_atoi(txtr[2]), 0);
					//  	memset(cub->grnd->pixels, 255, cub.grnd->width * cub->grnd->height * sizeof(int32_t));
					// }
					// else if (id < 6)
					// {
					// 	color = str_to_color(ft_atoi(txtr[0]), ft_atoi(txtr[1]), ft_atoi(txtr[2]), 0);
					// 	cub.img[1] = memset(cub.img[1]->pixels, 225, cub.img[1]->width * cub.img[1]->height * sizeof(int32_t));		
					// }
			nb++;		// 	// match tex_name to parse_color
		}
		else 
		{
			free(line);
			nb++;
			// break;
		}
	free(line);	
	
	}
	return (map);
}
