/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 18:52:08 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/26 15:57:20 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_cmd(char *exec_path, char **cmd, t_main *main)
{
	close(main->orig_fd[0]);
	close(main->orig_fd[1]);
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
	write(2, cmd[0], ft_strlen(cmd[0]));
	return (print_error(": command not found", NULL, NULL), 127);
}

void	handle_path(t_token **token, t_main *main)
{
	int		i;
	int		ret;
	char	**new_path;
	char	**cmd;

	i = 0;
	while (main->env[i] && ft_strnstr(main->env[i], "PATH=", 5) == 0)
		i++;
	if (!main->env[i])
		print_error("minishell: ",
			(*token)->content, ": No such file or directory");
	ret = 127;
	if (main->env[i])
	{
		new_path = ft_split(main->env[i] + 5, ':');
		cmd = ft_from_list_to_array_pipes(token);
		ret = add_slash(new_path, cmd, main);
		free_mtx(&cmd);
		free_mtx(&new_path);
	}
	free_all(main, &main->token);
	free_linked_list(token);
	close(main->orig_fd[0]);
	close(main->orig_fd[1]);
	exit(ret);
}

void	child(t_token **token, t_main *main)
{
	int	status;
	int	pid1;
	int	fd;

	pid1 = ft_fork();
	if ((*token)->heredoc_file)
	{
		fd = open((*token)->heredoc_file, O_RDONLY);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (pid1 == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		handle_path(token, main);
	}
	waitpid(pid1, &status, 0);
	ft_handle_exit_status(status);
	dup2(main->orig_fd[0], STDIN_FILENO);
}

void	ft_execve_main(t_token **token, t_main *main)
{
	child(token, main);
}
