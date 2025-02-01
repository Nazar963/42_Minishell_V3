/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_main_pipes_cl_fr.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:04:46 by naal-jen          #+#    #+#             */
/*   Updated: 2025/02/01 16:05:42 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	close_fdss(int **fds, t_main *main)
{
	main->i = 0;
	while (main->i < main->pipe_count)
	{
		close(fds[main->i][0]);
		main->i++;
	}
}

void	free_free_free(t_main *main, int **fds, t_token **list)
{
	free(main->pids);
	free_fds(fds, main->pipe_count);
	free_all(main, list);
}
