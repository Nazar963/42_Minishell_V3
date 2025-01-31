/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_pipes_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 22:27:27 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/31 22:40:48 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_valid_var_name_pipes(const char *name)
{
	int	i;

	i = 0;
	if (!name || name[0] == '\0')
		return (0);
	if (!(ft_isalpha(name[0]) || name[0] == '_'))
		return (0);
	while (name[++i] != '\0' && name[i] != '=')
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
		{
			if (name[i] == '+' && name[i + 1] == '\0')
				return (2);
			return (0);
		}
	}
	return (1);
}

void	ft_export_pipes_norm0(t_token **token, t_main *main)
{
	int	len;

	if (ft_check_for_already_existing_variable(main->env, (*token)->content))
	{
		ft_del_first_node(token);
		return ;
	}
	len = ft_strlen_mtx(main->env);
	main->env = ft_realloc(main->env, len + 1);
	main->env[len] = ft_strdup((*token)->content);
	ft_del_first_node(token);
	return ;
}

void	ft_export_pipes_norm1(t_token **token, t_main *main, char **splitted)
{
	int	len;

	if (ft_check_for_already_existing_variable(main->env, splitted[0]))
	{
		main->env = ft_export_var_reassign_p(main->env, splitted[0], "");
	}
	else
	{
		len = ft_strlen_mtx(main->env);
		main->env = ft_realloc(main->env, len + 1);
		main->env[len] = ft_strdup((*token)->content);
	}
}

void	ft_export_pipes_norm2(t_main *main, char **s)
{
	int		len;
	char	*temp;

	if (ft_check_for_already_existing_variable(main->env, s[0]))
	{
		main->env = ft_export_var_reassign_p(main->env, s[0], s[1]);
	}
	else
	{
		len = ft_strlen_mtx(main->env);
		main->env = ft_realloc(main->env, len + 1);
		main->env[len] = ft_strdup(s[0]);
		temp = main->env[len];
		main->env[len] = ft_strjoin(main->env[len], "=");
		free (temp);
		temp = main->env[len];
		main->env[len] = ft_strjoin(main->env[len], s[1]);
		free (temp);
	}
}

char	*ft_export_var_reassign_p_norm0(char *orig, char *value)
{
	orig = ft_strjoin_mod(orig, value);
	return (orig);
}
