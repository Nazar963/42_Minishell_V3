/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakoriko <nakoriko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:43:55 by nakoriko          #+#    #+#             */
/*   Updated: 2025/01/03 18:23:49 by nakoriko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void preparsing_check_and_split_input(t_mtx *data, char **env)
{
	while (data->i < data->len && data->arg_count < data->k) //ciclo per creare tokens
	{
		if(data->str[data->i] == ' ') //se c'e spazio, metti token in tokens
			ft_token_space(data);
		else if (ft_is_tab(data->str[data->i], data->str[data->i + 1]))
		{
			data->buffer[data->j++] = data->str[data->i++];
			data->buffer[data->j++] = data->str[data->i++];
		}
		else if (data->str[data->i] == '\'' || data->str[data->i] == '\"')
		{
			ft_token_quote(data, env);
			if (data->check == 1)
				return ;
		}
		else if(data->str[data->i] == '$')
		{
			if (data->str[data->i + 1] == '?')
				ft_expand_global(data);
			else if (data->str[data->i + 1] == '\"' || data->str[data->i + 1] == '\'')
				data->i++;
			else if (data->delim_flag == 0)
				ft_expand(data, env);
			else
				data->buffer[data->j++] = data->str[data->i++];
			if (data->check == 1)
				return ;
		}
		// se ci sono operatori (include || e &&)
		else if (data->str[data->i] == '|' || data->str[data->i] == '>' || data->str[data->i] == '<')
			ft_token_operator(data);
		else
			data->buffer[data->j++] = data->str[data->i++];
	}
}

char	**ft_token_generator(char *input, char **env)
{
	t_mtx	data;

	ft_tokendata_init(&data, input);
	if(data.tokens == NULL || data.buffer == NULL)
		return (NULL);
	preparsing_check_and_split_input(&data, env); // spazi, virgolette, operatori
	if (data.check == 1 || data.arg_count >= data.k)
	{
		free_all_preparsing(&data);
		return (NULL);
	}
	ft_tokens_finish(&data);
	//print_mtx(data.tokens, "token N");
	return (data.tokens);
}


int	is_a_builtin_check(t_token *token)
{
	int	n;
	
	n = ft_strlen(token->content);
	if (ft_strncmp(token->content, "echo", n) == 0
		|| ft_strncmp(token->content, "cd", n) == 0
		|| ft_strncmp(token->content, "pwd", n) == 0
		|| ft_strncmp(token->content, "export", n) == 0
		|| ft_strncmp(token->content, "unset", n) == 0
		|| ft_strncmp(token->content, "env", n) == 0 // e invironment? Siamo sicuri che token->content e qquesto? da capire un attimo
		|| ft_strncmp(token->content, "exit", n)== 0)
		return (1);
	return (0);
}

int quotes_delimiter(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			return(1);
		i++;
	}
	return(0);
}

// AGGIUNTO PER ASSEGNARE IL TIPO AL NODO PASSATO 
void	assign_token_type(t_token *token, t_token *prev_token)
{
	int	n;

	n = ft_strlen(token->content);
	if (prev_token && prev_token->type == TOKEN_PIPE)
		token->type = TOKEN_COMMAND; // dopo pipe sempre  commanda
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
		token->type = TOKEN_FILENAME; // dopo questi solo filename
	else if (prev_token && prev_token->type == TOKEN_HEREDOC)
	{
		token->type = TOKEN_DELIMITER;
		if (quotes_delimiter(token->content) == 0)//se non ci sono virgolette
			token->expaned_del = 1;
	}
	else if (ft_strncmp(token->content, "-", 1) == 0)
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
	else if (is_a_builtin_check(token) == 1)
		token->type = TOKEN_BUILTIN;
	else
		token->type = TOKEN_ARGUMENT;
}



t_token	*ft_token_list_creation(char **tokens)
{
	t_token	*token;
	t_token *new_token;
	t_token *prev_token;
	int		i;

	new_token = NULL;
	prev_token = NULL;
	i = 0;
	// token = malloc(sizeof(t_token));
	// if (!token)
	// 	return (NULL);
	token = ft_lstnew(tokens[i]);
	assign_token_type(token, prev_token); // per caso se input 'e ''
	//if (token->type != TOKEN_COMMAND)
	token->content = check_content_quotes(token->content);
	//first_token_type_assigning(&token);
	prev_token = token;
	while (tokens[++i])
	{
		new_token = ft_lstnew(tokens[i]);
		ft_lstadd_back(&token, new_token);
		assign_token_type(new_token, prev_token);
		//if (new_token->type != TOKEN_COMMAND) // per caso se input 'e ''
		new_token->content = check_content_quotes(new_token->content);
		prev_token = new_token;
	}
	return (token);
}

t_token	*ft_tokenizer_main(char *input, t_main *main)
{
	t_token	*token;
	char	**tokens;
	
	(void)main;
	token = NULL;
	tokens = ft_token_generator(input, main->env);
	if (tokens!= NULL && tokens[0] && ft_tokens_check(tokens) == 0)
	{
		//print_mtx(tokens, "Tokens");
		token = ft_token_list_creation(tokens);
		free_mtx(tokens);
		if (token == NULL)
		//controllare se devo freeare
			return (NULL);
		// else 
		// 	print_token_list(token);
	}
	else
		return (free_mtx(tokens), NULL);
	return (token);
}
