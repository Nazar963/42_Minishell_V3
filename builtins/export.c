/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 15:30:09 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/25 08:58:58 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_check_for_special_c(char **var)
{
	char	**sp;
	int		i;

	i = 0;
	if (ft_strnstr(var[1], "\t", ft_strlen(var[1])))
	{
		sp = ft_split(var[1], '\t');
		while (sp[i])
		{
			if (i == 0)
				printf("declare -x %s=$\'%s\\t", var[0], sp[i]);
			else if (sp[i + 1])
				printf("%s\\t", sp[i]);
			else
				printf("%s", sp[i]);
			i++;
		}
		printf("'\n");
		free_mtx(&sp);
		return (1);
	}
	return (0);
}

void	print_env_export(t_main *main)
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
		{
			if (ft_check_for_special_c(var) == 0)
				printf("declare -x %s=\"%s\"\n", var[0], var[1]);
		}
		else
			printf("declare -x %s=\"\"\n", var[0]);
		free_mtx(&var);
		i++;
	}
	return ;
}

int	ft_check_for_already_existing_variable(char **env, char *variable)
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

char	**ft_export_var_reassign(char **env, char *variable, char *value)
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

void	ft_export(t_token **token, t_main *main)
{
	char	**splitted_argument;

	ft_del_first_node(token);
	if (!*token || (*token)->content[0] == '|')
		return (print_env_export(main), (void)0);
	if (ft_export_pipes_check_var(token, main) == 1)
		return ;
	while (*token && (*token)->type != 3)
	{
		if (!ft_strchr((*token)->content, '='))
		{
			ft_export_pipes_norm0(token, main);
			continue ;
		}
		splitted_argument = ft_split((*token)->content, '=');
		if (splitted_argument[1] == NULL)
			ft_export_pipes_norm1(token, main, splitted_argument);
		else
			ft_export_pipes_norm2(main, splitted_argument);
		if (splitted_argument)
			free_mtx(&splitted_argument);
		ft_del_first_node(token);
	}
	g_global = 0;
	return ;
}
