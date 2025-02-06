/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 15:29:24 by naal-jen          #+#    #+#             */
/*   Updated: 2025/02/04 20:37:07 by naal-jen         ###   ########.fr       */
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
	struct stat	path_stat;

	if (stat((*token)->content, &path_stat) != 0)
	{
		g_global = 1;
		print_error("minishell: cd: ", (*token)->content,
			": No such file or directory");
		return ;
	}
	if (!S_ISDIR(path_stat.st_mode))
	{
		g_global = 1;
		print_error("minishell: cd: ", (*token)->content, ": Not a directory");
		return ;
	}
	if (chdir((*token)->content) == -1)
	{
		g_global = 1;
		print_error("minishell: cd: ", (*token)->content,
			": Permission denied");
	}
	else
		g_global = 0;
}

int	check_for_home(t_main *main)
{
	int	i;

	i = 0;
	while (main->env[i])
	{
		if (ft_strncmp(main->env[i], "HOME=", 5) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	ft_cd(t_token **token, t_main *main)
{
	ft_del_first_node(token);
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
		if (check_for_home(main) && chdir(ft_getenv("HOME", main)) == 0)
			g_global = 0;
		else
		{
			g_global = 1;
			print_error("minishell: cd: HOME not set", NULL, NULL);
		}
	}
	else
		ft_cd_norm0(token);
	ft_update_env(main);
	ft_del_first_node(token);
}
