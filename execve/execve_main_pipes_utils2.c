/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_main_pipes_utils2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 12:50:23 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/26 15:50:42 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	error_cmd_pipes(char *cmd)
{
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": command not found", 2);
	if (errno == ENOENT)
		exit(127);
	else
		exit(126);
}

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

int	ft_no_special_characters_fun(char **cmd, t_main *main)
{
	int	ret;

	ret = ft_no_special_characters_pipes(cmd[0]);
	if (ret != 0)
		return (ret);
	if (ft_strncmp(cmd[0], "./", 2) == 0 || ft_strrchr(cmd[0], '/'))
		return (ft_add_slash_pipes_file(cmd, main->env, main));
	return (0);
}

int	handle_path_pipes_no_path(t_token **token)
{
	print_error("minishell: ",
		(*token)->content, ": No suck file or directory");
	free_linked_list(token);
	return (127);
}

void	ft_handle_exit_status(int status)
{
	if (status == 131)
	{
		write(2, "Quit (core dumped)\n", 20);
		g_global = 131;
	}
	else if (WIFEXITED(status))
	{
		g_global = WEXITSTATUS(status);
		if (g_global == 7)
			g_global = 0;
	}
	else if (WIFSIGNALED(status))
		g_global = 128 + WTERMSIG(status);
	else
		g_global = 1;
}
