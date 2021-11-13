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

char	**ft_create_tab(char const *s, char c, size_t words_nbr)
{
	char		**tab;
	size_t		i;
	size_t		k;
	size_t		wd_length;

	i = 0;
	k = 0;
	tab = malloc(sizeof(*tab) * (words_nbr + 1));
	if (!tab)
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

size_t	ft_words_nbr(const char *s, char c)
{
	size_t	i;
	size_t	k;
	size_t	words_nbr;

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

char	**ft_split(char const *s, char c)
{
	char	**tab;
	size_t	words_nbr;

	if (!s)
		return (NULL);
	words_nbr = ft_words_nbr(s, c);
	tab = ft_create_tab(s, c, words_nbr);
	return (tab);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

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
