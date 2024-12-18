/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_generator.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 13:13:26 by nakoriko          #+#    #+#             */
/*   Updated: 2024/12/14 20:14:20 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void ft_tokendata_init(t_mtx *data, char *input)
{
	data->i = 0;
	data->y = 0;
	data->j = 0;
	data->quote = 0;
	data->twin_char = 0;
	data->k = 0;
	data->check = 0;
	data->str = remove_begin_end_whitespaces(input);
	data->len = ft_strlen(data->str);
	data->tokens = malloc(sizeof(char *) * 256);//massima quantita di argomenti, visto che non possiamo usare realloc
	if (!data->tokens)
		return ;
	while (data->k < 256)
	{
		data->tokens[data->k] = NULL;
		data->k++;
	}
	data->buffer = malloc(sizeof(char) * data->len + 1);
	if (data->buffer == NULL)
	{
		free(data->tokens);
		return ;
	}
}

void ft_token_space(t_mtx *data)
{
	if (data->j > 0)
	{
		data->buffer[data->j] = '\0'; //per finire il token precedente
		data->tokens[data->y] = ft_strdup(data->buffer);
		//free_str(data->buffer); // ?
		data->y++;
		data->j = 0;
	}
	data->i++;
}

void 	ft_add_to_buffer(t_mtx *data, char *var_value)
{
	int i;
	char *new_buffer;
	size_t var_len;

	var_len = ft_strlen(var_value) + ft_strlen(data->str);
	////// aggiungere la memoria nel buffer. Dimensione precisa o solo dimensione abbastanza? Se precisa - bisogna di toglere il nome (var_name)
	
	new_buffer = realloc(data->buffer, (var_len + 1));//cambiare per la funzione nostra
	if (!new_buffer)
	{
		print_error("Impossible to allocate memory for environment variable value", NULL);
		data->check = 1;
		return ;
	}
	data->buffer = new_buffer;
	i = 0;
	while (var_value[i])
	{
		data->buffer[data->j] = var_value[i];
		i++;
		data->j++;
	}
	data->buffer[data->j] = '\0';
}

int ft_var_len(char *str)
{
	int len;

	len = 0;
	if (!str[len] || !(ft_isalpha(str[len])) || str[len] == '_') // controllo che la prima lettera esiste, lettera o underscore
		return (0);
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_' )) // controllo che sono alphanumerici o underscore
		len++;
	return (len);
}

char *ft_get_env(char *str, char **env_test, int var_len)
{
	char *new;
	int i;
	int j;
	int value_len;

	i = 0;
	j = 0;
	new = NULL;
	while (j < 5) // passrae largezza di matrice
	{
		if ((ft_strncmp(str, env_test[j], var_len) == 0) && (env_test[j][var_len] == '=')) //se stringe uguali
		{
			
			value_len = ft_strlen(&env_test[j][var_len + 1]); // calcoliamo lunghezza del valore iniziando
			//dal fine del nome di variabile + 1 (simbolo "=");
			new = malloc(sizeof(char) * (value_len + 1)); // lunghezza + "\0"
			if (new == NULL)
				return(NULL);
			i = 0;
			while(env_test[j][var_len + 1 + i] != '\0') // aggiunggere controllo di '\n'?
			{
				new[i] = env_test[j][var_len + 1 + i];
				i++;
			}
			new[i] = '\0';
			return(new);
		}
		j++;
	}
	return(new);
}

void ft_expand(t_mtx *data, char **env_test)
{
	char *var_name;
	char *var_value;
	int var_len;

	var_len = ft_var_len(&data->str[data->i + 1]);
	if (var_len > 0)
	{
		var_name = ft_substr(data->str, data->i + 1, var_len);
		var_value = ft_get_env(var_name, env_test, var_len);
		free_str(var_name);
		if (var_value)
		{
			ft_add_to_buffer(data, var_value);
			free(var_value);
			if (data->check == 1)
				return ;
		}
		else
			data->buffer[data->j] = '\0';
		data->i = data->i + var_len + 1;
	}
	else 
		data->buffer[data->j++] = data->str[data->i++];
}

void ft_token_quote(t_mtx *data, char **env_test)
{
	

	data->quote = data->str[data->i];
	data->i++;
	while (data->str[data->i] && data->str[data->i] != data->quote)
	{
		if (data->str[data->i] == '$' && data->quote== '\"')
		{
			ft_expand(data, env_test);
			if (data->check == 1)
				return ;
		}
		else
		{
			data->buffer[data->j] = data->str[data->i];
			data->j++;
			data->i++;
		}
	}
	if (data->str[data->i] != data->quote)
	{
		print_error("Error: Unmatched quote", NULL);
		data->check = 1;
		return ;
	}
	data->i++;
}


void ft_token_operator(t_mtx *data)
{
	data->twin_char = data->str[data->i];
	if (data->j > 0)
	{
		data->buffer[data->j] = '\0';
		data->tokens[data->y] = ft_strdup(data->buffer);
		data->y++;
		data->j = 0;
	}
	data->buffer[data->j] = data->str[data->i];
	if (data->str[data->i + 1] == data->twin_char)
	{
		data->j++;
		data->i++;
		data->buffer[data->j] = data->str[data->i];
	}
	data->buffer[data->j + 1] = '\0';
	data->tokens[data->y] = ft_strdup(data->buffer);
	data->y++;
	data->j = 0;
	data->i++;
}

void ft_tokens_finish(t_mtx *data)
{
	if (data->j > 0)
	{
		data->buffer[data->j] = '\0';
		data->tokens[data->y] = ft_strdup(data->buffer);
		data->y++;
	}
	data->tokens[data->y] = NULL;
	free(data->buffer);
	free(data->str);
}
