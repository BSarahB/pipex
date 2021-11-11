#ifndef PIPEX_H
# define PIPEX_H


#include <stdlib.h> //exit
#include <stdio.h>// perror() stdout stdin etc...
#include <unistd.h> //pipe() fork() execlp("", ", "", NULL)
#include <sys/types.h> //wait() fork()
#include <sys/wait.h> //wait()
#include <errno.h>// perror()
#include <sys/types.h> //open()
#include <sys/stat.h>
#include <fcntl.h>



typedef struct s_struct_pipe
{
	int		fd1;
	int		fd2;
	pid_t	retour;
	//char	*str_psp;
}					t_struct_pipe;

//static	char	*ft_strndup(char *src, int n);
//static	char	**ft_create_tab(char const *s, char c, size_t words_nbr);
//static	size_t	ft_words_nbr(const char *s, char c);
char			**ft_split(char const *s, char c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	**ft_get_path(char **envp);
char	*ft_strcpy(char *dst, char const *src);
char	*ft_init_string(size_t len);
char	*ft_strcat(char *dest, char const *src);
char	*ft_strjoin(char *s1, char const *s2);
void	ft_putstr_fd(char *s, int fd);
void	ft_free_struct_str(char **p);
char	*ft_update_string(char **str, char *new);
void	ft_free_tab(char ***tab);
void	ft_error(char * const str);
t_struct_pipe	*ft_struct_init(t_struct_pipe **ptr);



#endif
