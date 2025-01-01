/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 15:31:26 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/29 15:31:33 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_env(t_token **token, t_main *main)
{
	int	i;

	i = -1;
	ft_del_first_node(token);
	while (main->env[++i])
	{
		if (!ft_strchr(main->env[i], '='))
			continue ;
		printf("%s\n", main->env[i]);
	}
}
