/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 13:00:21 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/10 22:49:01 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_echo_pipes(t_token **token)
{
	int	flag_op;

	ft_del_first_node(token);
	if (!*token)
		return ((void)(printf("\n"), 0));
	flag_op = ft_clean_options(token);
	if (!(*token))
		return ;
	while (*token && (*token)->type != TOKEN_PIPE)
	{
		if (flag_op)
		{
			if ((*token)->next == NULL)
				printf("%s", (*token)->content);
			else
				if (*token && (*token)->content)
					printf("%s ", (*token)->content);
		}
		else if (flag_op == FALSE)
			ft_echo_norm0(token);
		g_global = 0;
		ft_del_first_node(token);
	}
}
