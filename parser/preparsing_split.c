/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preparsing_split.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 13:13:26 by nakoriko          #+#    #+#             */
/*   Updated: 2025/01/26 18:15:27 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_new_token(t_mtx *data)
{
	data->buffer[data->j] = '\0';
	data->arg_count++;
	data->tokens[data->y] = ft_strdup(data->buffer);
	data->y++;
	data->j = 0;
}

void	ft_token_operator(t_mtx *data)
{
	data->twin_char = data->str[data->i];
	if (data->j > 0)
		ft_new_token(data);
	if (data->arg_count >= data->k)
		return ;
	data->buffer[data->j] = data->str[data->i];
	if (data->str[data->i + 1] == data->twin_char)
	{
		if (data->str[data->i] == '<')
			data->delim_flag = 1;
		data->j++;
		data->i++;
		data->buffer[data->j] = data->str[data->i];
	}
	data->buffer[data->j + 1] = '\0';
	data->arg_count++;
	if (data->arg_count >= data->k)
		return ;
	data->tokens[data->y] = ft_strdup(data->buffer);
	data->y++;
	data->j = 0;
	data->i++;
}

void	split_spaces_and_quotes(t_mtx *data, char **env)
{
	if (data->str[data->i] == ' ')
	{
		if (data->j > 0)
			ft_new_token(data);
		data->i++;
	}
	else if (ft_is_tab(data->str[data->i], data->str[data->i + 1]))
	{
		data->buffer[data->j++] = data->str[data->i++];
		data->buffer[data->j++] = data->str[data->i++];
	}
	else if (data->str[data->i] == '\'' || data->str[data->i] == '\"')
		ft_token_quote(data, env);
}

void	split_dollar_and_operators(t_mtx *data, char **env)
{
	if (data->str[data->i] == '$')
	{
		if (data->str[data->i + 1] == '?')
			ft_expand_global(data);
		else if (data->str[data->i + 1] == '\"'
			|| data->str[data->i + 1] == '\'')
			data->i++;
		else if (data->delim_flag == 0)
			ft_expand(data, env);
		else
			data->buffer[data->j++] = data->str[data->i++];
	}
	else if (data->str[data->i] == '|'
		|| data->str[data->i] == '>'
		|| data->str[data->i] == '<')
		ft_token_operator(data);
}

void	preparsing_check_and_split_input(t_mtx *data, char **env)
{
	while (data->i < data->len && data->arg_count < data->k)
	{
		if (data->str[data->i] == ' '
			|| ft_is_tab(data->str[data->i], data->str[data->i + 1])
			|| data->str[data->i] == '\''
			|| data->str[data->i] == '\"')
		{
			split_spaces_and_quotes(data, env);
			if (data->check == 1)
				return ;
		}
		else if (data->str[data->i] == '$'
			|| (data->str[data->i] == '|'
				|| data->str[data->i] == '>'
				|| data->str[data->i] == '<'))
		{
			split_dollar_and_operators(data, env);
			if (data->check == 1)
				return ;
		}
		else
			data->buffer[data->j++] = data->str[data->i++];
	}
}
