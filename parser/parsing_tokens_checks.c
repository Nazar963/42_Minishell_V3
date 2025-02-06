/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tokens_checks.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 12:29:20 by nakoriko          #+#    #+#             */
/*   Updated: 2025/02/06 19:00:55 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_check_for_exit_argument(t_token *token, t_token *prev_token)
{
	int	len;

	len = ft_strlen(prev_token->content);
	if (len == 4 && ft_strncmp(prev_token->content, "exit", len) == 0
		&& !ft_strnstr(token->content, "|", 1)
		&& ft_strlen(token->content) != 1)
	{
		token->type = TOKEN_ARGUMENT;
		return (0);
	}
	return (1);
}

int	first_token_check(t_token *token)
{
	if (token->type == TOKEN_COMMAND && ft_strlen(token->content) == 0)
	{
		print_error(" command not found", NULL, NULL);
		g_global = 127;
		free_linked_list(&token);
		return (1);
	}
	return (0);
}

int	token_option_check(t_token *new_token, t_token *token)
{
	int	i;

	i = 0;
	if (new_token->type == TOKEN_OPTION)
	{
		if (new_token->content[i] == '-')
		{
			i++;
			if (new_token->content[i] == '-')
				i++;
			if (new_token->content[i] >= 'A' && new_token->content[i] <= 'Z')
			{
				print_error(" invalid option", NULL, NULL);
				free_linked_list(&token);
				g_global = 2;
				return (1);
			}
		}
	}
	return (0);
}

void	ft_token_quote(t_mtx *data, char **env_test)
{
	data->quote = data->str[data->i];
	data->buffer[data->j++] = data->str[data->i++];
	while (data->str[data->i] && data->str[data->i] != data->quote)
	{
		if (data->str[data->i] == '$' && data->quote == '\"')
		{
			if (data->str[data->i + 1] == '?')
				ft_expand_global(data);
			else
				ft_expand(data, env_test);
			if (data->check == 1)
				return ;
		}
		else
			data->buffer[data->j++] = data->str[data->i++];
	}
	if (data->str[data->i] != data->quote)
	{
		print_error("Error: Unmatched quote", NULL, NULL);
		g_global = 2;
		data->check = 1;
		return ;
	}
	data->buffer[data->j++] = data->str[data->i++];
}

char	*check_content_quotes(char *str, t_token *token)
{
	int		i;
	int		j;
	char	*new_str;
	char	c;

	(void) token;
	i = 0;
	j = 0;
	new_str = malloc(sizeof(char) * (ft_strlen(str) + 1));
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			c = str[i++];
			while (str[i] && str[i] != c)
				new_str[j++] = str[i++];
			if (str[i] == c)
				i++;
		}
		else
			new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	free_str(str);
	return (new_str);
}
