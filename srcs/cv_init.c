/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cv_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchenut <lchenut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/12 14:01:05 by lchenut           #+#    #+#             */
/*   Updated: 2015/10/12 16:29:09 by lchenut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <convert.h>

static void	get_usage(char *av)
{
	dprintf(2, "usage: %s [-b percent][-h] file.bmp [output_file_name]\n", av);
	exit(-1);
}

static void	get_help(char *av)
{
	dprintf(2, "usage: %s [-b percent][-h] file.bmp [output_file_name]\n", av);
	dprintf(2, "convert is a program that converts (tada!) an image bitmap 32 "\
			"or 24 bits per pixel in an image bitmap 1 bit per pixel\n");
	dprintf(2, "-h -- print this help message\n");
	dprintf(2, "-b percent -- define the percent between 0 and 100 of brightness of the image output\n");
	exit(0);
}

static void	get_brightness(char **av, int *index, t_conv *conv)
{
	int		percent;

	conv->brightness = 0xFF;
	percent = 50;
	*index = 1;
	while (av[*index])
	{
		if (av[*index][0] != 45) break ;
		if (av[*index][0] == 45 && av[*index][1] == 45 && av[*index][2] == 0)
		{
			(*index)++;
			break ;
		}
		if (av[*index][1] == 'h')
			get_help(av[0]);
		else if (av[*index][1] == 'b' && isdigit(av[*index][2]))
			percent = atoi(av[*index] + 2);
		else if (av[*index][1] == 'b' && !av[*index][2]
				&& av[(*index) + 1] && isdigit(av[(*index) + 1][0]))
			percent = atoi(av[(*index)++ + 1]);
		else
			get_usage(av[0]);
		(*index)++;
	}
	if (!av[*index] || (av[*index] && av[*index + 1] && av[*index + 2]))
		get_usage(av[0]);
	if (percent > 100 || percent < 0)
		percent = 50;
	conv->brightness = (conv->brightness * percent) / 100;
}

char		*get_name_new_file(char *av, char *file)
{
	char	new_file[128];
	size_t	len;

	if (av != NULL)
	{
		strncpy(new_file, av, 120);
		len = strlen(new_file);
		if (strcmp(new_file + len - 4, ".bmp"))
			strcat(new_file, ".bmp");
	}
	else
	{
		strncpy(new_file, file, 120);
		len = strlen(new_file);
		new_file[len - 4] = 0;
		strcat(new_file, "_1B.bmp");
	}
	return (strdup(new_file));
}

void		cv_init(int ac, char **av, t_conv *conv)
{
	int		index;
	char	*is_bmp;
	char	*file;

	if (ac == 1)
		get_help(av[0]);
	get_brightness(av, &index, conv);
	is_bmp = strrchr(av[index], '.');
	if (!is_bmp || (is_bmp && strcmp(is_bmp, ".bmp")))
		get_usage(av[0]);
	file = (strrchr(av[index], '/')) ? strrchr(av[index], '/') + 1 : av[index];
	if ((conv->fdr = open(av[index], O_RDONLY)) < 0)
	{
		perror(av[index]);
		get_usage(av[0]);
	}
	file = get_name_new_file(av[index + 1], file);
	if ((conv->fdw = open(file, O_WRONLY | O_CREAT, 0644)) < 0)
	{
		free(file);
		perror(file);
		get_usage(av[0]);
	}
	free(file);
}
