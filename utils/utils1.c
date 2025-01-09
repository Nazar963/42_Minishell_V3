/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:17:21 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/09 14:17:31 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_del_first_node_and_restructure_temp(t_token **token, t_token *temp)
{
	t_token	*tmp;
	t_token	*holder;

	if (token == NULL || *token == NULL)
		return ;
	holder = temp;
	tmp = *token;
	while (temp->next != *token)
		temp = temp->next;
	temp->next = temp->next->next;
	temp = holder;
	if ((*token)->next == NULL)
	{
		*token = (*token)->next;
		*token = NULL;
	}
	else
	{
		*token = (*token)->next;
	}
	free(tmp->content);
	free(tmp);
}

void	ft_del_first_node_delimeter(t_delimeter **delimeter)
{
	t_delimeter	*temp;

	if (delimeter == NULL || *delimeter == NULL)
		return ;
	temp = *delimeter;
	if ((*delimeter)->next == NULL)
	{
		*delimeter = (*delimeter)->next;
		*delimeter = NULL;
	}
	else
	{
		*delimeter = (*delimeter)->next;
	}
	free(temp->del);
	free(temp);
}

void	ft_del_node(t_token **token, t_token *delete)
{
	t_token	*temp;

	if (token == NULL || *token == NULL || delete == NULL)
		return ;
	if (*token == delete)
	{
		*token = delete->next;
		free(delete->content);
		delete->content = NULL;
		free(delete);
		return ;
	}
	temp = *token;
	while (temp->next != NULL && temp->next != delete)
		temp = temp->next;
	temp->next = delete->next;
	free(delete->content);
	delete->content = NULL;
	free(delete);
}

char	*ft_strjoin_mod(char *orig, char *join)
{
	char	*temp;

	temp = orig;
	orig = ft_strjoin(orig, join);
	free(temp);
	return (orig);
}
