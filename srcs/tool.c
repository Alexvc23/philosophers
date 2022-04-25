/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvalenci <jvalenci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 10:58:55 by jvalenci          #+#    #+#             */
/*   Updated: 2022/04/22 16:11:51 by jvalenci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philosophers.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*ptr1;
	unsigned char	*ptr2;

	ptr1 = (unsigned char *)s1;
	ptr2 = (unsigned char *)s2;
	if (!n)
		return (0);
	while (*ptr1 && *ptr2 && *ptr1 == *ptr2 && --n)
	{
		ptr1++;
		ptr2++;
	}
	return (*ptr1 - *ptr2);
}

uint64_t ft_get_time(void)
{
    struct timeval tp;
    uint64_t milliseconds;

    gettimeofday(&tp, NULL);
    milliseconds = tp.tv_sec * 1000;
    milliseconds += tp.tv_usec / 1000;
    return (milliseconds);
}