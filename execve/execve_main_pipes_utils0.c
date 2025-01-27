/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_main_pipes_utils0.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 12:46:39 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/26 16:37:03 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_first_pos_dup(t_token **list, t_main *main, int **fds)
{
	int	fd;

	if (!*list)
		return ;
	if ((*list)->heredoc_file)
	{
		fd = open((*list)->heredoc_file, O_RDONLY, 0666);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	dup2(fds[main->pos_fd][1], STDOUT_FILENO);
	close(fds[main->pos_fd][1]);
	close(fds[main->pos_fd][0]);
}

void	ft_second_pos_dup(t_token **list, t_main *main, int **fds)
{
	int	fd;

	if ((*list)->heredoc_file)
	{
		fd = open((*list)->heredoc_file, O_RDONLY, 0666);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else
	{
		dup2(fds[main->pos_fd - 1][0], STDIN_FILENO);
		close(fds[main->pos_fd - 1][0]);
	}
	dup2(fds[main->pos_fd][1], STDOUT_FILENO);
	close(fds[main->pos_fd][1]);
}

void	ft_third_pos_dup(t_token **list, t_main *main, int **fds)
{
	int	fd;

	if ((*list)->heredoc_file)
	{
		fd = open((*list)->heredoc_file, O_RDONLY, 0666);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else
	{
		dup2(fds[main->pos_fd - 1][0], STDIN_FILENO);
		close(fds[main->pos_fd - 1][0]);
	}
	dup2(main->orig_fd[1], STDOUT_FILENO);
}

void	ft_parent_pos_dup_close(int pos, t_main *main, int **fds)
{
	if (pos == 1 && fds[main->pos_fd])
		close(fds[main->pos_fd][1]);
	else if (pos == 2)
	{
		close(fds[main->pos_fd - 1][0]);
		close(fds[main->pos_fd][1]);
	}
	else if (pos == 3)
	{
		close(fds[main->pos_fd - 1][0]);
		dup2(main->orig_fd[0], STDIN_FILENO);
		dup2(main->orig_fd[1], STDOUT_FILENO);
	}
}

int	ft_fork(void)
{
	int	pid1;

	pid1 = fork();
	if (pid1 < 0)
	{
		perror("Fork Failed");
		exit(EXIT_FAILURE);
	}
	g_global = 7;
	return (pid1);
}
