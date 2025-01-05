/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 15:30:41 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/03 20:50:57 by naal-jen         ###   ########.fr       */
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

void	ft_unset(t_token **token, t_main *main)
{
	int	i;
	int	len;

	ft_del_first_node(token);
	while (*token)
	{
		i = 0;
		while (main->env[i])
		{
			len = ft_strlen((*token)->content);
			if (ft_strncmp(main->env[i], (*token)->content, len) == 0
				&& (main->env[i][len] == '=' || main->env[i][len] == '\0'))
			{
				i = ft_unset_norm0(token, main, i, len);
				break ;
			}
			i++;
			if (main->env[i] == NULL)
			{
				ft_del_first_node(token);
				break ;
			}
		}
	}
}
