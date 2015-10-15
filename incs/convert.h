/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchenut <lchenut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/12 10:12:33 by lchenut           #+#    #+#             */
/*   Updated: 2015/10/15 17:18:09 by lchenut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONVERT_H
# define CONVERT_H

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct	s_convert
{
	int			bmp_type;
	int			brightness;
	int			width;
	int			fdr;
	int			fdw;
}				t_conv;

typedef struct s_head
{
	uint32_t	size;
	uint32_t	crap;
	uint32_t	offset;
}				t_head;

void			cv_init(int ac, char **av, t_conv *conv);
void			fill_content_32(t_conv *conv);
void			fill_content_24(t_conv *conv);

#endif
