/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 20:44:33 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/20 19:29:55 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//TODO: Posso eliminare il size perche aumento di uno alla volta
char	**ft_realloc(char **mtx, int size)
{
	char	**new_mtx;
	int		i;

	i = 0;
	new_mtx = (char **)malloc(sizeof(char *) * (size + 1));
	if (new_mtx == NULL)
		return (NULL);
	// printf("hello am here\n");
	while (i < size - 1)
	{
		new_mtx[i] = ft_strdup(mtx[i]);
		i++;
	}
	new_mtx[size] = NULL;
	free_mtx(mtx);
	return (new_mtx);
}

int	ft_strlen_mtx(char **mtx)
{
	int	i;

	i = 0;
	while (mtx[i])
		i++;
	return (i);
}

int	ft_control_int(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (ft_isdigit(str[i]) == 1)
			return (1);
	}
	return (0);
}