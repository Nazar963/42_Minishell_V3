/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 15:30:09 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/17 19:38:32 by naal-jen         ###   ########.fr       */
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

void	ft_concatenate_var(t_token **token, t_main *main)
{
	t_con	con;

	con.i = -1;
	con.splitted_2 = ft_split((*token)->content, '=');
	while (main->env[++con.i])
	{
		con.temp = ft_strjoin(con.splitted_2[0], "=");
		if (ft_strncmp(main->env[con.i], con.temp, ft_strlen(con.temp)) == 0)
		{
			con.splitted = ft_split(main->env[con.i], '=');
			con.new_str = ft_strjoin(con.splitted[0], "=");
			con.tempos = con.new_str;
			con.new_str = ft_strjoin(con.tempos, con.splitted[1]);
			free(con.tempos);
			con.tempos = con.new_str;
			con.new_str = ft_strjoin(con.new_str, con.splitted_2[1]);
			free(con.tempos);
			free(con.temp);
			free(main->env[con.i]);
			free_mtx(&con.splitted);
			free_mtx(&con.splitted_2);
			main->env[con.i] = ft_strdup(con.new_str);
			free(con.new_str);
			free_linked_list(token);
			return ;
		}
		free(con.temp);
	}
}

char	*ft_clean_join(char *var)
{
	char	*new_str;
	int		i;

	i = 0;
	new_str = (char *)malloc(sizeof(char) * (ft_strlen(var) + 1));
	while (var[i] != '+')
	{
		new_str[i] = var[i];
		i++;
	}
	new_str[i] = '=';
	new_str[++i] = '\0';
	return (new_str);
}

int	ft_check_if_already_exits(char *var, t_main *main)
{
	int		i;
	char	*var_join;

	i = 0;
	var_join = ft_clean_join(var);
	while (main->env[i])
	{
		if (ft_strncmp(main->env[i], var_join, ft_strlen(var_join)) == 0)
			return (free(var_join), 1);
		i++;
	}
	return (free(var_join), 0);
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
			free_orig_linked_list(token);
			free_mtx(&splitted_argument);
			g_global = 1;
			print_error("export: not a valid identifier", NULL, NULL);
			return (1);
		}
		else if (loco == 2 && ft_check_if_already_exits(splitted_argument[0], main))
		{
			ft_clean_var(token);
			ft_concatenate_var(token, main);
			//free_mtx(&splitted_argument);
			return (free_mtx(&splitted_argument), 1);
		}
		ft_clean_var(token);
		free_mtx(&splitted_argument);
	}
	return (0);
}

void	ft_export_pipes(t_token **token, t_main *main)
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
