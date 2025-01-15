/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_main_pipes_utils2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 12:50:23 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/15 19:50:48 by naal-jen         ###   ########.fr       */
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
		return (ft_add_slash_pipes_file(cmd, main->env));
	return (0);
}
