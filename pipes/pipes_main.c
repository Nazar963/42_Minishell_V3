/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:43:05 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/15 20:10:33 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdbool.h>

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

int	ft_handle_execution_route(t_main *main, t_token **list, int **fds)
{
	int	pos;

	if (main->first && !main->last)
	{
		pos = 1;
		return (ft_exe_main_pipes(list, main, fds, pos));
	}
	else if (main->first && main->last)
	{
		pos = 2;
		return (ft_exe_main_pipes(list, main, fds, pos));
	}
	else if (!main->first && main->last)
	{
		pos = 3;
		return (ft_exe_main_pipes(list, main, fds, pos));
	}
	return (0);
}

int	ft_pip_handler(t_token **list, t_main *main, int **fds, int pos_fd)
{
	t_token	*temp;
	t_token	*next;

	main->first = false;
	main->last = false;
	main->pos_fd = pos_fd;
	temp = *list;
	while (temp)
	{
		if (temp->content && temp->type == TOKEN_PIPE && temp->next == NULL)
			main->first = true;
		else if (temp->content && temp->type == TOKEN_PIPE && temp->next)
			main->last = true;
		else
		{
			temp = temp->next;
			continue ;
		}
		next = temp->next;
		ft_del_node(list, temp);
		temp = next;
		if (!temp)
			break ;
	}
	return (ft_handle_execution_route(main, list, fds));
}

void	ft_pipes_loop(t_token **token, t_main *main, t_token **list, int **fds)
{
	int	i;

	i = -1;
	while (main->done == false && ++i > -1)
	{
		while (*token)
		{
			if ((*token)->type == TOKEN_PIPE && ft_lstsize(*list) != 1)
				break ;
			ft_del_first_node(token);
			if (!*token)
				break ;
			ft_lstadd_back(list, ft_lstnew_for_pipes(*token));
		}
		main->token = *token;
		main->pids[i] = ft_pip_handler(list, main, fds, i);
		free_linked_list(list);
		if (!*token)
			break ;
		if (ft_lstsize(*token) == 1 && (*token)->type == TOKEN_PIPE)
			return (free_linked_list(token), (void)0);
		*list = ft_lstnew_for_pipes(*token);
		if (!*token)
			main->done = true;
	}
}

int	ft_pipes_main(t_token **token, t_main *main)
{
	t_token	*list;
	int		**fds;

	main->done = false;
	fds = NULL;
	if (ft_pipes_main_init(token, main, &fds) == false)
		return (false);
	list = ft_lstnew_for_pipes(*token);
	ft_pipes_loop(token, main, &list, fds);
	return (ft_g_global_status(&list, main, fds), true);
}
