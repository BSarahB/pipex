/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/13 13:58:33 by mbenmesb          #+#    #+#             */
/*   Updated: 2021/11/13 13:58:36 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex.h"

void	ft_free_t_struct(t_struct **ptr)
{
	ft_free_tab(&((*ptr)->path_tab));
	free(*ptr);
	*ptr = NULL;
}

t_struct	*ft_struct_init(t_struct **ptr, char **argv)
{
	*ptr = (t_struct *)malloc(sizeof(t_struct));
	if (!(*ptr))
		return (0);
	(*ptr)->fd1 = open(argv[1], O_RDONLY);
	(*ptr)->fd2 = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	(*ptr)->retour = 0;
	(*ptr)->path_tab = 0;
	return (*ptr);
}

void	ft_create_pipe(t_struct *ptr)
{
	if (pipe((*ptr).p))
	{
		ft_free_t_struct(&ptr);
		perror("pipe");
		exit(0);
	}
}

int	ft_check_fork(t_struct *ptr, char **argv, char **envp)
{
	if ((*ptr).retour == -1)
	{
		ft_free_t_struct(&ptr);
		perror (" pb fork ");
		exit(1);
	}
	if ((*ptr).retour == 0)
	{
		if (ft_create_child(ptr, argv, envp))
		{
			ft_free_t_struct(&ptr);
			return (1);
		}
	}
	else
	{
		if (ft_create_parent(ptr, argv, envp))
		{
			ft_free_t_struct(&ptr);
			return (1);
		}
	}
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_struct	*ptr;

	if (argc < 5)
	{
		ft_putstr_fd("expected: ./pipex infile cmd1 cmd2 outfile \n", 1);
		return (1);
	}
	ptr = ft_struct_init(&ptr, argv);
	(*ptr).path_tab = ft_get_path(envp);
	if (ft_check_open_error(ptr))
	{
		ft_free_t_struct(&ptr);
		return (1);
	}
	ft_create_pipe(ptr);
	(*ptr).retour = fork();
	if (ft_check_fork(ptr, argv, envp))
		return (1);
	ft_free_t_struct(&ptr);
	return (0);
}
