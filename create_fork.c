/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_fork.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/13 14:57:06 by mbenmesb          #+#    #+#             */
/*   Updated: 2021/11/13 14:57:11 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex.h"

char	**ft_get_argv_cmd(int i, char **argv)
{
	char	**argv_cmd;
	char	slash[2];

	slash[0] = '/';
	slash[1] = 0;
	argv_cmd = ft_split(argv[i], ' ');
	ft_update_string(&argv_cmd[0], ft_strjoin(slash, argv_cmd[0]));
	return (argv_cmd);
}

int	ft_child_process(t_struct *ptr, char **argv, char **envp)
{
	char	*path_cmd1_joined;
	int		j;
	char	**argv_cmd1;
	int		exec_return;

	exec_return = 0;
	j = -1;
	argv_cmd1 = ft_get_argv_cmd(2, argv);
	close (STDOUT_FILENO);
	dup2((*ptr).fd1, STDIN_FILENO);
	(void)dup((*ptr).p[1]);
	close ((*ptr).p[1]);
	close ((*ptr).p[0]);
	while ((*ptr).path_tab[++j])
	{
		path_cmd1_joined = ft_strjoin((*ptr).path_tab[j], argv_cmd1[0]);
		exec_return = execve(path_cmd1_joined, argv_cmd1, envp);
		free(path_cmd1_joined);
		if (exec_return != -1)
			break ;
	}
	ft_free_tab(&argv_cmd1);
	return (exec_return);
}

int	ft_parent_process(t_struct *ptr, char **argv, char **envp)
{
	char	*path_cmd2_joined;
	int		i;
	char	**argv_cmd2;
	int		exec_return;

	exec_return = 0;
	i = -1;
	argv_cmd2 = ft_get_argv_cmd(3, argv);
	close (STDIN_FILENO);
	(void)dup((*ptr).p[0]);
	close ((*ptr).p[0]);
	close ((*ptr).p[1]);
	dup2((*ptr).fd2, STDOUT_FILENO);
	while ((*ptr).path_tab[++i])
	{
		path_cmd2_joined = ft_strjoin((*ptr).path_tab[i], argv_cmd2[0]);
		exec_return = execve(path_cmd2_joined, argv_cmd2, envp);
		free(path_cmd2_joined);
		if (exec_return != -1)
			break ;
	}
	ft_free_tab(&argv_cmd2);
	return (exec_return);
}

int	ft_create_child(t_struct *ptr, char **argv, char **envp)
{
	if (ft_child_process(ptr, argv, envp) == -1)
	{
		ft_error("pipex: command not found: ");
		ft_error(argv[2]);
		ft_error("\n");
		return (1);
	}
	if (ft_check_close_error((*ptr).fd1))
		return (1);
	return (0);
}

int	ft_create_parent(t_struct *ptr, char **argv, char **envp)
{
	if (ft_parent_process(ptr, argv, envp) == -1)
	{
		ft_error("pipex: command not found: ");
		ft_error(argv[3]);
		ft_error("\n");
		return (1);
	}
	if (ft_check_close_error((*ptr).fd2))
		return (1);
	return (0);
}
