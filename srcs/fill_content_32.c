/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_content_32.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchenut <lchenut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/15 16:31:01 by lchenut           #+#    #+#             */
/*   Updated: 2015/10/15 17:34:06 by lchenut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <convert.h>

/* Fonction qui parcourt l'image donnée en entrée, fait la moyenne des valeurs
 * RGB de chaque pixel, et écrit un pixel noir ou blanc, en fonction de cette
 * moyenne, sur l'image de sortie.
 */

void	fill_content_32(t_conv *conv)
{
	int		width;
	int		content;
	int		cpt;
	int		char_written;
	int		average;
	char	copy;

	width = 0;
	cpt = 7;
	copy = 0;
	char_written = 0;
	while (read(conv->fdr, &content, 4))
	{
		/* average == moyenne de RGB
		 */
		average = ((unsigned char)((content >> 8) & 0xFF)
			+ (unsigned char)((content >> 16) & 0xFF)
			+ (unsigned char)((content >> 24) & 0xFF)) / 3;
		copy |= ((average > conv->brightness) ? 1 : 0) << cpt;
		if (--cpt == -1)
		{
			/* Une fois que le caractère copy est 'rempli' on l'écrit dans
			 * l'image de sortie.
			 */
			write(conv->fdw, &copy, 1);
			char_written++;
			copy = 0;
			cpt = 7;
		}
		width++;
		if (width == conv->width)
		{
			/* Quand la ligne est fini, on 'padde' le reste jusqu'à ce qu'on ait
			 * écrit n % 4 caractères. (cf: sujet/format_bitmap)
			 */
			if (cpt != 7)
			{
				write(conv->fdw, &copy, 1);
				char_written++;
				copy = 0;
				cpt = 7;
			}
			while (char_written % 4)
			{
				write(conv->fdw, &copy, 1);
				char_written++;
			}
			width = 0;
			char_written = 0;
		}
	}
}
