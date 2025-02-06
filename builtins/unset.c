/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 15:30:41 by naal-jen          #+#    #+#             */
/*   Updated: 2025/02/06 18:26:54 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_unset_norm0(t_token **token, t_main *main, int i, int len)
{
	if (main->env[i][len] == '=')
		ft_del_first_node(token);
	free(main->env[i]);
	while (main->env[i])
	{
		main->env[i] = main->env[i + 1];
		i++;
	}
	return (i);
}

int	ft_is_valid_unset_identifier(char *var)
{
	int	i;

	i = 0;
	if (!var || var[0] == '\0')
		return (0);
	if (!ft_isalpha(var[0]) && var[0] != '_')
		return (0);
	i = 1;
	while (var[i])
	{
		if (var[i] == '=')
			return (0);
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	ft_unset_invalid_identifier(t_token **token)
{
	g_global = 1;
	print_error("minishell: unset: `",
		(*token)->content, "\' not a valid identifier");
	ft_del_first_node(token);
}

void	ft_unset_unset_loop(int *i, t_main *main, t_token **token)
{
	int	len;

	while (*token && main->env[(*i)])
	{
		len = ft_strlen((*token)->content);
		if (ft_strncmp(main->env[(*i)], (*token)->content, len) == 0
			&& (main->env[(*i)][len] == '=' || main->env[(*i)][len] == '\0'))
		{
			(*i) = ft_unset_norm0(token, main, (*i), len);
			g_global = 0;
			return ;
		}
		(*i)++;
		if (main->env[(*i)] == NULL)
		{
			ft_del_first_node(token);
			g_global = 0;
			return ;
		}
	}
}

void	ft_unset(t_token **token, t_main *main)
{
	int	i;

	ft_del_first_node(token);
	if (!(*token) || (*token)->type == TOKEN_PIPE)
	{
		g_global = 0;
		return ;
	}
	while (*token)
	{
		if (!ft_is_valid_unset_identifier((*token)->content))
			ft_unset_invalid_identifier(token);
		i = 0;
		ft_unset_unset_loop(&i, main, token);
	}
}
