/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_main_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:14:45 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/15 20:13:46 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	ft_pipes_main_init(t_token **token, t_main *main, int ***fds)
{
	int	i;

	i = -1;
	if (ft_check_for_pipes(token) == false)
		return (false);
	main->pipe_count = ft_count_pipes(*token);
	*fds = (int **)malloc(sizeof(int *) * (main->pipe_count));
	while (++i < main->pipe_count)
		(*fds)[i] = (int *)malloc(sizeof(int) * 2);
	i = -1;
	while (++i < main->pipe_count)
		pipe((*fds)[i]);
	main->pids = (int *)malloc(sizeof(int) * (main->pipe_count + 1));
	ft_memset(main->pids, 0, sizeof(int) * (main->pipe_count + 1));
	if (main->pids == NULL)
		return (false);
	if (ft_herdoc_pipes_main(token, main) == 1)
	{
		free_fds(*fds, main->pipe_count);
		free(main->pids);
		return (false);
	}
	return (true);
}

void	ft_g_global_status(t_token **list, t_main *main, int **fds)
{
	int	status;
	int	i;
	int	last;
	int	counter;

	free_linked_list(list);
	i = -1;
	counter = 0;
	last = 0;
	while (++i <= main->pipe_count)
	{
		waitpid(main->pids[i], &status, 0);
		if (WIFEXITED(status))
			ft_g_global_status_norm0(last, counter, status, i);
		else if (WIFSIGNALED(status))
			ft_g_global_status_norm1(&last, &counter, status, i);
		else
			g_global = 1;
	}
	free(main->pids);
	free_fds(fds, main->pipe_count);
}
