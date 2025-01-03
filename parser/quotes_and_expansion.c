/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_and_expansion.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakoriko <nakoriko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 17:45:01 by nakoriko          #+#    #+#             */
/*   Updated: 2025/01/03 18:18:58 by nakoriko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		if (var_value)//controllo delimitatore
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

void ft_expand_global(t_mtx *data)
{
	char *str_global;

	str_global = ft_itoa(g_global);
	if (str_global)
	{
		ft_add_to_buffer(data, str_global);
		data->i = data->i + ft_strlen(str_global) + 1; // non sono sicura se +1;
		free(str_global);
		if (data->check == 1)
			return ;
	}
}

//Espansione tra quotes + controllo di unmatched quotes
void ft_token_quote(t_mtx *data, char **env_test)
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
		data->check = 1;
		return ;
	}
	data->buffer[data->j++] = data->str[data->i++];
}

// Eliminazione di quetes prima di fare exec ////////
char *check_content_quotes(char *str)
{
    int i;
    int j;
    char *new_str;
    char c;

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
    return(new_str);
}
