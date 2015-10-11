/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchenut <lchenut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/11 12:16:43 by lchenut           #+#    #+#             */
/*   Updated: 2015/10/11 13:43:24 by lchenut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

typedef struct	s_header_bmp
{
	uint32_t	file_size;
	uint32_t	crap;
	uint32_t	data_offset;
}				t_header;

int		main(int argc, char **argv)
{
	t_header	header;
	char		magic[2];
	short		p_info;
	int			i_info;
	int			fd;

	if (argc != 2)
	{
		printf("parse: usage %s file.bmp\n", argv[0]);
		return (0);
	}
	fd = open(argv[1], O_RDONLY);
	printf("%zu\n", sizeof(header));
	read(fd, magic, 2);
	read(fd, &header, 12);
	printf("----- HEADER -----\n");
	printf("Magic:       %.2s\n", magic);
	printf("Size:        %u\n", header.file_size);
	printf("Crap:        %u\n", header.crap);
	printf("Data offset: %u\n", header.data_offset);
	read(fd, &header, 12);
	printf("------ INFO ------\n");
	printf("Size info:   %u\n", header.file_size);
	printf("Width:       %u\n", header.crap);
	printf("Height:      %u\n", header.data_offset);
	read(fd, &p_info, 2);
	printf("Planes ?:    %hi\n", p_info);
	read(fd, &p_info, 2);
	printf("Bit Count:   %hi\n", p_info);
	read(fd, &i_info, 4);
	printf("Compression: %i\n", i_info);
	read(fd, &i_info, 4);
	printf("Image size:  %i\n", i_info);
	read(fd, &i_info, 4);
	printf("Xpixel/m:    %i\n", i_info);
	read(fd, &i_info, 4);
	printf("Ypixel/m:    %i\n", i_info);
	read(fd, &i_info, 4);
	printf("Color used:  %i\n", i_info);
	read(fd, &i_info, 4);
	printf("Color /!\\:   %i\n", i_info);
	return (0);
}
