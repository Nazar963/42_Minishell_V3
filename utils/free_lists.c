/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_lists.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:07:52 by nakoriko          #+#    #+#             */
/*   Updated: 2025/01/13 22:48:56 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_linked_list(t_token **token)
{
	t_token	*temp;

	if (*token)
	{
		while (*token)
		{
			temp = *token;
			*token = (*token)->next;
			if (temp->content)
			{
				free(temp->content);
				temp->content = NULL;
			}
			if (temp->heredoc_file)
			{
				free(temp->heredoc_file);
				temp->heredoc_file = NULL;
			}
			free(temp);
			temp = NULL;
		}
	}
}

void	free_linked_list_delimeter(t_delimeter **delimeter)
{
	t_delimeter	*temp;

	if (*delimeter)
	{
		while (*delimeter)
		{
			temp = *delimeter;
			*delimeter = (*delimeter)->next;
			free(temp->del);
			free(temp);
		}
	}
}

void	free_orig_linked_list(t_token **token)
{
	t_token	*temp;

	while (*token)
	{
		temp = *token;
		*token = (*token)->next;
		free(temp->content);
		free(temp);
	}
}

void	ft_free_linked_list_until_pipe(t_token **token)
{
	t_token	*temp;

	while (*token)
	{
		if ((*token)->type == TOKEN_PIPE)
			break ;
		temp = *token;
		*token = (*token)->next;
		free(temp->content);
		if (temp->heredoc_file)
			free(temp->heredoc_file);
		free(temp);
	}
	*token = NULL;
}
