/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 15:29:24 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/11 11:44:45 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_update_env(t_main *main)
{
	int		i;
	char	*cwd;

	i = 0;
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return ;
	while (main->env[i])
	{
		if (ft_strncmp(main->env[i], "OLDPWD=", 7) == 0)
		{
			free(main->env[i]);
			main->env[i] = ft_strjoin("OLDPWD=", cwd);
		}
		if (ft_strncmp(main->env[i], "PWD=", 4) == 0)
		{
			free(main->env[i]);
			main->env[i] = ft_strjoin("PWD=", cwd);
		}
		i++;
	}
	free(cwd);
}

void	ft_cd_norm0(t_token **token)
{
	struct stat path_stat;

	// Check if the path exists and get its status
	if (stat((*token)->content, &path_stat) != 0)
	{
		g_global = 1;
		print_error("minishell: cd: ", (*token)->content, ": No such file or directory");
		return;
	}

	// Check if the path is a directory
	if (!S_ISDIR(path_stat.st_mode))
	{
		g_global = 1;
		print_error("minishell: cd: ", (*token)->content, ": Not a directory");
		return;
	}

	// Attempt to change the directory
	if (chdir((*token)->content) == -1)
	{
		g_global = 1;
		print_error("minishell: cd: ", (*token)->content, ": Permission denied");
	}
	else
	{
		g_global = 0;
	}
	// if (chdir((*token)->content) == -1)
	// {
	// 	g_global = 1;
	// 	print_error("minishell: cd: ", (*token)->content, ": No such file or directory");
	// }
	// else
	// 	g_global = 0;
}

void	ft_cd(t_token **token, t_main *main)
{
	ft_del_first_node(token);
	if (*token && !((*token)->type == 1))
		return ;
	if (*token && (*token)->next != NULL)
	{
		print_error("cd: too many arguments", NULL, NULL);
		free_orig_linked_list(token);
		g_global = 1;
		return ;
	}
	if (!*token || (ft_strncmp((*token)->content, "~", 1) == 0)
		|| ((*token)->content[0] == '|'))
	{
		if (chdir(getenv("HOME")) == -1)
		{
			g_global = 1;
			printf("cd: HOME not set\n");
		}
		g_global = 0;
	}
	else
		ft_cd_norm0(token);
	ft_update_env(main);
	ft_del_first_node(token);
}
