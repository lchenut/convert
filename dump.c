/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dump.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchenut <lchenut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/12 10:23:53 by lchenut           #+#    #+#             */
/*   Updated: 2015/10/12 14:43:02 by lchenut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/convert.h"

int main(int ac, char **av)
{
	unsigned char	s[1024];
	s[0] = '_';
	s[1] = '1';
	s[2] = 'B';
	s[3] = '.';
	s[4] = 'b';
	s[5] = 'm';
	s[6] = 'p';
	s[7] = 0;
	printf("%llx\n", *(long long *)s);
	if (ac != 2)
		return (0);
	int fd = open(av[1], O_RDONLY);
	if (fd == -1)
		return (0);
	read(fd, s, 1024);
	close(fd);
	int offset = *((int *)(s + 10));
	int i = 0;
	printf("unsigned char	header_1B = {\t");
	while (i < offset)
	{
		printf("\t%#hhx", s[i]);
		if (++i < offset)
			printf(",\n");
	}
	printf("\n};\n");
	return (0);
}
