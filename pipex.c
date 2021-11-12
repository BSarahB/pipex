/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 23:45:57 by mbenmesb          #+#    #+#             */
/*   Updated: 2021/11/12 23:46:14 by mbenmesb         ###   ########.fr       */
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

int	ft_check_open_error(t_struct *ptr)
{
	if (((*ptr).fd1 == -1) || ((*ptr).fd2 == -1))
	{
		ft_error("pipex: No such file or directory\n");
		return (1);
	}
	return (0);
}

int	ft_check_close_error(int fd)
{
	if (close(fd) == -1)
	{
		ft_error("Error: close() infile/outfile failed.\n");
		return (1);
	}
	return (0);
}

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
		ft_error("Error: Command not found.\n");
	if (ft_check_close_error((*ptr).fd1))
		return (1);
	return (0);
}

int	ft_create_parent(t_struct *ptr, char **argv, char **envp)
{
	if (ft_parent_process(ptr, argv, envp) == -1)
		ft_error("Error: Command not found.\n");
	if (ft_check_close_error((*ptr).fd2))
		return (1);
	return (0);
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
