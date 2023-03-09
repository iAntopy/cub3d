/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alpha_dimmer_test.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 15:55:20 by iamongeo          #+#    #+#             */
/*   Updated: 2023/03/03 18:54:07 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "libft.h"

#define WIDTH 512
#define HEIGHT 512

void	print_int_arr(int *arr, int w, int h)
{
	int	i;
	int	j;

	i = -1;
	while (++i < h)
	{
		j = -1;
		printf("[ ");
		while (++j < w)
			printf("%x ", *(arr++));
		printf(" ]\n");
	}
	printf("\n\n");
}

void	alpha_dimmer(int *arr, int w, int h, double dimmer)
{
	size_t	mask;
	size_t	*end;
	size_t	*start;
//	int	i;

	mask = ((size_t)(255.0f * dimmer) << 24) | 0x00ffffff;
//	printf("mask 1 = %zx\n", mask);
	mask |= (mask << 32);
//	printf("mask 2 = %zx\n", mask);
	start = (size_t *)arr;
	end = (size_t *)(arr + (w * h));
//	i = 0;
//	while (end > start)
//		*(--end) &= mask;

	while (end > start)
	{
		*(--end) &= mask;
		*(--end) &= mask;
		*(--end) &= mask;
		*(--end) &= mask;
		*(--end) &= mask;
		*(--end) &= mask;
		*(--end) &= mask;
		*(--end) &= mask;
		
		*(--end) &= mask;
		*(--end) &= mask;
		*(--end) &= mask;
		*(--end) &= mask;
		*(--end) &= mask;
		*(--end) &= mask;
		*(--end) &= mask;
		*(--end) &= mask;
	}
}

int	main()
{
//	int	arr[WIDTH * HEIGHT] = {[0 ... WIDTH * HEIGHT - 1] = 0xff7f7f7f};
	int	*arr;
	int	size = WIDTH * HEIGHT;

	arr = malloc(sizeof(int) * size);
	
	while (size--)
		arr[size] = 0xff7f7f7f;
//	print_int_arr(arr, WIDTH, HEIGHT);
//	printf("arr start : %p\n", arr);
//	ft_deltatime_usec_note(NULL);
	alpha_dimmer(arr, WIDTH, HEIGHT, 0.2f);
//	ft_deltatime_usec_note("This is great !\n");
//	printf("arr after : %p\n", arr);
//	print_int_arr(arr, WIDTH, HEIGHT);
//	printf("print over\n");
	return (0);
}