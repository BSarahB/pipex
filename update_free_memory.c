/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_free_memory.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/13 15:42:37 by mbenmesb          #+#    #+#             */
/*   Updated: 2021/11/13 15:42:40 by mbenmesb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex.h"

void	ft_free_struct_str(char **p)
{
	if (*p != NULL)
	{
		free(*p);
		*p = NULL;
	}
}

void	ft_free_tab(char ***tab)
{
	int	i;

	i = 0;
	while ((*tab)[i])
	{
		ft_free_struct_str(&(*tab)[i]);
		i++;
	}
	if (*tab != NULL)
	{
		free(*tab);
		*tab = NULL;
	}
}

char	*ft_update_string(char **str, char *new)
{
	char	*str_old;

	str_old = *str;
	*str = new;
	ft_free_struct_str(&str_old);
	return (*str);
}

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
