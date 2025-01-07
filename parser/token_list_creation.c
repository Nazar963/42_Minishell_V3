/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list_creation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakoriko <nakoriko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:30:02 by nakoriko          #+#    #+#             */
/*   Updated: 2025/01/07 15:55:48 by nakoriko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	assign_the_rest(t_token *token, int n)
{
	if (ft_strncmp(token->content, "-", 1) == 0)
		token->type = TOKEN_OPTION;
	else if (ft_strncmp(token->content, "<", n) == 0)
		token->type = TOKEN_REDIRECTION_IN;
	else if (ft_strncmp(token->content, ">", n) == 0)
		token->type = TOKEN_REDIRECTION_OUT;
	else if (ft_strncmp(token->content, ">>", n) == 0)
		token->type = TOKEN_APPEND_OUT;
	else if (ft_strncmp(token->content, "<<", n) == 0)
		token->type = TOKEN_HEREDOC;
	else if (ft_strncmp(token->content, "|", n) == 0)
		token->type = TOKEN_PIPE;
	else if (ft_strncmp(token->content, "echo", n) == 0
		|| ft_strncmp(token->content, "cd", n) == 0
		|| ft_strncmp(token->content, "pwd", n) == 0
		|| ft_strncmp(token->content, "export", n) == 0
		|| ft_strncmp(token->content, "unset", n) == 0
		|| ft_strncmp(token->content, "env", n) == 0
		|| ft_strncmp(token->content, "exit", n) == 0)
		token->type = TOKEN_BUILTIN;
	else
		token->type = TOKEN_ARGUMENT;
}

void	assign_token_type(t_token *token, t_token *prev_token)
{
	int	n;

	n = ft_strlen(token->content);
	if (prev_token && prev_token->type == TOKEN_PIPE)
		token->type = TOKEN_COMMAND;
	else if (!prev_token)
	{
		if (ft_strncmp(token->content, ">>", n) == 0)
			token->type = TOKEN_APPEND_OUT;
		else
			token->type = TOKEN_COMMAND;
	}
	else if (prev_token && (prev_token->type == TOKEN_REDIRECTION_IN
			|| prev_token->type == TOKEN_REDIRECTION_OUT
			|| prev_token->type == TOKEN_APPEND_OUT))
		token->type = TOKEN_FILENAME;
	else if (prev_token && prev_token->type == TOKEN_HEREDOC)
	{
		token->type = TOKEN_DELIMITER;
		if (quotes_delimiter(token->content) == 0)
			token->expaned_del = 1;
	}
	else
		assign_the_rest(token, n);
}

t_token	*ft_token_list_creation(char **tokens)
{
	t_token	*token;
	t_token	*new_token;
	t_token	*prev_token;
	int		i;

	new_token = NULL;
	prev_token = NULL;
	i = 0;
	token = ft_lstnew(tokens[i]);
	assign_token_type(token, prev_token);
	token->content = check_content_quotes(token->content, token);
	if (first_token_check(token) == 1)
		return (NULL);
	prev_token = token;
	while (tokens[++i])
	{
		new_token = ft_lstnew(tokens[i]);
		ft_lstadd_back(&token, new_token);
		assign_token_type(new_token, prev_token);
		new_token->content = check_content_quotes(new_token->content, token);
		if (token_option_check(new_token, token) == 1)
			return (NULL);
		prev_token = new_token;
	}
	return (token);
}
