/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 18:52:08 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/01 21:54:56 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_cmd(char *exec_path, char **cmd, t_main *main)
{
	if (execve(exec_path, cmd, main->env) == -1)
	{
		free_mtx(cmd);
		free_all(main, &main->token);
		perror("execve");
		if (errno == ENOENT)
			exit(127);
		else
			exit(126);
	}
}

int	error_file_dir(char *cmd)
{
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": No such file or directory", 2);
	if (errno == ENOENT)
		return (127);
	else
		return (126);
}

int	ft_no_special_characters(char *cmd)
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

int	add_slash(char **new_path, char **cmd, t_main *main)
{
	int		i;
	int		ret;
	char	*exec_path;
	struct stat	path_stat;
	char	*with_slash;

	i = -1;
	ret = ft_no_special_characters(cmd[0]);
	if (ret != 0)
		return (ret);
	with_slash = ft_strrchr(cmd[0], '/');
	if (ft_strncmp(cmd[0], "./", 2) == 0)
	{
		if (stat(cmd[0], &path_stat) == -1)
		{
			if (errno == ENOENT)
				return (error_file_dir(cmd[0]));
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

		execute_cmd(cmd[0], cmd, main);
	}
	if (with_slash)
	{
		if (stat(cmd[0], &path_stat) == -1)
		{
			if (errno == ENOENT)
				return (error_file_dir(cmd[0]));
			print_error("Error retrieving file information", NULL, NULL);
			return(126);
		}

		if (S_ISDIR(path_stat.st_mode)) {
			print_error(" Is a directory", NULL, NULL);
			return(126);
		}

		if (access(cmd[0], X_OK) == -1) {
			if (errno == EACCES) {
				print_error(" Permission denied", NULL, NULL);
				return(126);
			}
			print_error(" Error accessing file", NULL, NULL);
			return(126);
		}

		execute_cmd(cmd[0], cmd, main);
	}
	while (new_path[++i])
	{
		if (with_slash)
		{
			exec_path = ft_strjoin(new_path[i], with_slash);
			if (access(exec_path, F_OK | X_OK) == 0)
				execute_cmd(exec_path, cmd, main);
		}
		if (new_path[i][ft_strlen(new_path[i] + 1)] != '/'){
			char	*tmp;
			tmp = new_path[i];
			new_path[i] = ft_strjoin(new_path[i], "/");
			free(tmp);
		}
		exec_path = ft_strjoin(new_path[i], cmd[0]);
		if (access(exec_path, F_OK | X_OK) == 0)
			execute_cmd(exec_path, cmd, main);
		else
			free(exec_path);
	}
	print_error(" command not found", NULL, NULL);
	return (127);
}

char	**ft_from_list_to_array(t_token **token)
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
	cmd = ft_from_list_to_array(token);
	ret = add_slash(new_path, cmd, main);
	free_mtx(cmd);
	free_mtx(new_path);
	free_all(main, &main->token);
	exit(ret);
}

void	child(t_token **token, t_main *main)
{
	int	status;
	int	pid1;
	int	fd;

	pid1 = fork();
	if ((*token)->heredoc_file)
	{
		fd = open((*token)->heredoc_file, O_RDONLY);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (pid1 < 0)
	{
		perror("Fork Failed");
		exit(EXIT_FAILURE);
	}
	if (pid1 == 0)
		handle_path(token, main);
	waitpid(pid1, &status, 0);
	if (WIFEXITED(status))
		g_global = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_global = 128 + WTERMSIG(status);
	else
		g_global = 1;
	dup2(main->orig_fd[0], STDIN_FILENO);
}

void	ft_execve_main(t_token **token, t_main *main)
{
	child(token, main);
}
