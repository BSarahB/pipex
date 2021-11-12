#include "pipex.h"

void	ft_free_t_struct_pipe(t_struct_pipe **ptr)
{
	//ft_free_struct_str(&((*ptr)->str_psp));
	//ft_free_struct_str(&((*ptr)->str_sign));
	//ft_free_struct_str(&((*ptr)->str_diez));
	//ft_free_struct_str(&((*ptr)->str_0_p));
	//ft_free_struct_str(&((*ptr)->str_sp_g));
	//ft_free_struct_str(&((*ptr)->str_0fil_g));
	free(*ptr);
	*ptr = NULL;
}


t_struct_pipe	*ft_struct_init(t_struct_pipe **ptr, char **argv)//t_struct_pipe	*ft_struct_init(t_struct_pipe **ptr, char init_value)
{
	*ptr = (t_struct_pipe *)malloc(sizeof(t_struct_pipe));
	if (!(*ptr))
		return (0);
	(*ptr)->fd1 = open(argv[1], O_RDONLY);
	(*ptr)->fd2 = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	(*ptr)->retour = 0;


	//(*ptr)->str_psp = ft_init_cstring(&((*ptr)->str_psp), 0, init_value);

	return (*ptr);
}

int ft_check_open_error(t_struct_pipe *ptr)
{
	if (((*ptr).fd1 == -1) || ((*ptr).fd2 == -1))
		{
		ft_error("pipex: No such file or directory\n");//avant : open()infile/outfile failed

		//ft_putstr_fd("pipex: No such file or directory\n", 1);//avant : open()infile/outfile failed
			//	ft_free_t_struct_pipe(&ptr);

			return (1);
		}
	return (0);
}
int ft_check_close_error(int fd)
{
	if (close(fd) == -1)
	{
		ft_error("Error: close() infile/outfile failed.\n");
		//	ft_free_t_struct_pipe(&ptr);

		return (1);
	}
	return (0);
}


char **ft_get_argv_cmd(int i,char **argv)
{
	char **argv_cmd;
	char slash[2];
	slash[0] = '/';
	slash[1] = 0;

	argv_cmd = ft_split(argv[i], ' ');
	ft_update_string(&argv_cmd[0],ft_strjoin(slash, argv_cmd[0]));
	return(argv_cmd);
}


int	ft_child_process(t_struct_pipe *ptr, char **argv, char **path_tab, char **envp)
{
	char *path_cmd1_joined;
	int j;
	char **argv_cmd1;
	int exec_return;

	exec_return = 0;
	j = -1;
	argv_cmd1 = ft_get_argv_cmd(2, argv);
	close (STDOUT_FILENO);
	dup2((*ptr).fd1, STDIN_FILENO);
	(void)dup((*ptr).p[1]);
	close ((*ptr).p[1]);
	close ((*ptr).p[0]);
	while (path_tab[++j])
	{
		path_cmd1_joined = ft_strjoin(path_tab[j], argv_cmd1[0]);
		exec_return = execve(path_cmd1_joined, argv_cmd1, envp);		//perror("Error");
		free(path_cmd1_joined);		
		if(exec_return != -1)// Respecter la priorité du path, seule la 1ere commande est exec
			break;
	}
	ft_free_tab(&argv_cmd1);
	return (exec_return);
}
int	ft_parent_process(t_struct_pipe *ptr, char **argv, char **path_tab, char **envp)
{
	char *path_cmd2_joined;
	int i;
	char **argv_cmd2;
	int exec_return;

	exec_return = 0;
	i = -1;
	argv_cmd2 = ft_get_argv_cmd(3, argv);
	close (STDIN_FILENO);
	(void)dup((*ptr).p[0]);
	close ((*ptr).p[0]);
	close ((*ptr).p[1]);
	dup2((*ptr).fd2, STDOUT_FILENO);
	while (path_tab[++i])
	{
		path_cmd2_joined = ft_strjoin(path_tab[i], argv_cmd2[0]);//usr/bin/ls
		exec_return = execve(path_cmd2_joined, argv_cmd2, envp);//perror("Error");
		free(path_cmd2_joined);
		if(exec_return != -1)// Respecter la priorité du path, seule la 1ere commande est exec
			break;
	}
	ft_free_tab(&argv_cmd2);
	return (exec_return);
}

int	ft_create_child(t_struct_pipe *ptr, char **argv, char **path_tab, char **envp)
{
	if (ft_child_process(ptr, argv, path_tab, envp) == -1)
		ft_error("Error: Command not found.\n");// ATTENDU sur terminal ->$pipex: command not found: not-executable//soit argv[3]
	if (ft_check_close_error((*ptr).fd1))
		return(1);
	return(0);
}

int ft_create_parent(t_struct_pipe *ptr, char **argv, char **path_tab, char **envp)
{
	if (ft_parent_process(ptr, argv, path_tab, envp) == -1)
		ft_error("Error: Command not found.\n");
	if (ft_check_close_error((*ptr).fd2))
		return(1);
	return(0);
}

void	ft_create_pipe(t_struct_pipe *ptr)
{
	if(pipe((*ptr).p))
	{
		ft_free_t_struct_pipe(&ptr);
		perror("pipe");
		exit(0);
	}
}
int	ft_check_fork(t_struct_pipe *ptr, char **argv, char **envp)
{
	char **path_tab;

	path_tab = ft_get_path(envp);
	if((*ptr).retour == -1)
	{
		ft_free_tab(&path_tab);
		ft_free_t_struct_pipe(&ptr);
		perror (" pb fork ");
		exit(1);
	}
	if((*ptr).retour == 0)
	{
		if(ft_create_child(ptr, argv, path_tab, envp))
			{
				ft_free_tab(&path_tab);
				return(1);
			}
	}
	else {
		if(ft_create_parent(ptr, argv, path_tab, envp))
			{
				ft_free_tab(&path_tab);
				return(1);
			}
		}
	ft_free_tab(&path_tab);
	return (0);
}

int main(int argc, char *argv[], char *envp[])
{
	t_struct_pipe	*ptr;

	if (argc < 5)
		{
			ft_putstr_fd("expected: ./pipex infile cmd1 cmd2 outfile \n", 1);
			return (1);
		}
	ptr = ft_struct_init(&ptr, argv);
	if (ft_check_open_error(ptr))//param: (ptr)(on recupere le fd dans l check_open())
	{
		ft_free_t_struct_pipe(&ptr);
		return(1);
	}
	ft_create_pipe(ptr);
	(*ptr).retour = fork();
	if (ft_check_fork(ptr, argv, envp))
	{
		ft_free_t_struct_pipe(&ptr);
		return(1);
	}
	ft_free_t_struct_pipe(&ptr);
	return (0);
}





























//**************************************************************************
/*


int main(int argc, char *argv[], char *envp[])
{
	int i;
	int j;
	(void)argc;
	(void)argv;
	//(void)envp;[]
	int p[2]; //array pour accueillir les 2 fd
	pid_t retour;
	char **path_tab;
	char **argv_cmd2;
	char **argv_cmd1;
//	char *cmd2_parent2[] = { "/usr/bin/wc", "-l", NULL };
//	char **cmd1_child;
//	char *cmd1_child1[] = { "/usr/bin/ls", "-l", NULL };//char *newargv[] = {"/usr/bin/ls", "-l", NULL};
//	char *path_cmd1_joined;
	char *path_cmd2_joined;
	char *path_cmd1_joined;
	char slash[2];
	int fd1;
	fd1 = open(argv[1], O_RDONLY);
	int fd2;
	fd2 = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);

	slash[0] = '/';
	slash[1] = 0;
	i = -1;
	j = -1;
// ouverture d’un tube
	if(pipe(p))
	{
		perror("pipe");
		exit(0);
	}

	retour = fork(); //le pere cree le fils , on remplace le switch case -1, 0, default
	if(retour == -1) // erreur
	{
		perror (" pb fork ");
		exit(1);
	}
	//ON RECUPERE LE TABLEAU DE PATH AVEC LES ADRESSES DEDANS
	path_tab = ft_get_path(envp);

	//ON RECUPERE LA COMMANDE AVEC LES OPTIONS ex : {"ls", "-la"}
	//argv_cmd2 =  ft_split("wc -l", ' ');
	//argv_cmd1 =  ft_split("ls -l", ' ');

	argv_cmd2 =  ft_split(argv[3], ' ');

	argv_cmd1 = ft_split(argv[2], ' ');
	argv_cmd2[0] = ft_strjoin(slash, argv_cmd2[0]);
	argv_cmd1[0] = ft_strjoin(slash, argv_cmd1[0]);

//cmd2_parent = ft_split(argv[3], ' ');
//path_cmd2_joined = ft_strjoin(path_tab[0], argv_cmd2[0]);// /.../.../wc


	if(retour == 0) // ON EST DANS LE CHILD PROCESS le processus fils exécute la commande ls // la sortie standard du processus est redirigée sur le tube
	{
		close (STDOUT_FILENO);
		dup2(fd1, STDIN_FILENO); //TODO man dup2 pr check return et mettre une protection if si necessaire

		(void)dup(p[1]); // p[1] sortie standard du processus -> stdout devient le synonyme de p[1]
		close (p[1]);
		close (p[0]); // le processus ne lit pas dans le tube

		//execve(cmd1_child1[0], cmd1_child1, envp);

		while (path_tab[++j])
		{
			path_cmd1_joined = ft_strjoin(path_tab[j], argv_cmd1[0]);
			//path_cmd2_joined = ft_strjoin(path_tab[0], argv_cmd2[0]);
			execve(path_cmd1_joined, argv_cmd1, envp);
			perror("Error"); //-->nous permet de tester les erreurs (path joined pas bon pour l execve)
			free(path_cmd1_joined);
		}
	//	while (path_tab[i])
	//	{
		//	path_cmd1_joined = ft_strjoin(path_tab[i], cmd1_child[0]);
			//execve (path_cmd1_joined, cmd1_child, envp);
			//i++;

		//}
		close(fd1);

	}


	//PAR DEFAUT:
	// le processus père exécute la commande wc -l
	// l’entrée standard du processus est redirigée sur le tube
	else {
		close (STDIN_FILENO);
		(void)dup(p[0]); // p[1] sortie standard du processus
		close (p[0]);
		close (p[1]);
		dup2(fd2, STDOUT_FILENO); //TODO man dup2 pr check return et mettre une protection if si necessaire

		//execlp ("wc", "wc", "-l", NULL);
		//execve(path_cmd2_joined, cmd2_parent2, envp);
		//execve(cmd2_parent2[0], cmd2_parent2, envp);
		//execve(newargv[0], newargv, envp);
		while (path_tab[++i])
		{
			path_cmd2_joined = ft_strjoin(path_tab[i], argv_cmd2[0]);
			//path_cmd2_joined = ft_strjoin(path_tab[0], argv_cmd2[0]);
			execve(path_cmd2_joined, argv_cmd2, envp);
			perror("Error");// -->nous permet de tester les erreurs (path joined pas bon pour l execve)
			free(path_cmd2_joined);
		}
		close(fd2);
}
return 0;
}



*/
/*
#include <stdlib.h> //exit
#include <stdio.h>// perror() stdout stdin etc...
#include <unistd.h> //pipe() fork() execlp("", ", "", NULL)
#include <sys/types.h> //wait() fork()
#include <sys/wait.h> //wait()
#include <errno.h>// perror()

static	char	*ft_strndup(char *src, int n)
{
	char	*dest;
	int		size_src;
	int		i;

	i = 0;
	size_src = 0;
	while (src[size_src] && i < n)
		size_src++;
	if (!(dest = (char*)malloc(sizeof(*dest) * (size_src + 1))))
		return (NULL);
	while (src[i] && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

static	char	**ft_create_tab(char const *s, char c, size_t words_nbr)
{
	char		**tab;
	size_t		i;
	size_t		k;
	size_t		wd_length;

	i = 0;
	k = 0;
	if (!(tab = malloc(sizeof(*tab) * (words_nbr + 1))))
		return (NULL);
	while (k < words_nbr)
	{
		while (s[i] && s[i] == c)
			i++;
		wd_length = 0;
		while ((s[i + wd_length]) && (s[i + wd_length]) != c)
			wd_length++;
		tab[k] = ft_strndup((char *)&s[i], wd_length);
		k++;
		i = i + wd_length;
	}
	tab[k] = 0;
	return (tab);
}

static	size_t	ft_words_nbr(const char *s, char c)
{
	size_t i;
	size_t k;
	size_t words_nbr;

	i = 0;
	words_nbr = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		k = 0;
		while (s[i + k] && (s[i + k]) != c)
			k++;
		if (k != 0)
			words_nbr++;
		i = i + k;
	}
	return (words_nbr);
}

char			**ft_split(char const *s, char c)
{
	char		**tab;
	size_t		words_nbr;

	if (!s)
		return (NULL);
	words_nbr = ft_words_nbr(s, c);
	tab = ft_create_tab(s, c, words_nbr);
	return (tab);
}
int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t i;

	i = 0;
	if (n == 0)
		return (0);
	while (i < n - 1 && s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
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
char	*ft_strcpy(char *dst, char const *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	*ft_init_string(size_t len)
{
	char	*str;
	size_t	i;

	i = 0;
	str = (char *)malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	if (len > 0)
	{
		while (i < len)
			str[i++] = 0;
	}
	(str)[len] = '\0';
	return (str);
}
char	*ft_strcat(char *dest, char const *src)
{
	int	i;
	int	l;

	i = 0;
	l = 0;
	while (dest[l] != '\0')
		l++;
	while (src[i] != '\0')
	{
		dest[l + i] = src[i];
		i++;
	}
	dest[l + i] = '\0';
	return (dest);
}

char	*ft_strjoin(char *s1, char const *s2)
{
	char	*str;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s1[i])
		i++;
	while (s2[j])
		j++;
	str = ft_init_string(i + j);
	if (!s1 || !s2 || !str)
		return (NULL);
	str = ft_strcat(ft_strcpy(str, s1), s2);
	free(s1);
	return (str);
}

int main(int argc, char *argv[], char *envp[])
{
	int i;
	(void)argc;
	(void)argv;
	//(void)envp;[]
	int p[2]; //array pour accueillir les 2 fd
	pid_t retour;
	char **path_tab;
	char **argv_cmd2;
	char *cmd2_parent2[] = { "/usr/bin/wc", "-l", NULL };
//	char **cmd1_child;
	char *cmd1_child1[] = { "/usr/bin/ls", "-l", NULL };//char *newargv[] = {"/usr/bin/ls", "-l", NULL};

//	char *path_cmd1_joined;
	char *path_cmd2_joined;
	i = -1;
// ouverture d’un tube
	if(pipe(p))
	{
		perror("pipe");
		exit(0);
	}

	retour = fork(); //le pere cree le fils , on remplace le switch case -1, 0, default
	if(retour == -1) // erreur
	{
		perror (" pb fork ");
		exit(1);
	}
	//ON RECUPERE LE TABLEAU DE PATH AVEC LES ADRESSES DEDANS
	path_tab = ft_get_path(envp);

	//ON RECUPERE LA COMMANDE AVEC LES OPTIONS ex : {"ls", "-la"}
	argv_cmd2 = ft_split("wc -l", ' ');
//cmd2_parent = ft_split(argv[3], ' ');
path_cmd2_joined = ft_strjoin(path_tab[0], argv_cmd2[1]);

	if(retour == 0) // ON EST DANS LE CHILD PROCESS le processus fils exécute la commande ls // la sortie standard du processus est redirigée sur le tube
	{
		close (STDOUT_FILENO);
		(void)dup(p[1]); // p[1] sortie standard du processus -> stdout devient le synonyme de p[1]
		close (p[1]);
		close (p[0]); // le processus ne lit pas dans le tube

		execve(cmd1_child1[0], cmd1_child1, envp);

	//	while (path_tab[i])
	//	{
		//	path_cmd1_joined = ft_strjoin(path_tab[i], cmd1_child[0]);
			//execve (path_cmd1_joined, cmd1_child, envp);
			//i++;

		//}
	}


	//PAR DEFAUT:
	// le processus père exécute la commande wc -l
	// l’entrée standard du processus est redirigée sur le tube
	else {
		close (STDIN_FILENO);
		(void)dup(p[0]); // p[1] sortie standard du processus
		close (p[0]);
		close (p[1]);
		//execlp ("wc", "wc", "-l", NULL);
		execve(cmd2_parent2[0], cmd2_parent2, envp);
		//execve(newargv[0], newargv, envp);
		//while (path_tab[++i])
		//{
			//path_cmd2_joined = ft_strjoin(path_tab[i], argv_cmd2[0]);
			//execve (path_cmd2_joined, argv_cmd2, envp);
			//perror("Error");
			//free(path_cmd2_joined);

		//}

}

}
*/
/*

#include <stdlib.h> //exit
#include <stdio.h>// perror() stdout stdin etc...
#include <unistd.h> //pipe() fork() execlp("", ", "", NULL)
#include <sys/wait.h> //wait()
#include <errno.h>// perror()
#include <sys/types.h>//wait() fork() open()
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	int p[2]; //array pour accueillir les 2 fd
	pid_t retour;
	//int fd1;
	//int fd2;
	char *exec_argvs[] = {"ls", "-la", NULL};
		printf("allez zidane");

	//fd1 = open(argv[1], O_RDONLY);
	//fd2 = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
// ouverture d’un tube
	if(pipe(p))
	{
		perror("pipe");
		exit(0);
	}
	retour = fork(); //le pere cree le fils , on remplace le switch case -1, 0, default
	if(retour == -1) // erreur
	{
		perror (" pb fork ");
		exit(1);
	}
	// I) FILS
	if(retour == 0) // ON EST DANS LE CHILD PROCESS le processus fils exécute la commande ls // la sortie standard du processus est redirigée sur le tube
	{
		close (STDOUT_FILENO);
		//dup2(fd1, STDIN_FILENO); //TODO man dup2 pr check return et mettre une protection if si necessaire
		(void)dup(p[1]); // p[1] sortie standard du processus ->
		close (p[1]);
		close (p[0]); // le processus ne lit pas dans le tube
		//close (fd1);

		//LIER LA CMD1 au PATH DE L ENVIRONNEMENT (afin d adapter en fonction des variables d environnement contenues dans chaque programme)
		// 1) extraire le PATH depuis le double tab envp ->ft_substr
		//2) QESTION NANI : EST CE NECESSAIRE DE PARSER LA PATH_VAR .... avec separateur  " : " entre chaque chemin ? la fonction execve n est pas censee la faire toute seule?  qui contient eventuellement la commande a executer . usr/bin etc etc ->ft_split avec separator :
		//3) split nous renvoie normalement un **tab on va
		//QUESTION NANI POURQUOI     cmd = ft_join(mypaths[i], ag[2]); on fait la jointure /usr/bin/wc, adress de cmds(ls -la) double tab qui contient la commande et l option
		// pour etre comme adress du ** cmd + option (exemple : ls -la) double tab qui contient la commande et l optionadress du ** cmd + option (exemple : ls -la) double tab qui contient la commande et l optionadress du ** cmd + option (exemple : ls -la) double tab qui contient la commande et l option
		// comme le prototype : int execve(const char *filename, char *const argv[], char *const envp[]);

		execve("ls", exec_argvs, envp);
		printf("allez zidane");
		perror("ERROR");
		//free(cmd) ; -> on fera un fonction generique pour get the command.(cmd1 et cmd2)
		// focntion gernerique	qui aura un return (EXIT_FAILURE);

		//execlp ("wc", "wc", "-l", NULL); //cmd1 doit etre associee au fils
		exit(EXIT_FAILURE);

	}
	// II) PERE      PAR DEFAUT:
	// le processus père exécute la commande wc -l
	// l’entrée standard du processus est redirigée sur le tube
	else {

		//TODO waitpid(-1, &status, 0); //attendre que le fils ait fini
		//dup2(fd2, STDOUT_FILENO); // (fd2 = open de outfile) outfile  devient stdout
		close (STDIN_FILENO);
		(void)dup(p[0]); // p[1] sortie standard du processus
		close (p[0]);
		close (p[1]);
		execlp ("wc", "wc", "-l", NULL); //cmd 2 doit etre associee au pere
	}
}



































//TODO MANIPULER DANS LE TERMINAL env et $VAR cf exemples de Nani


//PSEUDO CODE


les tubes et le shell : Redirection des descripteurs standards via la primitive dup()

						$> ./pipex infile ``ls -l'' ``wc -l'' outfile
doit etre equivalent a 	$>  < infile ls -l | wc -l > outfile

						$> ./pipex infile ``grep a1'' ``wc -w'' outfile
should be the same as	$>	< infile grep a1 | wc -w > outfile


//tt d abord, < feinte l entree standard stdin pour y mettre infile . on a rediriger 1 fichier sur son entree standard ca dit va dans le ficher

1) CREER LE TUBE

2) CREER LE PROCESSUS FILS
CHARPENTE :
->si erreur perror + exit()
-> cas ou tout se passe bien : return 0 -> I) LE PROCESSUS FILS EXECUTE LA COMMANDE 1
-> default      -> II) LE PROCESSUS PERE EXECUTE LA COMMANDE 2

3) CONFIGURER LE TUBE (unidirectionnel? bidirectionnel? fermer ce quil faut, lecture/ecriture inutile etc)

4) REDIRIGER LA SORTIE DE CMD1 (sur l entree du tube) ET LENTREE DE CMD2 (a la sortie du tube): SOIT REDIRIGER LES DESCRIPTEURS STANDARDS
			{ pour cela :
						1)identifier quelle est la commande shell et l associer a infile ou outfile
						2)identifier ou sont ecrites/lues NATURELLEMENT les donnees (STDIN, STDOUT)
						3)fermer le descripteur standard correspondant
						4) associer le descripteur standard au descritpeur de tube et mettre le standard synonyme du descripteur de tube (->fr appel a la primitive dup (ac comme parametre le descripteur du tube) afin d
						5)execlp (interdite.) pour integrer la commande  cmd1 au fils , cmd2 au pere.... -> NOUS ALLONS PROCEDER AVEC EXECVE()



Commande ls –l | wc -l

< infile cmd1 -----> il faut que cmd1 (du processus fils ) s execute sur infile

cmd2 > outfile ------> a besoin de  waitpid() at the very beginning to wait for the child to finish her process. TODO man waitpid

execve () efface (initialise nouveau) la stack la heap la data et les variables au tt debut.
execlp () ^ int execlp(const char *file, const char *arg, ...
						(char  *) NULL
fork () les duplique. (c est quasiment l inverse)



#include <stdlib.h> //exit
#include <stdio.h>// perror() stdout stdin etc...
#include <unistd.h> //pipe() fork() execlp("", ", "", NULL)
#include <sys/wait.h> //wait()
#include <errno.h>// perror()
#include <sys/types.h>//wait() fork() open()
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	int p[2]; //array pour accueillir les 2 fd
	pid_t retour;
	int fd1;
	int fd2;

	fd1 = open(argv[1], O_RDONLY);
	fd2 = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
// ouverture d’un tube
	if(pipe(p))
	{
		perror("pipe");
		exit(0);
	}
	retour = fork(); //le pere cree le fils , on remplace le switch case -1, 0, default
	if(retour == -1) // erreur
	{
		perror (" pb fork ");
		exit(1);
	}
	// I) FILS
	if(retour == 0) // ON EST DANS LE CHILD PROCESS le processus fils exécute la commande ls // la sortie standard du processus est redirigée sur le tube
	{
		close (STDOUT_FILENO);
		dup2(fd1, STDIN_FILENO); //TODO man dup2 pr check return et mettre une protection if si necessaire
		(void)dup(p[1]); // p[1] sortie standard du processus ->
		close (p[1]);
		close (p[0]); // le processus ne lit pas dans le tube
		close (fd1);

		//LIER LA CMD1 au PATH DE L ENVIRONNEMENT (afin d adapter en fonction des variables d environnement contenues dans chaque programme)
		// 1) extraire le PATH depuis le double tab envp ->ft_substr
		//2) QESTION NANI : EST CE NECESSAIRE DE PARSER LA PATH_VAR .... avec separateur  " : " entre chaque chemin ? la fonction execve n est pas censee la faire toute seule?  qui contient eventuellement la commande a executer . usr/bin etc etc ->ft_split avec separator :
		//3) split nous renvoie normalement un **tab on va
		//QUESTION NANI POURQUOI     cmd = ft_join(mypaths[i], ag[2]); on fait la jointure /usr/bin/wc, adress de cmds(ls -la) double tab qui contient la commande et l option
		// pour etre comme adress du ** cmd + option (exemple : ls -la) double tab qui contient la commande et l optionadress du ** cmd + option (exemple : ls -la) double tab qui contient la commande et l optionadress du ** cmd + option (exemple : ls -la) double tab qui contient la commande et l option
		// comme le prototype : int execve(const char *filename, char *const argv[], char *const envp[]);
		if(execve( , , ))== -1)
		{

		perror("execve error");
		}
			execve(void);
		//perror("ERROR");
		//free(cmd) ; -> on fera un fonction generique pour get the command.(cmd1 et cmd2)
		// focntion gernerique	qui aura un return (EXIT_FAILURE);

		//execlp ("wc", "wc", "-l", NULL); //cmd1 doit etre associee au fils
		exit(EXIT_FAILURE);

	}
	// II) PERE      PAR DEFAUT:
	// le processus père exécute la commande wc -l
	// l’entrée standard du processus est redirigée sur le tube
	else {

		//TODO waitpid(-1, &status, 0); //attendre que le fils ait fini
		dup2(fd2, STDOUT_FILENO); // (fd2 = open de outfile) outfile  devient stdout
		close (STDIN_FILENO);
		(void)dup(p[0]); // p[1] sortie standard du processus
		close (p[0]);
		close (p[1]);
		execlp ("ls", "ls", "-la", NULL); //cmd 2 doit etre associee au pere
	}
}
*/
