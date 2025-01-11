/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 13:00:21 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/10 22:49:06 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_check_sequence(t_token *temp)
{
	int	i;

	i = 0;
	if (ft_strlen(temp->content) == 1 || temp->content[i] != '-'
		|| temp->content[i + 1] != 'n')
		return (0);
	while (temp->content[++i])
	{
		if (temp->content[i] != 'n')
			return (0);
	}
	return (1);
}

int	ft_clean_options(t_token **token)
{
	t_token	*temp;
	int		flag_op;

	temp = *token;
	flag_op = FALSE;
	while (ft_check_sequence(temp))
	{
		ft_del_first_node(token);
		temp = *token;
		if (!temp)
			break ;
		flag_op = TRUE;
	}
	return (flag_op);
}

void	ft_echo_norm0(t_token **token)
{
	if ((*token)->next == NULL)
		printf("%s\n", (*token)->content);
	else
		printf("%s ", (*token)->content);
}

void	ft_echo(t_token **token)
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
