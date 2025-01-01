/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:58:01 by nakoriko          #+#    #+#             */
/*   Updated: 2025/01/01 16:38:02 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void free_mtx(char **mtx)
{
	int i;

	i = 0;
	if (mtx == NULL)
		return ;
	while (mtx[i] != NULL)
	{
		free(mtx[i]);
		i++;
	}
	free(mtx);
}

void	free_linked_list(t_token **token)
{
	t_token	*temp;

	if (*token)
	{
		while (*token)
		{
			temp = *token;
			*token = (*token)->next;
			free(temp->content);
			if (temp->heredoc_file)
				free(temp->heredoc_file);
			free(temp);
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
			free(temp->delimeter);
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

void	free_all(t_main *main, t_token **token)
{
	free_mtx(main->env);
	if (*token != NULL)
		free_linked_list(token);
	if (main->token != NULL)
		free_linked_list(&main->token);
}

void free_str(char *str)
{
	if (str == NULL)
		return ;
	free(str);
	str = NULL;
}

void free_all_preparsing(t_mtx *data)
{
	if (data->str)
		free_str(data->str);
	if (data->buffer)
		free_str(data->buffer);
	if (data->tokens)
		free_mtx(data->tokens);
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
		free(temp);
	}
	*token = NULL;
}