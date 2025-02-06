/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 15:30:09 by naal-jen          #+#    #+#             */
/*   Updated: 2025/02/06 19:41:31 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_env_export_pipes(t_main *main)
{
	char	**var;
	int		i;

	i = 0;
	while (main->env[i])
	{
		var = ft_split(main->env[i], '=');
		if (!ft_strchr(main->env[i], '='))
			printf("declare -x %s\n", var[0]);
		else if (var[1] && var[1][0] && var[1][0] == '\"')
			printf("declare -x %s=%s\n", var[0], var[1]);
		else if (var[1])
			printf("declare -x %s=\"%s\"\n", var[0], var[1]);
		else
			printf("declare -x %s=\"\"\n", var[0]);
		free_mtx(&var);
		i++;
	}
	return ;
}

int	ft_check_already_existing_var_pipes(char **env, char *variable)
{
	int		i;
	char	*temp;

	i = 0;
	while (env[i])
	{
		temp = ft_strjoin(variable, "=");
		if (ft_strncmp(env[i], temp, ft_strlen(temp)) == 0)
		{
			free(temp);
			return (1);
		}
		else if (ft_strncmp(env[i], variable, ft_strlen(variable)) == 0
			&& env[i][ft_strlen(variable)] == '\0')
		{
			free(temp);
			return (1);
		}
		free(temp);
		i++;
	}
	return (0);
}

char	**ft_export_var_reassign_p(char **env, char *variable, char *value)
{
	int		i;
	char	*temp;

	i = -1;
	while (env[++i])
	{
		temp = ft_strjoin(variable, "=");
		if (ft_strncmp(env[i], temp, ft_strlen(temp)) == 0
			|| (ft_strncmp(env[i], variable, ft_strlen(variable)) == 0
				&& env[i][ft_strlen(variable)] == '\0'))
		{
			free(temp);
			free(env[i]);
			temp = ft_strdup(variable);
			env[i] = ft_strjoin(temp, "=");
			free(temp);
			if (strchr(value, ' '))
				env[i] = ft_export_var_reassign_p_norm0(env[i], value);
			else
				env[i] = ft_strjoin_mod(env[i], value);
			return (env);
		}
		free(temp);
	}
	return (NULL);
}

int	ft_export_pipes_check_var(t_token **token, t_main *main)
{
	char	**splitted_argument;
	int		loco;

	splitted_argument = ft_split((*token)->content, '=');
	if (splitted_argument)
	{
		loco = is_valid_var_name_pipes(splitted_argument[0]);
		if (loco == 0)
		{
			print_error_helpering(splitted_argument[0], token);
			free_mtx(&splitted_argument);
			g_global = 1;
			return (1);
		}
		else if (loco == 2 && ft_check_if_exits(splitted_argument[0], main))
		{
			ft_clean_var(token);
			ft_concatenate_var(token, main);
			return (free_mtx(&splitted_argument), 1);
		}
		ft_clean_var(token);
		free_mtx(&splitted_argument);
	}
	return (0);
}

void	ft_export_pipes(t_token **token, t_main *main)
{
	int		flag;

	flag = FALSE;
	ft_del_first_node(token);
	if (!*token || (*token)->content[0] == '|')
		return (print_env_export(main), (void)0);
	while (*token && (*token)->type != 3)
	{
		if (ft_export_pipes_check_var(token, main) == 1)
		{
			ft_del_first_node(token);
			continue ;
		}
		if (!ft_strchr((*token)->content, '='))
		{
			ft_export_pipes_norm0(token, main);
			flag = TRUE;
			continue ;
		}
		ft_export_pipes_variable_handling(token, main);
		ft_del_first_node(token);
	}
	ft_g_global_final_assigning(flag);
	return ;
}
