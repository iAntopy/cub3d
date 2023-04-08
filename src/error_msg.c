/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_msg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 20:57:55 by iamongeo          #+#    #+#             */
/*   Updated: 2023/04/07 20:59:27 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int report_err(char *msg)
{
    ft_eprintf("CUB ERROR : Trying to instanciate object in wall.\n");
    return (-1);
}

int report_malloc_err(void)
{
    ft_eprintf("CUB ERROR : Malloc failed.\n");
    return (-1);
}