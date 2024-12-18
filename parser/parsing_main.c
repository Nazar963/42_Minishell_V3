/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakoriko <nakoriko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:43:55 by nakoriko          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/12/19 11:10:39 by nakoriko         ###   ########.fr       */
=======
/*   Updated: 2024/12/19 08:07:15 by naal-jen         ###   ########.fr       */
>>>>>>> 4f46f632774812c7174a687a1f850861461a678e
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//TODO: Separazione tra parsing e tokenizzazione
//TODO: integrare la funziona tokenization direttamente dentro ft_token_generator
//TODO: Inorder to create the right tokens which could be multiple we will need to split
//TODO - the tokens into multiple matrix arrays inorder to reset the proccess of tokenization
//TODO - in case of pipes
//TODO: i modified the ft_lstnew to create a new string from the input string inorder to free tokens at the end.

void preparsing_check_and_split_input(t_mtx *data, char **env)
{
	while (data->i < data->len) //ciclo per creare tokens
	{
		if(data->str[data->i] == ' ') //se c'e spazio, metti token in tokens
			ft_token_space(data);
		else if (data->str[data->i] == '\'' || data->str[data->i] == '\"')
		{
			ft_token_quote(data, env);
			if (data->check == 1)
				return ;
		}
		else if(data->str[data->i] == '$')
		{
			ft_expand(data, env);
			if (data->check == 1)
				return ;
		}
		else if(ft_is_special_char(&data->str[data->i]))
		{
			print_error("Error: Unsupported characters or operator '", &data->str[data->i]);
			data->check = 1;
			return ;
		} 
		// se ci sono operatori (include || e &&)
		else if (data->str[data->i] == '|' || data->str[data->i] == '>' || data->str[data->i] == '<' || data->str[data->i] == '&')
			ft_token_operator(data);
		else
			data->buffer[data->j++] = data->str[data->i++];
	}
}

char **ft_token_generator(char *input, char **env)
{
	t_mtx	data;

	ft_tokendata_init(&data, input);
	if(data.tokens == NULL || data.buffer == NULL)
		return (NULL);
	preparsing_check_and_split_input(&data, env); // spazi, virgolette, operatori
	if (data.check == 1)
	{
		free_all_preparsing(&data);
		return (NULL);
	}
	ft_tokens_finish(&data);
	return (data.tokens);
}



void	first_token_type_assigning(t_token **token)
{
	if (ft_strncmp((*token)->content, "echo", 4) == 0)
		(*token)->type = TOKEN_BUILTIN;
	else if (ft_strncmp((*token)->content, "pwd", 3) == 0)
		(*token)->type = TOKEN_BUILTIN;
	else if (ft_strncmp((*token)->content, "cd", 2) == 0)
		(*token)->type = TOKEN_BUILTIN;
	else if (ft_strncmp((*token)->content, "export", 6) == 0)
		(*token)->type = TOKEN_BUILTIN;
	else if (ft_strncmp((*token)->content, "unset", 5) == 0)
		(*token)->type = TOKEN_BUILTIN;
	else if (ft_strncmp((*token)->content, "env", 3) == 0)
		(*token)->type = TOKEN_BUILTIN;
	else if (ft_strncmp((*token)->content, "exit", 4) == 0)
		(*token)->type = TOKEN_BUILTIN;
	else if (ft_strncmp((*token)->content, "<", 1) == 0)
		(*token)->type = TOKEN_REDIRECTION_IN;
	else if (ft_strncmp((*token)->content, ">", 1) == 0)
		(*token)->type = TOKEN_REDIRECTION_OUT;
	else
		(*token)->type = TOKEN_COMMAND;
}

t_token	*ft_token_list_creation(char **tokens)
{
	t_token	*token;
	int		i;

	i = 0;
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token = ft_lstnew(tokens[i]);
	first_token_type_assigning(&token);
	while (tokens[++i])
	{
		// if (token_node_controll (tokens[i]) == 0)
		ft_lstadd_back(&token, ft_lstnew(tokens[i]));
		// else 
		// {
		// 	free(token);
		// 	token = NULL;
		// 	return(token);
		// }
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
	if (tokens && ft_tokens_check(tokens) == 0)
	{
		print_mtx(tokens, "Tokens");
		token = ft_token_list_creation(tokens);
		if (token == NULL)
			return(NULL);
	}
	else
		return (NULL);
	// while (token)
	// {
	// 	printf("Token: %s\n", token->content);
	// 	printf("Type: %d\n", token->type);
	// 	token = token->next;
	// }
	// exit(0);
	return (token);
}