/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_content_24.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchenut <lchenut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/15 17:09:38 by lchenut           #+#    #+#             */
/*   Updated: 2015/10/15 17:29:26 by lchenut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <convert.h>

/* Pour moult informations (ou pas), voir les commentaires de fill_content_32.c.
 * Les seules différences entre les deux viennent:
 * 1°) du fait qu'en plus de padder l'image sur laquelle on écrit, on doit
 * padder éventuellement celle sur laquelle on lit, 
 * 2°) que les couleurs RGB ne sont plus décalées de 8 bits.
 * Sinon, c'est la même chose.
 * C'est potentiellement possible de faire une fonction générique aux deux
 * formats, mais j'ai eu la flemme.
 */

void	fill_content_24(t_conv *conv)
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
	while (read(conv->fdr, &content, 3))
	{
		average = ((unsigned char)(content & 0xFF)
			+ (unsigned char)((content >> 8) & 0xFF)
			+ (unsigned char)((content >> 16) & 0xFF)) / 3;
		copy |= ((average > conv->brightness) ? 1 : 0) << cpt;
		if (--cpt == -1)
		{
			write(conv->fdw, &copy, 1);
			char_written++;
			copy = 0;
			cpt = 7;
		}
		width++;
		if (width == conv->width)
		{
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
			width *= 3;
			while (width % 4)
			{
				read(conv->fdr, &content, 1);
				width++;
			}
			width = 0;
			char_written = 0;
		}
	}
}
