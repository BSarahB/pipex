/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/13 16:02:24 by mbenmesb          #+#    #+#             */
/*   Updated: 2021/11/13 16:02:28 by mbenmesb         ###   ########.fr       */
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

int	ft_execve_join(t_struct *ptr, char **envp, char **argv_cmd)
{
	char	*path_cmd_joined;
	int		i;
	int		exec_return;

	exec_return = 0;
	i = -1;
	while ((*ptr).path_tab[++i])
	{
		path_cmd_joined = ft_strjoin((*ptr).path_tab[i], argv_cmd[0]);
		exec_return = execve(path_cmd_joined, argv_cmd, envp);
		free(path_cmd_joined);
		if (exec_return != -1)
			break ;
	}
	return (exec_return);
}

char	**ft_get_path(char **envp)
{
	char	**path_addr;
	int		i;

	i = 0;
	path_addr = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_addr = ft_split(&envp[i][5], ':');
			break ;
		}
		i++;
	}
	i = 0;
	return (path_addr);
}
