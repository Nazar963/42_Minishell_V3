/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_main_pipes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 18:52:08 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/19 10:38:14 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_cmd_pipes(char *exec_path, char **cmd, char **envp)
{
	if (execve(exec_path, cmd, envp) == -1)
	{
		free_mtx(cmd);
		exit(EXIT_FAILURE);
	}
}

void	error_cmd_pipes(char *cmd)
{
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": command not found", 2);
	exit(EXIT_FAILURE);
}

void	add_slash_pipes(char **new_path, char **cmd, char **envp)
{
	int		i;
	char	*exec_path;
	char	*with_slash;

	i = -1;
	with_slash = ft_strrchr(cmd[0], '/');
	while (new_path[++i])
	{
		if (with_slash)
		{
			exec_path = ft_strjoin(new_path[i], with_slash);
			if (access(exec_path, F_OK | X_OK) == 0)
				execute_cmd_pipes(exec_path, cmd, envp);
		}
		if (new_path[i][ft_strlen(new_path[i] + 1)] != '/')
			new_path[i] = ft_strjoin(new_path[i], "/");
		exec_path = ft_strjoin(new_path[i], cmd[0]);
		if (access(exec_path, F_OK | X_OK) == 0)
			execute_cmd_pipes(exec_path, cmd, envp);
		else
			free(exec_path);
	}
	error_cmd_pipes(cmd[0]);
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
	while (*token)
	{
		cmd[i] = ft_strdup((*token)->content);
		*token = (*token)->next;
		i++;
	}
	cmd[i] = NULL;
	return (cmd);
}

void	handle_path_pipes(t_token **token, char **envp)
{
	int		i;
	char	**new_path;
	char	**cmd;

	i = 0;
	while (ft_strnstr(envp[i], "PATH=", 5) == 0)
		i++;
	new_path = ft_split(envp[i] + 5, ':');
	cmd = ft_from_list_to_array_pipes(token);
	// cmd = ft_split(argv[kind], ' ');
	add_slash_pipes(new_path, cmd, envp);
}

void	handler_pipes(int case_num)
{
	if (case_num == 1)
	{
		perror("Fork Failed");
		exit(EXIT_FAILURE);
	}
	if (case_num == 2)
	{
		perror("Pipe Failed");
		exit(EXIT_FAILURE);
	}
	if (case_num == 3)
	{
		ft_putstr_fd("Check How Many Arguments You Have", 2);
		exit(EXIT_FAILURE);
	}
	return ;
}

void	child_pipes(t_token **list, t_main *main, int **fds, int pos)
{
	int	pid1;

	pid1 = fork();
	if (pid1 < 0)
		handler_pipes(1);
	if (pid1 == 0)
	{
		if (pos == 1)
		{
			dup2(fds[main->pos_fd][1], STDOUT_FILENO);
			close(fds[main->pos_fd][0]);
		}
		else if (pos == 2)
		{
			dup2(fds[main->pos_fd - 1][0], STDIN_FILENO);
			dup2(fds[main->pos_fd][1], STDOUT_FILENO);
		}
		else if (pos == 3)
		{
			dup2(fds[main->pos_fd - 1][0], STDIN_FILENO);
			dup2(main->orig_fd[1], STDOUT_FILENO);
			close(fds[main->pos_fd][1]);
		}
		// close(fds[main->pos_fd][0]);
		// close(fds[main->pos_fd][1]);
		ft_redirections_main(list, main);
		ft_check_for_builtin(list, main);
		handle_path_pipes(list, main->env);
		exit(0);
	}
	// close(fds[main->pos_fd][0]);
	// close(fds[main->pos_fd][1]);
	waitpid(pid1, NULL, 0);
}

void	ft_execve_main_pipes(t_token **list, t_main *main, int **fds, int pos)
{
	child_pipes(list, main, fds, pos);
}