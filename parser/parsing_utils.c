/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:49:23 by nakoriko          #+#    #+#             */
/*   Updated: 2025/01/26 18:15:14 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_only_spaces(char **str)
{
	int	i;

	i = 0;
	while ((*str)[i])
	{
		if ((*str)[i] > 0 && (*str)[i] <= 32)
			i++;
		else
			return (1);
	}
	free(*str);
	*str = NULL;
	return (0);
}

void	ft_add_to_buffer(t_mtx *data, char *var_value)
{
	int		i;
	char	*new_buffer;
	size_t	var_len;

	var_len = ft_strlen(var_value) + ft_strlen(data->str);
	new_buffer = ft_realloc_str(data->buffer, data->i, (var_len + 1));
	if (!new_buffer)
	{
		print_error("Memory alloc error", NULL, NULL);
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
	data->arg_count++;
}

int	ft_var_len(char *str)
{
	int	len;

	len = 0;
	if (!str[len] || !(ft_isalpha(str[len])) || str[len] == '_')
		return (0);
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_' ))
		len++;
	return (len);
}

char	*ft_get_env(char *str, char **env_test, int var_len)
{
	char	*new;
	int		i;
	int		j;
	int		value_len;

	i = 0;
	j = -1;
	new = NULL;
	while (env_test[++j])
	{
		if ((ft_strncmp(str, env_test[j], var_len) == 0)
			&& (env_test[j][var_len] == '='))
		{
			value_len = ft_strlen(&env_test[j][var_len + 1]);
			new = malloc(sizeof(char) * (value_len + 1));
			if (new == NULL)
				return (NULL);
			ft_strlcpy(new, &env_test[j][var_len + 1 + i], value_len + 1);
			return (new);
		}
	}
	return (new);
}
