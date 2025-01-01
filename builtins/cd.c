/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 15:29:24 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/29 19:43:30 by naal-jen         ###   ########.fr       */
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
	{
		if (chdir((*token)->content) == -1)
		{
			g_global = 1;
			print_error("cd: No such file or directory", NULL, NULL);
		}
		else
			g_global = 0;
	}
	ft_update_env(main);
	ft_del_first_node(token);
}
