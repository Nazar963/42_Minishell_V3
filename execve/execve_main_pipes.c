/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_main_pipes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 18:52:08 by naal-jen          #+#    #+#             */
/*   Updated: 2025/02/06 19:21:44 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_cmd_pipes(char *exec_path, char **cmd, char **envp, t_main *m)
{
	close(m->orig_fd[0]);
	close(m->orig_fd[1]);
	if (execve(exec_path, cmd, envp) == -1)
	{
		free_mtx(&cmd);
		perror("execve");
		if (errno == ENOENT)
			exit(127);
		else
			exit(126);
	}
}

int	add_slash_pipes(char **new_path, char **cmd, char **envp, t_main *main)
{
	int		i;
	char	*exec_path;

	i = -1;
	if (ft_no_special_characters_pipes(cmd[0]) != 0)
		return (ft_no_special_characters_pipes(cmd[0]));
	if (ft_strncmp(cmd[0], "./", 2) == 0 || ft_strrchr(cmd[0], '/'))
		return (ft_add_slash_pipes_file(cmd, envp, main));
	while (new_path[++i])
	{
		if (ft_strrchr(cmd[0], '/'))
		{
			exec_path = ft_strjoin(new_path[i], ft_strrchr(cmd[0], '/'));
			if (access(exec_path, F_OK | X_OK) == 0)
				execute_cmd_pipes(exec_path, cmd, envp, main);
		}
		new_path[i] = add_slash_pipes_norm0(new_path[i]);
		exec_path = ft_strjoin(new_path[i], cmd[0]);
		if (access(exec_path, F_OK | X_OK) == 0)
			execute_cmd_pipes(exec_path, cmd, envp, main);
		else
			free(exec_path);
	}
	write(2, cmd[0], ft_strlen(cmd[0]));
	return (print_error(": command not found", NULL, NULL), 127);
}

void	handle_path_pipes(t_token **token, t_main *main, int **fds)
{
	int		i;
	int		ret;
	char	**new_path;
	char	**cmd;

	i = 0;
	while (main->env[i] && ft_strnstr(main->env[i], "PATH=", 5) == 0)
		i++;
	if (main->env[i])
	{
		new_path = ft_split(main->env[i] + 5, ':');
		cmd = ft_from_list_to_array_pipes(token);
		ret = add_slash_pipes(new_path, cmd, main->env, main);
		free_mtx(&cmd);
		free_mtx(&new_path);
	}
	else
		ret = handle_path_pipes_no_path(token);
	free(main->pids);
	free_fds(fds, main->pipe_count);
	free_all(main, &main->token);
	close(main->orig_fd[0]);
	close(main->orig_fd[1]);
	exit(ret);
}

int	child_pipes(t_token **list, t_main *main, int **fds, int pos)
{
	int	pid1;

	g_global = 7;
	pid1 = ft_fork();
	if (pid1 == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		if (pos == 1)
			ft_first_pos_dup(list, main, fds);
		else if (pos == 2)
			ft_second_pos_dup(list, main, fds);
		else if (pos == 3)
			ft_third_pos_dup(list, main, fds);
		close_fdss(fds, main);
		if (*list)
			ft_redirections_main(list, main);
		if (*list)
			ft_builtins_main_pipes(list, main, fds);
		if (*list)
			handle_path_pipes(list, main, fds);
		free_free_free(main, fds, list);
		exit(g_global);
	}
	return (ft_parent_pos_dup_close(pos, main, fds), pid1);
}

int	ft_exe_main_pipes(t_token **list, t_main *main, int **fds, int pos)
{
	return (child_pipes(list, main, fds, pos));
}
