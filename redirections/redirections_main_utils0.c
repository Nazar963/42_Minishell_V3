/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_main_utils0.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 21:18:08 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/08 21:20:14 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_getenv(char *name, t_main *main)
{
	int	i;
	int	j;

	i = 0;
	while (main->env[i])
	{
		j = 0;
		while (main->env[i][j] == name[j] && main->env[i][j] != '=')
			j++;
		if (main->env[i][j] == '=')
		{
			return (main->env[i] + j + 1);
		}
		i++;
	}
	return ("");
}

void	ft_expaned_var_ini(t_ex *ex, char *str)
{
	(*ex).i = 0;
	(*ex).y = 0;
	(*ex).new_str = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1000));
	if (!(*ex).new_str)
		return ;
	ft_memset((*ex).new_str, 0, 1000);
	(*ex).var = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1000));
	if (!(*ex).var)
	{
		free((*ex).new_str);
		return ;
	}
}

void	ft_expaned_var_norm0(t_ex *ex, char *str)
{
	(*ex).temp = (char *)malloc(sizeof(char) * 2);
	(*ex).temp[0] = str[(*ex).i];
	(*ex).temp[1] = '\0';
	(*ex).helper = (*ex).new_str;
	(*ex).new_str = ft_strjoin((*ex).new_str, (*ex).temp);
	free((*ex).helper);
	free((*ex).temp);
	(*ex).i++;
}

void	ft_expaned_var_norm1(t_ex *ex, char *str, t_main *main)
{
	(*ex).i++;
	(*ex).y = 0;
	while (str[(*ex).i] && str[(*ex).i] != ' ' && str[(*ex).i] != '$')
		(*ex).var[(*ex).y++] = str[(*ex).i++];
	(*ex).var[(*ex).y] = '\0';
	(*ex).env_var = ft_getenv((*ex).var, main);
	(*ex).env_var_copy = ft_strdup((*ex).env_var);
	(*ex).joined_str = ft_strjoin((*ex).new_str, (*ex).env_var_copy);
	free((*ex).env_var_copy);
	free((*ex).new_str);
	(*ex).new_str = (*ex).joined_str;
}

int	ft_access_check(t_token *temp, t_token **token)
{
	if (access(temp->content, W_OK) < 0)
	{
		if (errno != ENOENT)
		{
			ft_free_linked_list_until_pipe(token);
			perror("access");
			g_global = 1;
			return (1);
		}
	}
	return (0);
}
