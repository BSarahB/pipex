/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/13 14:08:29 by mbenmesb          #+#    #+#             */
/*   Updated: 2021/11/18 10:42:50 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_struct
{
	int		fd1;
	int		fd2;
	int		errnum;
	pid_t	retour;
	int		ret_fd1;
	int		p[2];
	char	**path_tab;
}					t_struct;
char		**ft_get_path(char **envp);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		**ft_split(char const *s, char c);
size_t		ft_words_nbr(const char *s, char c);
char		**ft_create_tab(char const *s, char c, size_t words_nbr);
char		*ft_strndup(char *src, int n);
char		*ft_strcpy(char *dst, char const *src);
char		*ft_init_string(size_t len);
char		*ft_strcat(char *dest, char const *src);
char		*ft_strjoin(char *s1, char const *s2);
void		ft_putstr_fd(char *s, int fd);
void		ft_free_t_struct(t_struct **ptr);
void		ft_free_struct_str(char **p);
char		*ft_update_string(char **str, char *new);
void		ft_free_tab(char ***tab);
void		ft_error(char *const str);
t_struct	*ft_struct_init(t_struct **ptr, char **argv);
int			ft_check_open_error(t_struct *ptr);
int			ft_check_close_error(int fd);
int			ft_create_parent(t_struct *ptr, char **argv, char **envp);
int			ft_create_child(t_struct *ptr, char **argv, char **envp);
void		ft_create_pipe(t_struct *ptr);
int			ft_get_child_ret(t_struct *ptr, char **argv, char **envp);
int			ft_get_parent_ret(t_struct *ptr, char **argv, char **envp);
int			ft_parent_process(t_struct *ptr, char **argv, char **envp);
int			ft_child_process(t_struct *ptr, char **argv, char **envp);
int			ft_execve_join(t_struct *ptr, char **envp, char **argv_cmd);
char		**ft_get_argv_cmd(int i, char **argv);
void		ft_error_msg(char **argv);

#endif
