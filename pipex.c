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

int	ft_get_parent_ret(t_struct *ptr, char **argv, char **envp)
{
	int	ret;

	ret = ft_create_parent(ptr, argv, envp);
	if (ret)
	{
		ft_free_t_struct(&ptr);
		return (ret);
	}
	return (0);
}

int	ft_get_child_ret(t_struct *ptr, char **argv, char **envp)
{
	int	ret;

	ret = ft_create_child(ptr, argv, envp);
	if (ret)
	{
		ft_free_t_struct(&ptr);
		return (ret);
	}
	return (0);
}

int	ft_check_fork(t_struct *ptr, char **argv, char **envp)
{
	int	ret;

	if ((*ptr).retour == -1)
	{
		ft_free_t_struct(&ptr);
		perror (" pb fork ");
		exit(1);
	}
	if ((*ptr).retour == 0)
	{
		ret = ft_get_child_ret(ptr, argv, envp);
		return (ret);
	}
	else
	{
		ret = ft_get_parent_ret(ptr, argv, envp);
		return (ret);
	}
}

void	ft_check_fork_fd1(t_struct *ptr, char **argv, char **envp, int ret)
{
	pid_t	retour_fd1;

	if (ret == 2)
		exit(1);
	if (ret == 1)
	{
		ft_error_msg(argv);
		ft_create_pipe(ptr);
		retour_fd1 = fork();
		if (retour_fd1 == -1)
		{
			ft_free_t_struct(&ptr);
			perror (" pb fork ");
			exit(1);
		}
		if (retour_fd1 == 0)
		{
			ret = ft_get_parent_ret(ptr, argv, envp);
			(*ptr).errnum = ret;
			exit(ret);
		}
		ret = (*ptr).errnum ;
		ft_free_t_struct(&ptr);
		exit(ret);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_struct	*ptr;
	int			ret;

	if (argc != 5)
	{
		ft_putstr_fd("usage: ./pipex infile cmd1 cmd2 outfile \n", 1);
		return (1);
	}
	ptr = ft_struct_init(&ptr, argv);
	(*ptr).path_tab = ft_get_path(envp);
	ret = ft_check_open_error(ptr);
	if (ret == 2 || ret == 1)
		ft_check_fork_fd1(ptr, argv, envp, ret);
	ft_create_pipe(ptr);
	(*ptr).retour = fork();
	ret = ft_check_fork(ptr, argv, envp);
	if (ret)
		return (ret);
	ft_free_t_struct(&ptr);
	return (0);
}
