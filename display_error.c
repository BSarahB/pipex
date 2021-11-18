/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/13 16:10:59 by mbenmesb          #+#    #+#             */
/*   Updated: 2021/11/13 16:11:02 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex.h"

int	ft_check_open_error(t_struct *ptr)
{
	if ((*ptr).fd1 == -1)
	{
		return (1);
	}
	if ((*ptr).fd2 == -1)
	{
		perror("pipex");
		ft_free_t_struct(&ptr);
		return (2);
	}
	return (0);
}

int	ft_check_close_error(int fd)
{
	if (close(fd) == -1)
	{
		perror("pipex");
		return (1);
	}
	return (0);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (s != 0)
	{
		while (*s)
		{
			write(fd, s, 1);
			s++;
		}
	}
}

void	ft_error(char *const str)
{
	ft_putstr_fd(str, 2);
}

void	ft_error_msg(char **argv)
{
	ft_error("pipex: ");
	ft_error("no such file or directory: ");
	ft_error(argv[1]);
	ft_error("\n");
}
