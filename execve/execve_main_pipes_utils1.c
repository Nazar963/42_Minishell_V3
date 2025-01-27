/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_main_pipes_utils1.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 12:47:45 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/26 15:54:44 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	error_file_dir(char *cmd)
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
		return (127);
	}
	if (ft_isdigit(cmd[0]) == 1)
	{
		if (ft_atoi(cmd) == g_global)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putendl_fd(": command not found", 2);
			return (127);
		}
	}
	if (ft_strlen(cmd) == 1 && cmd[0] == '.')
	{
		ft_putstr_fd("minishell: .: filename argument required\n", 2);
		return (2);
	}
	return (0);
}

int	ft_add_slash_pipes_file(char **cmd, char **envp, t_main *main)
{
	struct stat	path_stat;

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
	return (execute_cmd_pipes(cmd[0], cmd, envp, main), 0);
}

char	*add_slash_pipes_norm0(char *new)
{
	char	*temp;

	if (new[ft_strlen(new + 1)] != '/')
	{
		temp = new;
		new = ft_strjoin(new, "/");
		free(temp);
	}
	return (new);
}
