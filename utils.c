/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/13 13:59:36 by mbenmesb          #+#    #+#             */
/*   Updated: 2021/11/13 13:59:40 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex.h"

char	*ft_strndup(char *src, int n)
{
	char	*dest;
	int		size_src;
	int		i;

	i = 0;
	size_src = 0;
	while (src[size_src] && i < n)
		size_src++;
	dest = (char *)malloc(sizeof(*dest) * (size_src + 1));
	if (!dest)
		return (NULL);
	while (src[i] && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
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
	return (str);
}
