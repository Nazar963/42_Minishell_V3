/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_main_pipes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 18:52:08 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/30 15:35:35 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_join_path_pipes(char **cmd)
{
	char	*exec_path;
	int		i;

	i = 0;
	while (cmd[i])
	{
		exec_path = ft_strjoin(cmd[i], "/");
		i++;
	}
	printf("this is the whole path man: %s\n", exec_path);
	exit(0);
	return (exec_path);
}

void	execute_cmd_pipes(char *exec_path, char **cmd, char **envp)
{
	if (execve(exec_path, cmd, envp) == -1)
	{
		free_mtx(cmd);
		perror("execve");
		if (errno == ENOENT)
			exit(127);
		else
			exit(126);
	}
}

void	error_cmd_pipes(char *cmd)
{
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": command not found", 2);
	if (errno == ENOENT)
		exit(127);
	else
		exit(126);
}

int	error_file_dir_pipes(char *cmd)
{
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": No such file or directory", 2);
	if (errno == ENOENT)
		return (127);
	else
		return (126);
}

int	ft_no_special_characters_pipes(char *cmd)
{
	if (ft_strncmp(cmd, "$", 1) == 0)
	{
		ft_putstr_fd("minishell: $: command not found\n", 2);
		return(127);
	}
	if (ft_isdigit(cmd[0]) == 1)
	{
		if (ft_atoi(cmd) == g_global)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putendl_fd(": command not found", 2);
			return(127);
		}
	}
	return (0);
}

int	add_slash_pipes(char **new_path, char **cmd, char **envp)
{
	int		i;
	int		ret;
	char	*exec_path;
	struct stat	path_stat;
	char	*with_slash;

	i = -1;
	ret = ft_no_special_characters_pipes(cmd[0]);
	if (ret != 0)
		return (ret);
	with_slash = ft_strrchr(cmd[0], '/');
	if (ft_strncmp(cmd[0], "./", 2) == 0)
	{
		if (stat(cmd[0], &path_stat) == -1)
		{
			if (errno == ENOENT)
				return (error_file_dir_pipes(cmd[0]));
			print_error("Error retrieving file information", NULL, NULL);
			return (126);
		}
		if (S_ISDIR(path_stat.st_mode))
		{
			print_error(" Is a directory", NULL, NULL);
			return (126);
		}

		if (access(cmd[0], X_OK) == -1)
		{
			if (errno == EACCES)
			{
				print_error(" Permission denied", NULL, NULL);
				return (126);
			}
			print_error(" Error accessing file", NULL, NULL);
			return (126);
		}

		execute_cmd_pipes(cmd[0], cmd, envp);
	}
	if (with_slash)
	{
		if (stat(cmd[0], &path_stat) == -1)
		{
			if (errno == ENOENT)
				return (error_file_dir_pipes(cmd[0]));
			print_error("Error retrieving file information", NULL, NULL);
			exit(126);
		}

		if (S_ISDIR(path_stat.st_mode)) {
			print_error(" Is a directory", NULL, NULL);
			return (126);
		}

		if (access(cmd[0], X_OK) == -1) {
			if (errno == EACCES) {
				print_error(" Permission denied", NULL, NULL);
				return (126);
			}
			print_error(" Error accessing file", NULL, NULL);
			return (126);
		}

		execute_cmd_pipes(cmd[0], cmd, envp);
	}
	while (new_path[++i])
	{
		if (with_slash)
		{
			exec_path = ft_strjoin(new_path[i], with_slash);
			if (access(exec_path, F_OK | X_OK) == 0)
				execute_cmd_pipes(exec_path, cmd, envp);
		}
		if (new_path[i][ft_strlen(new_path[i] + 1)] != '/'){
			char	*tmp;
			tmp = new_path[i];
			new_path[i] = ft_strjoin(new_path[i], "/");
			free(tmp);
		}
		exec_path = ft_strjoin(new_path[i], cmd[0]);
		if (access(exec_path, F_OK | X_OK) == 0)
			execute_cmd_pipes(exec_path, cmd, envp);
		else
			free(exec_path);
	}
	print_error(" command not found", NULL, NULL);
	return (127);
}

char	**ft_from_list_to_array_pipes(t_token **token)
{
	t_token	*temp;
	int		i;
	char	**cmd;

	i = 0;
	temp = *token;
	while (temp)
	{
		i++;
		temp = temp->next;
	}
	cmd = (char **)malloc(sizeof(char *) * (i + 1));
	if (!cmd)
		exit(EXIT_FAILURE);
	i = 0;
	temp = *token;
	while (temp)
	{
		cmd[i] = ft_strdup(temp->content);
		temp = temp->next;
		i++;
	}
	cmd[i] = NULL;
	free_linked_list(token);
	return (cmd);
}

void	handle_path_pipes(t_token **token, t_main *main, int **fds)
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
	ret = add_slash_pipes(new_path, cmd, main->env);
	free_mtx(cmd);
	free_mtx(new_path);
	free_all(main, &main->token);
	free(main->pids);
	free_fds(fds, main->pipe_count);
	fds = NULL;
	exit(ret);
}

int	child_pipes(t_token **list, t_main *main, int **fds, int pos)
{
	int	pid1;

	ft_herdoc_pipes_main(list);
	pid1 = fork();
	if (pid1 < 0)
	{
		perror("Fork Failed");
		exit(EXIT_FAILURE);
	}
	if (pid1 == 0)
	{
		if (pos == 1)
		{
			dup2(fds[main->pos_fd][1], STDOUT_FILENO);
			close(fds[main->pos_fd][1]);
			close(fds[main->pos_fd][0]);
		}
		else if (pos == 2)
		{
			dup2(fds[main->pos_fd - 1][0], STDIN_FILENO);
			close(fds[main->pos_fd - 1][0]);

			dup2(fds[main->pos_fd][1], STDOUT_FILENO);
			close(fds[main->pos_fd][1]);
		}
		else if (pos == 3)
		{
			dup2(fds[main->pos_fd - 1][0], STDIN_FILENO);
			close(fds[main->pos_fd - 1][0]);

			dup2(main->orig_fd[1], STDOUT_FILENO);
		}
		ft_redirections_main(list, main);
		if (*list)
			ft_builtins_main_pipes(list, main, fds);
		if (*list)
			handle_path_pipes(list, main, fds);
		free(main->pids);
		free_fds(fds, main->pipe_count);
		fds = NULL;
		free_all(main, list);
		exit(g_global);
	}
	if (pos == 1)
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
	return (pid1);
}

int	ft_exe_main_pipes(t_token **list, t_main *main, int **fds, int pos)
{
	return (child_pipes(list, main, fds, pos));
}