/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_and_expansion.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 17:45:01 by nakoriko          #+#    #+#             */
/*   Updated: 2025/01/26 18:15:21 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	quotes_delimiter(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

void	ft_expand(t_mtx *data, char **env_test)
{
	char	*var_name;
	char	*var_value;
	int		var_len;

	var_len = ft_var_len(&data->str[data->i + 1]);
	if (var_len > 0 && data->delim_flag == 0)
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

void	ft_expand_global(t_mtx *data)
{
	char	*str_global;

	if (g_global == 667)
		str_global = ft_itoa(130);
	else
		str_global = ft_itoa(g_global);
	if (str_global)
	{
		ft_add_to_buffer(data, str_global);
		data->i = data->i + 2;
		free(str_global);
		if (data->check == 1)
			return ;
	}
}
