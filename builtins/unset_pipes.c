/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 15:30:41 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/30 13:32:07 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_unset_pipes(t_token **token, t_main *main)
{
	int	i;

	ft_del_first_node(token);
	if (!*token)
		return ;
	while (*token)
	{
		i = 0;
		while (main->env[i])
		{
			if (ft_strncmp(main->env[i], (*token)->content, ft_strlen((*token)->content)) == 0
				&& main->env[i][ft_strlen((*token)->content)] == '=')
			{
				ft_del_first_node(token);
				free(main->env[i]);
				while (main->env[i])
				{
					main->env[i] = main->env[i + 1];
					i++;
				}
				break ;
			}
			else if (ft_strncmp(main->env[i], (*token)->content, ft_strlen((*token)->content)) == 0
				&& main->env[i][ft_strlen((*token)->content)] == '\0')
			{
				printf("hi am here man\n");
				free(main->env[i]);
				while (main->env[i])
				{
					main->env[i] = main->env[i + 1];
					i++;
				}
				break ;
			}
			i++;
			if (main->env[i] == NULL){
				ft_del_first_node(token);
				break ;
			}
		}
	}
}