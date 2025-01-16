/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:58:01 by nakoriko          #+#    #+#             */
/*   Updated: 2025/01/16 18:40:17 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_mtx(char ***mtx)
{
	int	i;

	i = 0;
	if ((*mtx) == NULL)
		return ;
	if (!(*mtx))
		return ;
	if (!(*mtx)[i])
	{
		free((*mtx));
		return ;
	}
	while ((*mtx)[i] != NULL)
	{
		free_str((*mtx)[i]);
		(*mtx)[i] = NULL;
		i++;
	}
	if (*mtx)
		free((*mtx));
	(*mtx) = NULL;
}

void	free_all(t_main *main, t_token **token)
{
	if (main->env)
		free_mtx(&main->env);
	if (*token != NULL)
		free_linked_list(token);
	if (main->token != NULL)
		free_linked_list(&main->token);
}

void	free_str(char *str)
{
	if (str == NULL)
		return ;
	free(str);
	str = NULL;
}

void	free_all_preparsing(t_mtx *data)
{
	if (data->str)
		free_str(data->str);
	if (data->buffer)
		free_str(data->buffer);
	if (data->tokens)
		free_mtx(&data->tokens);
}

void	free_token_and_input(char *input, t_token **token)
{
	free_linked_list(token);
	free_str(input);
}
