/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:43:05 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/03 17:01:58 by naal-jen         ###   ########.fr       */
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

int	ft_pip_handler(t_token **list, t_main *main, int **fds, int pos_fd)
{
	bool	first;
	bool	last;
	t_token	*temp;
	int		pos;
	t_token	*next;

	first = false;
	last = false;
	temp = *list;
	main->pos_fd = pos_fd;
	while (temp)
	{
		if (temp->content && temp->type == TOKEN_PIPE && temp->next == NULL)
		{
			next = temp->next;
			ft_del_node(list, temp);
			temp = next;
			first = true;
			if (!temp)
				break ;
		}
		else if (temp->content && temp->type == TOKEN_PIPE && temp->next != NULL)
		{
			next = temp->next;
			ft_del_node(list, temp);
			temp = next;
			last = true;
			if (!temp)
				break ;
		}
		else
			temp = temp->next;
	}
	if (first && !last)
	{
		pos = 1;
		return (ft_exe_main_pipes(list, main, fds, pos));
	}
	else if (first && last)
	{
		pos = 2;
		return (ft_exe_main_pipes(list, main, fds, pos));

	}
	else if (!first && last)
	{
		pos = 3;
		return (ft_exe_main_pipes(list, main, fds, pos));
	}
	return (0);
}

void	free_fds(int **fds, int pipe_count)
{
	int	i;

	i = 0;
	if (!fds)
		return ;
	while (i < pipe_count)
	{
		free(fds[i]);
		fds[i] = NULL;
		i++;
	}
	free(fds);
	fds = NULL;
}

int	ft_pipes_main(t_token **token, t_main *main)
{
	t_token	*list;
	bool	done;
	int		**fds;
	int		i;
	int		status;

	i = -1;
	done = false;

	if (ft_check_for_pipes(token) == false)
		return (false);

//* ---------------------------------- pipes --------------------------------- */
	main->pipe_count = ft_count_pipes(*token);
	fds = (int**)malloc(sizeof(int *) * (main->pipe_count));
	while (++i < main->pipe_count)
		fds[i] = (int*)malloc(sizeof(int) * 2);
	i = -1;
	while (++i < main->pipe_count)
		pipe(fds[i]);
	i = -1;
/* -------------------------------------------------------------------------- */

//* ---------------------------------- pids ---------------------------------- */
	main->pids = (int *)malloc(sizeof(int) * (main->pipe_count + 1));
	if (main->pids == NULL)
		return (false);
/* -------------------------------------------------------------------------- */
	if (ft_herdoc_pipes_main(token, main) == 1)
	{
		free_fds(fds, main->pipe_count);
		free(main->pids);
		return (true);
	}
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
		main->token = *token;
		main->pids[i] = ft_pip_handler(&list, main, fds, i);
		free_linked_list(&list);
		if (!*token)
			break ;
		free_linked_list(&list);
		list = ft_lstnew_for_pipes(*token);
		if (!*token)
			done = true;
	}
	free_linked_list(&list);
	i = -1;
	while (++i <= main->pipe_count)
	{
		waitpid(main->pids[i], &status, 0);
		if (WIFEXITED(status))
			g_global = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_global = 128 + WTERMSIG(status); // Conventionally, 128 + signal number
		else
			g_global = 1; // General error
	}
	free(main->pids);
	free_fds(fds, main->pipe_count);
	return (true);
}
