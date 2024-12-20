/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakoriko <nakoriko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:58:01 by nakoriko          #+#    #+#             */
/*   Updated: 2024/12/20 16:25:09 by nakoriko         ###   ########.fr       */
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

void	free_linked_list(t_token *token)
{
	t_token	*temp;

	while (token)
	{
		temp = token;
		token = token->next;
		free(temp->content);
		free(temp);
	}
}

void	free_all(t_main *main, t_token **token)
{
	free_mtx(main->env);
	if (*token != NULL)
		free_linked_list(*token);
}

void free_str(char *str)
{
	if (str == NULL)
		return ;
	free(str);
	str = NULL;
}

void free_all_preparsing (t_mtx *data)
{
	if (data->str)
		free_str(data->str);
	if (data->buffer)
		free_str(data->buffer);
	if (data->tokens)
		free_mtx(data->tokens);
}
