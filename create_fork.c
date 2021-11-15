/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_fork.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 22:09:14 by mbenmesb          #+#    #+#             */
/*   Updated: 2021/11/15 22:09:25 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex.h"

int	ft_parent_process(t_struct *ptr, char **argv, char **envp)
{
	char	**argv_cmd2;
	char	**argv_abs_cmd2;
	int		exec_return;

	exec_return = 0;
	argv_abs_cmd2 = ft_split(argv[3], ' ');
	argv_cmd2 = ft_get_argv_cmd(3, argv);
	close (STDIN_FILENO);
	(void)dup((*ptr).p[0]);
	close ((*ptr).p[0]);
	close ((*ptr).p[1]);
	dup2((*ptr).fd2, STDOUT_FILENO);
	if (execve(argv_abs_cmd2[0], argv_abs_cmd2, envp) == -1)
		exec_return = ft_execve_join(ptr, envp, argv_cmd2);
	if (exec_return == -1 && (errno == 2 || errno == 13))
		(*ptr).errnum = 127;
	ft_free_tab(&argv_abs_cmd2);
	ft_free_tab(&argv_cmd2);
	return (exec_return);
}

int	ft_child_process(t_struct *ptr, char **argv, char **envp)
{
	char	**argv_cmd1;
	char	**argv_abs_cmd1;
	int		exec_return;

	exec_return = 0;
	argv_abs_cmd1 = ft_split(argv[2], ' ');
	argv_cmd1 = ft_get_argv_cmd(2, argv);
	close (STDOUT_FILENO);
	dup2((*ptr).fd1, STDIN_FILENO);
	(void)dup((*ptr).p[1]);
	close ((*ptr).p[1]);
	close ((*ptr).p[0]);
	if (execve(argv_abs_cmd1[0], argv_abs_cmd1, envp) == -1)
		exec_return = ft_execve_join(ptr, envp, argv_cmd1);
	if (exec_return == -1 && (errno == 2 || errno == 13))
		(*ptr).errnum = 127;
	ft_free_tab(&argv_abs_cmd1);
	ft_free_tab(&argv_cmd1);
	return (exec_return);
}

int	ft_create_child(t_struct *ptr, char **argv, char **envp)
{
	int	ret;

	ret = ft_child_process(ptr, argv, envp);
	if (ret == -1)
	{
		ft_error("pipex: command not found: ");
		ft_error(argv[2]);
		ft_error("\n");
		ft_check_close_error((*ptr).fd1);
		if ((*ptr).errnum == 127)
		{
			(*ptr).errnum = 0;
			return (127);
		}
		return (1);
	}
	if (ft_check_close_error((*ptr).fd1))
		return (1);
	return (0);
}

int	ft_create_parent(t_struct *ptr, char **argv, char **envp)
{
	int	ret;

	ret = ft_parent_process(ptr, argv, envp);
	if (ret == -1)
	{
		ft_error("pipex: command not found: ");
		ft_error(argv[2]);
		ft_error("\n");
		ft_check_close_error((*ptr).fd2);
		if ((*ptr).errnum == 127)
		{
			(*ptr).errnum = 0;
			return (127);
		}
		return (1);
	}
	if (ft_check_close_error((*ptr).fd2))
		return (1);
	return (0);
}
