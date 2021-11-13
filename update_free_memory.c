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
