/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 20:44:33 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/09 17:39:49 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_realloc(char **mtx, int size)
{
	char	**new_mtx;
	int		i;

	i = 0;
	new_mtx = (char **)malloc(sizeof(char *) * (size + 1));
	if (new_mtx == NULL)
		return (NULL);
	while (i < size - 1)
	{
		new_mtx[i] = ft_strdup(mtx[i]);
		i++;
	}
	new_mtx[size] = NULL;
	free_mtx(&mtx);
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
	int	loco;

	i = -1;
	loco = 0;
	while (str[++i])
	{
		if (ft_isdigit(str[i]) == 1)
			return (1);
		else if ((str[i] == '+' || str[i] == '-') && loco == 0)
			loco++;
		else
			return (0);
	}
	return (0);
}

void	ft_del_first_node(t_token **token)
{
	t_token	*temp;

	if (token == NULL || *token == NULL)
		return ;
	temp = *token;
	if ((*token)->next == NULL)
	{
		*token = (*token)->next;
		*token = NULL;
	}
	else
	{
		*token = (*token)->next;
	}
	free(temp->content);
	free(temp);
}
