/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list_creation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:30:02 by nakoriko          #+#    #+#             */
/*   Updated: 2025/02/06 19:04:03 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	assign_the_rest(t_token *token, int n)
{
	int	loco;

	loco = ft_strlen(token->content);
	if (ft_strncmp(token->content, "-", 1) == 0 && loco != 1)
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

void	assign_token_type_norm0(t_token **token, int n)
{
	if (ft_strncmp((*token)->content, ">>", n) == 0 && n == 2)
		(*token)->type = TOKEN_APPEND_OUT;
	else if (ft_strncmp((*token)->content, "<<", n) == 0 && n == 2)
		(*token)->type = TOKEN_HEREDOC;
	else if (ft_strncmp((*token)->content, "<", n) == 0 && n == 1)
		(*token)->type = TOKEN_REDIRECTION_IN;
	else if (ft_strncmp((*token)->content, ">", n) == 0 && n == 1)
		(*token)->type = TOKEN_REDIRECTION_OUT;
	else
		(*token)->type = TOKEN_COMMAND;
}

void	assign_token_type(t_token *token, t_token *prev_token)
{
	int	n;

	n = ft_strlen(token->content);
	if (prev_token && prev_token->type == TOKEN_PIPE)
		assign_token_type_norm0(&token, n);
	else if (!prev_token)
		assign_token_type_norm0(&token, n);
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
	else if (prev_token->type == TOKEN_DELIMITER
		&& ft_strncmp(token->content, "<<", n) != 0
		&& ft_strncmp(token->content, ">", n) != 0
		&& ft_strncmp(token->content, "|", n) != 0
		&& ft_strncmp(token->content, ">>", n) != 0)
		token->type = TOKEN_COMMAND;
	else
		assign_the_rest(token, n);
}

t_token	*ft_first_token_creation(char **tokens, t_token *token,
		int i, t_token *prev_token)
{
	token = ft_lstnew(tokens[i]);
	assign_token_type(token, prev_token);
	token->content = check_content_quotes(token->content, token);
	return (token);
}

t_token	*ft_token_list_creation(char **tokens)
{
	t_token	*token;
	t_token	*new_token;
	t_token	*prev_token;
	int		i;

	token = NULL;
	new_token = NULL;
	prev_token = NULL;
	i = 0;
	token = ft_first_token_creation(tokens, token, i, prev_token);
	if (first_token_check(token) == 1)
		return (NULL);
	prev_token = token;
	while (tokens[++i])
	{
		new_token = ft_lstnew(tokens[i]);
		ft_lstadd_back(&token, new_token);
		if (ft_check_for_exit_argument(new_token, prev_token))
			assign_token_type(new_token, prev_token);
		new_token->content = check_content_quotes(new_token->content, token);
		if (token_option_check(new_token, token) == 1)
			return (NULL);
		prev_token = new_token;
	}
	return (token);
}
