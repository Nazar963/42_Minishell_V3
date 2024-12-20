/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 18:52:08 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/19 20:41:20 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_cmd(char *exec_path, char **cmd, char **envp)
{
	if (execve(exec_path, cmd, envp) == -1)
	{
		free_mtx(cmd);
		exit(127);
	}
}

void	error_cmd(char *cmd)
{
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": command not found", 2);
	exit(EXIT_FAILURE);
}

void	add_slash(char **new_path, char **cmd, char **envp)
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
				execute_cmd(exec_path, cmd, envp);
		}
		if (new_path[i][ft_strlen(new_path[i] + 1)] != '/')
			new_path[i] = ft_strjoin(new_path[i], "/");
		exec_path = ft_strjoin(new_path[i], cmd[0]);
		if (access(exec_path, F_OK | X_OK) == 0)
			execute_cmd(exec_path, cmd, envp);
		else
			free(exec_path);
	}
	error_cmd(cmd[0]);
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
	while (*token)
	{
		cmd[i] = ft_strdup((*token)->content);
		*token = (*token)->next;
		i++;
	}
	cmd[i] = NULL;
	return (cmd);
}

void	handle_path(t_token **token, char **envp)
{
	int		i;
	char	**new_path;
	char	**cmd;

	i = 0;
	while (ft_strnstr(envp[i], "PATH=", 5) == 0)
		i++;
	new_path = ft_split(envp[i] + 5, ':');
	cmd = ft_from_list_to_array(token);
	// cmd = ft_split(argv[kind], ' ');
	add_slash(new_path, cmd, envp);
}

void	handler(int case_num)
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

void	child(t_token **token, char **envp)
{
	int	status;
	int	pid1;

	// if (pipe(fd) == -1)
	// 	handler(2);
	pid1 = fork();
	if (pid1 < 0)
		handler(1);
	if (pid1 == 0)
	{
		// dup2(input_file, 0);
		// dup2(fd[1], 1);
		// close(fd[0]);
		// close(output_file);
		handle_path(token, envp);

	}
	// waitpid(pid1, NULL, 0);
	waitpid(pid1, &status, 0);
	// if (WIFEXITED(status))
	// {
	// 	if (status == 512)
	// 		g_global = 2;
	// 	// else if (status == 32512 && ft_strnstr(inputs[0], "./", 2)) //! Remember for which error do i need to check for this case.
	// 	// 	g_global = 126;
	// 	else if (status == 32512)
	// 		g_global = 127;
	// 	else if (status == 256)
	// 		g_global = 127;
	// }
	if (WIFEXITED(status))
		g_global = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_global = 128 + WTERMSIG(status); // Conventionally, 128 + signal number
	else
		g_global = 1; // General error
}

void	ft_execve_main(t_token **token, t_main *main)
{
	child(token, main->env);
}