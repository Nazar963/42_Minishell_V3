/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 18:52:08 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/11 15:05:53 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_cmd(char *exec_path, char **cmd, t_main *main)
{
	if (execve(exec_path, cmd, main->env) == -1)
	{
		free_mtx(&cmd);
		free_all(main, &main->token);
		perror("execve");
		if (errno == ENOENT)
			exit(127);
		else
			exit(126);
	}
}

int	ft_no_special_characters_fun(char **cmd, t_main *main)
{
	int	ret;

	ret = ft_no_special_characters_pipes(cmd[0]);
	if (ret != 0)
		return (ret);
	if (ft_strncmp(cmd[0], "./", 2) == 0 || ft_strrchr(cmd[0], '/'))
		return (ft_add_slash_pipes_file(cmd, main->env));
	return (0);
}

int	add_slash(char **new_path, char **cmd, t_main *main)
{
	int		i;
	char	*exec_path;

	i = ft_no_special_characters_fun(cmd, main);
	if (i != 0)
		return (i);
	i = -1;
	while (new_path[++i])
	{
		if (ft_strrchr(cmd[0], '/'))
		{
			exec_path = ft_strjoin(new_path[i], ft_strrchr(cmd[0], '/'));
			if (access(exec_path, F_OK | X_OK) == 0)
				execute_cmd(exec_path, cmd, main);
		}
		new_path[i] = add_slash_pipes_norm0(new_path[i]);
		exec_path = ft_strjoin(new_path[i], cmd[0]);
		if (access(exec_path, F_OK | X_OK) == 0)
			execute_cmd(exec_path, cmd, main);
		else
			free(exec_path);
	}
	return (print_error(" command not found", NULL, NULL), 127);
}

void	handle_path(t_token **token, t_main *main)
{
	int		i;
	int		ret;
	char	**new_path;
	char	**cmd;

	i = 0;
	while (ft_strnstr(main->env[i], "PATH=", 5) == 0)
		i++;
	new_path = ft_split(main->env[i] + 5, ':');
	cmd = ft_from_list_to_array_pipes(token);
	ret = add_slash(new_path, cmd, main);
	free_mtx(&cmd);
	free_mtx(&new_path);
	free_all(main, &main->token);
	free_linked_list(token);
	exit(ret);
}

void	child(t_token **token, t_main *main)
{
	int	status;
	int	pid1;
	int	fd;

	g_global = 7;
	pid1 = ft_fork();
	if ((*token)->heredoc_file)
	{
		fd = open((*token)->heredoc_file, O_RDONLY);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (pid1 == 0)
		handle_path(token, main);
	waitpid(pid1, &status, 0);
	if (WIFEXITED(status))
	{
		g_global = WEXITSTATUS(status);
		if (g_global == 7)
			g_global = 0;
	}
	else if (WIFSIGNALED(status))
	{
		g_global = 128 + WTERMSIG(status);
	}
	else
		g_global = 1;
	dup2(main->orig_fd[0], STDIN_FILENO);
}

void	ft_execve_main(t_token **token, t_main *main)
{
	child(token, main);
}
