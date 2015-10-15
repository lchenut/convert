/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchenut <lchenut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/12 09:50:14 by lchenut           #+#    #+#             */
/*   Updated: 2015/10/15 17:09:58 by lchenut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <convert.h>

/*
 * Deux constantes pour remplir le header de l'image de sortie.
 */
const unsigned char	header_1B[] = { 0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0, 0x3e,\
	0, 0, 0, 0x28, 0, 0, 0 };
const unsigned char	info_1B[] = { 0x1, 0, 0x1, 0, 0, 0, 0, 0, 0, 0x18, 0, 0,\
	0x12, 0xd, 0, 0, 0x12, 0xd, 0, 0, 0x2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
	0xff, 0xff, 0xff, 0 };

int		fill_header(t_conv *conv)
{
	/* Comme son nom l'indique, la fonction écrit le header dans l'image  de
	 * sortie et se place à la fin du header de l'image bitmap en entrée.
	 */
	char	garbage[256];
	int		offset;

	write(conv->fdw, header_1B, sizeof(header_1B));
	read(conv->fdr, garbage, sizeof(header_1B));
	offset = *((int *)(garbage + 10)) - sizeof(header_1B);
	read(conv->fdr, garbage, offset);
	conv->width = *((int *)garbage);
	conv->bmp_type = *((short *)(garbage + 10));
	write(conv->fdw, garbage, 8);
	write(conv->fdw, info_1B, sizeof(info_1B));
	if (conv->bmp_type != 32 && conv->bmp_type != 24)
	{
		close(conv->fdw);
		close(conv->fdr);
		dprintf(2, "Conversion interrupted: file is a %i bitmap type\n", conv->bmp_type);
		exit(0);
	}
	return (0);
}

int		main(int ac, char **av)
{
	t_conv		conv;
	struct stat	buf;

	cv_init(ac, av, &conv);
	fill_header(&conv);
	if (conv.bmp_type == 32)
		fill_content_32(&conv);
	else if (conv.bmp_type == 24)
		fill_content_24(&conv);
	close(conv.fdr);
	lseek(conv.fdw, 0, SEEK_SET);
	if (!fstat(conv.fdw, &buf))
	{
		write(conv.fdw, "BM", 2);
		write(conv.fdw, &(buf.st_size), 4);
	}
	close(conv.fdw);
	return (0);
}
