/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:43:55 by nakoriko          #+#    #+#             */
/*   Updated: 2025/01/26 17:26:24 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_tokens_finish(t_mtx *data)
{
	if (data->j > 0)
	{
		data->buffer[data->j] = '\0';
		data->arg_count++;
		data->tokens[data->y] = ft_strdup(data->buffer);
		data->y++;
	}
	if (data->arg_count >= data->k)
		return ;
	data->tokens[data->y] = NULL;
	free(data->buffer);
	free(data->str);
}

char	*remove_begin_end_whitespaces(char *input)
{
	int		j;
	char	*new;
	int		end;
	int		start;

	end = ft_strlen(input) - 1;
	start = 0;
	while (input[start] == ' ' || input[start] == '	')
		start++;
	while ((ft_is_space(input[end]) == 1) && end > start)
		end--;
	new = malloc(sizeof(char) * (end - start) + 2);
	if (!new)
		return (NULL);
	j = 0;
	while (start <= end)
		new[j++] = input[start++];
	new[j] = '\0';
	return (new);
}

void	ft_tokendata_init(t_mtx *data, char *input)
{
	data->i = 0;
	data->y = 0;
	data->j = 0;
	data->arg_count = 0;
	data->quote = 0;
	data->twin_char = 0;
	data->k = 0;
	data->check = 0;
	data->delim_flag = 0;
	data->str = remove_begin_end_whitespaces(input);
	data->len = ft_strlen(data->str);
	data->tokens = malloc(sizeof(char *) * 2000);
	if (!data->tokens)
		return ;
	while (data->k < 2000)
	{
		data->tokens[data->k] = NULL;
		data->k++;
	}
	data->buffer = NULL;
	data->buffer = malloc(sizeof(char) * (data->len + 1));
	if (data->buffer == NULL)
		return (free(data->tokens), (void)0);
	ft_memset(data->buffer, 0, data->len + 1);
}

char	**ft_token_generator(char *input, char **env)
{
	t_mtx	data;

	ft_tokendata_init(&data, input);
	if (data.tokens == NULL)
		return (NULL);
	preparsing_check_and_split_input(&data, env);
	if (data.check == 1 || data.arg_count >= data.k)
	{
		free_all_preparsing(&data);
		return (NULL);
	}
	ft_tokens_finish(&data);
	return (data.tokens);
}

t_token	*ft_tokenizer_main(char *input, t_main *main)
{
	t_token	*token;
	char	**tokens;

	(void) main;
	token = NULL;
	tokens = ft_token_generator(input, main->env);
	if (tokens != NULL && tokens[0] && ft_tokens_check(tokens) == 0)
	{
		token = ft_token_list_creation(tokens);
		free_mtx(&tokens);
		if (token == NULL)
			return (NULL);
	}
	else
	{
		free_mtx(&tokens);
		return (NULL);
	}
	if (g_global == 667)
		g_global = 0;
	return (token);
}
