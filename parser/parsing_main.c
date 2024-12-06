/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:43:55 by nakoriko          #+#    #+#             */
/*   Updated: 2024/12/06 15:56:28 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//TODO: Separazione tra parsing e tokenizzazione
//TODO: integrare la funziona tokenization direttamente dentro ft_token_generator
//TODO: Inorder to create the right tokens which could be multiple we will need to split
//TODO - the tokens into multiple matrix arrays inorder to reset the proccess of tokenization
//TODO - in case of pipes
//TODO: i modified the ft_lstnew to create a new string from the input string inorder to free tokens at the end.


char **ft_token_generator(char *input)
{
	char **tokens;
	char *token;
	int len;
	int i;  // contatore di input
	int t_count;//contatore di stringhe dentro char **tokens
	int t_index; // contatore di caratteri dentro la stringa token
	char quote; // per tenere virgolette ed apici
	char twin_char;
	int j;
	char *clean_input;

	j = 0;
	i = 0;
	t_count = 0;
	t_index = 0;
	if (input == NULL)
		return (NULL);
	clean_input = remove_begin_end_whitespaces(input); // ft_strtrim, spazi dal inizio e alla fine
	len = ft_strlen(clean_input);

	//allocazione di memoria per tokens
	tokens = malloc(sizeof(char *) * 256);//massima quantita di argomenti, visto che non possiamo usare realloc
	if (!tokens)
		return (NULL);
	while (j < 256)
	{
		tokens[j] = NULL;
		j++;
	}
	token = malloc(sizeof(char) * len + 1);
	if (token == NULL)
	{
		free(tokens);
		return (NULL);
	}

	//ciclo per creare tokens
	while (i < len)
	{
		//se c'e spazio, metti token in tokens
		if (clean_input[i] == ' ')
		{
			if (t_index > 0)
			{
				token[t_index] = '\0'; //per finire il token precedente
				tokens[t_count] = ft_strdup(token);
				t_count++;
				t_index = 0;
			}
			i++;
		}
		//se ci sono virgolette - cerca un altra virgoletta
		else if (clean_input[i] == '\'' || clean_input[i] == '\"')
		{
			quote = clean_input[i];
			i++;
			while (clean_input[i] && clean_input[i] != quote)
			{
				token[t_index] = clean_input[i];
				t_index++;
				i++;
			}
			if (i >= len) // se arrivato alla fine e non ha trovato la virgoletta, deve pulire tutto e stampare errore
			{
				printf("Error: missing quote\n");
				free(token);
				free_mtx(tokens);
				free(clean_input);
				return (NULL);
				//break ;
			}
			i++;
		}
		// se ci sono operatori (include || e &&)
		else if (clean_input[i] == '|' || clean_input[i] == '>' || clean_input[i] == '<' || clean_input[i] == '&')
		{
			twin_char = clean_input[i];
			if (t_index > 0)
			{
				token[t_index] = '\0';
				tokens[t_count] = ft_strdup(token);
				t_count++;
				t_index = 0;
			}
			token[t_index] = clean_input[i];
			if (clean_input[i + 1] == twin_char)
			{
				t_index++;
				i++;
				token[t_index] = clean_input[i];
			}
			token[t_index + 1] = '\0';
			tokens[t_count] = ft_strdup(token);
			t_count++;
			t_index = 0;
			i++;
		}
		else
		{
			token[t_index] = clean_input[i];
			t_index++;
			i++;
		}
	}
	//se ha letto tutto, metti ultimo token e finisci la matrice (null)
	if (t_index > 0)
	{
		token[t_index] = '\0';
		tokens[t_count] = ft_strdup(token);
		t_count++;
	}
	tokens[t_count] = NULL;
	free(token);
	free(clean_input);
	return (tokens);
}

t_token	*ft_token_list_without_redirection(char **tokens)
{
	t_token	*token;
	int		i;

	i = 0;
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token = ft_lstnew(tokens[i]);
	token->type = TOKEN_COMMAND;
	i++;
	while (tokens[i])
	{
		ft_lstadd_back(&token, ft_lstnew(tokens[i]));
		i++;
	}
	return (token);
}

char	**ft_tokenizer_main(char *input)
{
	t_token	*token;
	char	**tokens;

	token = NULL;
	tokens = ft_token_generator(input);
	token = ft_token_list_without_redirection(tokens);
	while (token)
	{
		printf("Token: %s\n", token->content);
		printf("Type: %d\n", token->type);
		token = token->next;
	}
	exit(0);

}