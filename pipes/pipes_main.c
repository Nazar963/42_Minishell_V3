/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:43:05 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/22 20:19:28 by naal-jen         ###   ########.fr       */
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

int	ft_count_pipes(t_token *token)
{
	int		count;

	count = 0;
	while (token)
	{
		if (token->type == TOKEN_PIPE)
			count++;
		token = token->next;
	}
	return (count);
}

int	ft_pipe_handler(t_token **list, t_main *main, int **fds, int pos_fd)
{
	bool	first;
	bool	last;
	t_token	*temp;
	int		pos;

	first = false;
	last = false;
	temp = *list;
	main->pos_fd = pos_fd;
	while (temp)
	{
		if (temp->type == TOKEN_PIPE && temp->next == NULL)
		{
			ft_del_node(list, temp);
			first = true;
		}
		if (temp->type == TOKEN_PIPE && temp->next != NULL)
		{
			ft_del_node(list, temp);
			last = true;
		}
		temp = temp->next;
	}
	if (first && !last)
	{
		pos = 1;
		return (ft_execve_main_pipes(list, main, fds, pos));
	}
	else if (first && last)
	{
		pos = 2;
		return (ft_execve_main_pipes(list, main, fds, pos));

	}
	else if (!first && last)
	{
		pos = 3;
		return (ft_execve_main_pipes(list, main, fds, pos));
	}
	return (0);
}

int	ft_pipes_main(t_token **token, t_main *main)
{
	t_token	*list;
	bool	done;
	int		**fds;
	int		pipe_count;
	int		i;
	int		*pids;
	int		status;

	i = -1;
	done = false;

	(void) main;
	if (ft_check_for_pipes(token) == false)
		return (false);

//* ---------------------------------- pipes --------------------------------- */
	pipe_count = ft_count_pipes(*token);
	fds = (int**)malloc(sizeof(int *) * (pipe_count));
	while (++i < pipe_count)
		fds[i] = (int*)malloc(sizeof(int) * 2);
	i = -1;
	while (++i < pipe_count)
		pipe(fds[i]);
	i = -1;
/* -------------------------------------------------------------------------- */

//* ---------------------------------- pids ---------------------------------- */
	pids = (int*)malloc(sizeof(int) * pipe_count);
	if (pids == NULL)
		return (false);
/* -------------------------------------------------------------------------- */

	list = malloc(sizeof(t_token));
	list = ft_lstnew_for_pipes(*token);
	while (done == false)
	{
		++i;
		while (*token)
		{
			if ((*token)->type == TOKEN_PIPE && ft_lstsize(list) != 1)
				break ;
			ft_del_first_node(token);
			if (!*token)
				break ;
			ft_lstadd_back(&list, ft_lstnew_for_pipes(*token));
		}
		pids[i] = ft_pipe_handler(&list, main, fds, i);
		free_linked_list(list);
		if (!*token)
			break ;
		list = malloc(sizeof(t_token));
		list = ft_lstnew_for_pipes(*token);
		if (!*token)
			done = true;
	}
	i = -1;
	while (++i <= pipe_count)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			g_global = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_global = 128 + WTERMSIG(status); // Conventionally, 128 + signal number
		else
			g_global = 1; // General error
	}
	return (true);
}