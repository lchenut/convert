/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse32.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchenut <lchenut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/11 13:38:59 by lchenut           #+#    #+#             */
/*   Updated: 2015/10/11 18:16:49 by lchenut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

int				width;
int				is_32;

typedef struct s_head
{
	uint32_t	size;
	uint32_t	crap;
	uint32_t	offset;
}				t_head;

int		init_fd(char *av, int *fd32, int *fdw, int *fdc)
{
	if ((*fd32 = open(av, O_RDONLY)) < 0)
		return (-1);
	if ((*fdw = open("test.bmp", O_WRONLY | O_CREAT, 0644)) < 0)
		return (-1);
	if ((*fdc = open("dump.bmp", O_RDONLY)) < 0)
		return (-1);
	return (0);
}

int		fill_header(int fd32, int fdw, int fdc)
{
	char	garbage[256];
	int		offset;
	int		tofill;

	read(fdc, garbage, 26);
	tofill = *((int *)(garbage + 10)) - 26;
	write(fdw, garbage, 18);
	read(fd32, garbage, 26);
	offset = *((int *)(garbage + 10)) - 26;
	write(fdw, garbage + 18, 8);
	width = *((int *)(garbage + 18));
	read(fdc, garbage, tofill);
	write(fdw, garbage, tofill);
	read(fd32, garbage, offset);
	is_32 = *((short *)(garbage + 2));
	close(fdc);
	return (0);
}

void	fill_content_32(int fd32, int fdw)
{
	int		wid;
	int		cnt;
	char	cpy;
	int		cpt;
	int		med;
	int		ncp;

	wid ^= wid;
	cpt = 7;
	cpy = 0xFF;
	ncp ^= ncp;
	while (read(fd32, &cnt, 4))
	{
		med = ((unsigned char)((cnt >> 8) & 0xFF)
			+ (unsigned char)((cnt >> 16) & 0xFF)
			+ (unsigned char)((cnt >> 24) & 0xFF)) / 3;
		cpy ^= ((med > 0x7F) ? 0 : 1) << cpt;
		if (--cpt == -1)
		{
			write(fdw, &cpy, 1);
			cpy = 0xFF;
			cpt = 7;
			ncp++;
		}
		wid++;
		if ((width % 4) && wid == width)
		{
			write(fdw, &cpy, 1);
			cpy = 0xFF;
			cpt = 7;
			while (!(++ncp % 16))
				write(fdw, &cpy, 1);
			wid ^= wid;
		}	
	}
}

void	fill_content_24(int fd24, int fdw)
{
	int		wid;
	int		cnt;
	char	cpy;
	int		cpt;
	int		med;
	int		ncp;

	wid ^= wid;
	cpt = 7;
	cpy = 0xFF;
	ncp ^= ncp;
	int t = 0;
	while (read(fd24, &cnt, 3))
	{
		if (t == 1) { t = 2; printf("%i %i %i %x\n", wid, ncp, cpy, cnt); }
		med = ((unsigned char)((cnt >> 8) & 0xFF)
			+ (unsigned char)((cnt >> 16) & 0xFF)
			+ (unsigned char)((cnt >> 0) & 0xFF)) / 3;
		cpy ^= ((med > 0x3F) ? 0 : 1) << cpt;
		if (--cpt == -1)
		{
			write(fdw, &cpy, 1);
			cpy = 0xFF;
			cpt = 7;
			ncp++;
		}
		wid++;
		if ((width % 4) && wid == width)
		{
			write(fdw, &cpy, 1);
			cpy = 0xFF;
			cpt = 7;
			if (t == 0)
				printf("%i %i %i %x\n", wid, wid * 3, ncp, cnt);
			while (++ncp % 4)
				write(fdw, &cpy, 1);
			wid *= 3;
			while ((wid % 4))
			{
				printf("%i\n", wid);
				wid++;
				read(fd24, &cnt, 1);
			}
			if (t++ == 0)
				printf("%i %i %i %x\n", wid, wid * 3, ncp, cnt);
			wid ^= wid;
		}	
	}
}

int main(int ac, char **av)
{
	int		fd32, fdw, fdc;

	if (ac != 2 || init_fd(av[1], &fd32, &fdw, &fdc) < 0)
		return (dprintf(2, "Error\n"));
	fill_header(fd32, fdw, fdc);
	printf("%i %i\n", is_32, width);
	if (is_32 == 32)
		fill_content_32(fd32, fdw);
	else if (is_32 == 24)
		fill_content_24(fd32, fdw);
	close(fd32); close(fdw);
}
