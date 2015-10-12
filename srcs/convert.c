/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchenut <lchenut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/12 09:50:14 by lchenut           #+#    #+#             */
/*   Updated: 2015/10/12 18:24:04 by lchenut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <convert.h>

unsigned char	header_1B[] = { 0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0, 0x3e, 0, 0,\
	0, 0x28, 0, 0, 0 };
unsigned char	info_1B[] = { 0x1, 0, 0x1, 0, 0, 0, 0, 0, 0, 0x18, 0, 0, 0x12,\
	0xd, 0, 0, 0x12, 0xd, 0, 0, 0x2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff,\
	0xff, 0xff, 0 };

int		fill_header(t_conv *conv)
{
	char	garbage[256];
	int		offset;

	write(conv->fdw, header_1B, sizeof(header_1B));
	read(conv->fdr, garbage, sizeof(header_1B));
	offset = *((int *)(garbage + 10)) - sizeof(header_1B);
	int i = read(conv->fdr, garbage, offset);
	printf("%i\n", i);
	conv->width = *((int *)garbage);
	conv->bmp_type = *((int *)(garbage + 10));
	write(conv->fdw, garbage, 8);
	write(conv->fdw, info_1B, sizeof(info_1B));
	printf("-- %i -- %i --\n", conv->bmp_type, conv->width);
	if (conv->bmp_type != 32 && conv->bmp_type != 24)
	{
		close(conv->fdw);
		close(conv->fdr);
		dprintf(2, "Conversion interrupted: file is a %i bitmap type\n", conv->bmp_type);
		exit(0);
	}
	return (0);
}

void	fill_content_32(t_conv *conv)
{
	int		wid = 0;
	int		cnt = 0;
	char	cpy = 0;
	int		cpt = 0;
	int		med = 0;
	int		ncp = 0;

	wid ^= wid;
	cpt = 7;
	cpy = 0xFF;
	ncp ^= ncp;
	while (read(conv->fdr, &cnt, 4))
	{
		med = ((unsigned char)((cnt >> 8) & 0xFF)
			+ (unsigned char)((cnt >> 16) & 0xFF)
			+ (unsigned char)((cnt >> 24) & 0xFF)) / 3;
		cpy ^= ((med > conv->brightness) ? 0 : 1) << cpt;
		if (--cpt == -1)
		{
			write(conv->fdw, &cpy, 1);
			cpy = 0xFF;
			cpt = 7;
			ncp++;
		}
		wid++;
		if ((conv->width % 4) && wid == conv->width)
		{
			write(conv->fdw, &cpy, 1);
			cpy = 0xFF;
			cpt = 7;
			while (!(++ncp % 16))
				write(conv->fdw, &cpy, 1);
			wid ^= wid;
		}	
	}
}

void	fill_content_24(t_conv *conv)
{
	int		wid = 0;
	int		cnt = 0;
	char	cpy = 0;
	int		cpt = 0;
	int		med = 0;
	int		ncp = 0;

	wid ^= wid;
	cpt = 7;
	cpy = 0xFF;
	ncp ^= ncp;
	while (read(conv->fdr, &cnt, 3))
	{
		med = ((unsigned char)((cnt >> 8) & 0xFF)
			+ (unsigned char)((cnt >> 16) & 0xFF)
			+ (unsigned char)((cnt >> 0) & 0xFF)) / 3;
		cpy ^= ((med > conv->brightness) ? 0 : 1) << cpt;
		if (--cpt == -1)
		{
			write(conv->fdw, &cpy, 1);
			cpy = 0xFF;
			cpt = 7;
			ncp++;
		}
		wid++;
		if ((conv->width % 4) && wid == conv->width)
		{
			write(conv->fdw, &cpy, 1);
			cpy = 0xFF;
			cpt = 7;
			while (++ncp % 4)
			{
				write(conv->fdw, &cpy, 1);
			}
			wid *= 3;
			while ((wid % 4))
			{
				wid++;
				read(conv->fdr, &cnt, 1);
			}
			wid ^= wid;
		}	
	}
}

int main(int ac, char **av)
{
	t_conv	conv;

	cv_init(ac, av, &conv);
	fill_header(&conv);
	if (conv.bmp_type == 32)
		fill_content_32(&conv);
	else if (conv.bmp_type == 24)
		fill_content_24(&conv);
	close(conv.fdr);
	close(conv.fdw);
}
