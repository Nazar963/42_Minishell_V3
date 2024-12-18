/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:43:05 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/17 21:46:16 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_check_for_pipes(t_token **token)
{
	t_token	*temp;

	temp = *token;
	while (temp)
	{
		if (temp->type == TOKEN_PIPE)
			return (true);
		temp = temp->next;
	}
	return (false);
}

void	ft_pipes_main(t_token **token, t_main *main)
{
	t_token	*list;

	(void) main;
	if (ft_check_for_pipes(token) == false)
		return ;
	list = malloc(sizeof(t_token));
	list = ft_lstnew_for_pipes(*token);
	while (*token)
	{
		if ((*token)->type == TOKEN_PIPE)
		{
			ft_del_first_node(token);
			break ;
		}
		ft_del_first_node(token);
		ft_lstadd_back(&list, ft_lstnew_for_pipes(*token));
	}

	while (list)
	{
		printf("this is the content: %s\n", list->content);
		printf("this is the type: %d\n", list->type);
		list = list->next;
	}

	printf("\n\n\n");

	while (*token)
	{
		printf("this is the content: %s\n", (*token)->content);
		printf("this is the type: %d\n", (*token)->type);
		*token = (*token)->next;
	}

	exit(0);
}