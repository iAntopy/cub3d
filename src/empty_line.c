
#include "../include/cub3d.h"

int	is_empty_line(char *line)
{
	if (!line)
		return (0);
	while (*line && ft_isspace(*line))
		line++;
	return (*line == '\0');	
}

// if nb_lines == 0, skips only empty lines. Otherwise skip nb_lines.
char	*skip_file_lines(t_map *map, int fd, int nb_lines)
{
	char	*tmp;
	int	i;

	i = 0;
	tmp = get_next_line(fd);
	while ((!nb_lines && is_empty_line(tmp)) || (tmp && i < nb_lines))
	{
		map->lines_to_map++;
		i++;
		free(tmp);
		tmp = get_next_line(fd);
	}
	return (tmp);
}
