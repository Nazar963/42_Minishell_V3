/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 13:00:21 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/03 12:50:31 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_echo_pipes(t_token **token, t_main *main)
{
	int	flag_op;
	(void) main;
	flag_op = FALSE;
	ft_del_first_node(token);
	if (!*token)
	{
		printf("\n");
		return ;
	}
	if ((*token)->type == TOKEN_OPTION && ft_strlen((*token)->content) == 2 && (ft_strncmp((*token)->content, "-n", ft_strlen((*token)->content)) == 0))
	{
		ft_del_first_node(token);
		flag_op = TRUE;
	}
	while (*token && (*token)->type != TOKEN_PIPE)
	{
		if (flag_op)
		{
			/* if (ft_strncmp((*token)->content, "$?", 2) == 0
				&& ft_strlen((*token)->content) == 2)
				printf("%d ", g_global);
			else  */if ((*token)->next == NULL)
				printf("%s", (*token)->content);
			else
				if (*token && (*token)->content)
					printf("%s ", (*token)->content);
		}
		else if (flag_op == FALSE)
		{
			if ((*token)->next == NULL)
			{
				/* if (ft_strncmp((*token)->content, "$?", 2) == 0
					&& ft_strlen((*token)->content) == 2)
					printf("%d\n", g_global);
				else */
					printf("%s\n", (*token)->content);
			}
			/* else if (ft_strncmp((*token)->content, "$?", 2) == 0
				&& ft_strlen((*token)->content) == 2)
				printf("%d ", g_global); */
			else
				printf("%s ", (*token)->content);

		}
		g_global = 0;
		ft_del_first_node(token);
	}
}