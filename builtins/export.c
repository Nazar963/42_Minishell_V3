/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 15:30:09 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/05 13:51:17 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_env_export(t_main *main)
{
	char	**var;
	int		i;

	i = 0;
	while (main->env[i])
	{
		var = ft_split(main->env[i], '=');
		// if (var[1] && var[1][0] && ft_strncmp(main->env[i], "hi", 2) == 0)
			// printf("thello this is the result: var[0]: %c\n ", var[1][0]);
			// printf("thello this is the result: var[0]: %s,  var[1]: %s\n ", var[0], var[1]);
		if (!ft_strchr(main->env[i], '='))
			printf("declare -x %s\n", var[0]);
		else if (var[1] && var[1][0] && var[1][0] == '\"'/*  && var[1][ft_strlen(var[1] - 2)] == '\"' */) //* Which means the variable was first assigned to " " and so it has the " already inside.
			printf("declare -x %s=%s\n", var[0], var[1]);
		else if (var[1])
			printf("declare -x %s=\"%s\"\n", var[0], var[1]);
		else
			printf("declare -x %s=\"\"\n", var[0]);
		free_mtx(var);
		i++;
	}
	return ;
}

int	ft_check_for_already_existing_variable(char **env, char *variable)
{
	int i;
	char *temp;

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

char **ft_export_var_reassign(char **env, char *variable, char *value)
{
	int		i;
	char	*temp;
	char	*join_helper;

	i = 0;
	while (env[i])
	{
		temp = ft_strjoin(variable, "=");
		if (ft_strncmp(env[i], temp, ft_strlen(temp)) == 0)
		{
			free(temp);
			free(env[i]);
			join_helper = ft_strdup(variable);
			env[i] = ft_strjoin(join_helper, "=");
			free(join_helper);
			char *hi;
			if (strchr(value, ' '))
			{
				hi = env[i];
				env[i] = ft_strjoin(env[i], "\"");
				free (hi);

				hi = env[i];
				env[i] = ft_strjoin(env[i], value);
				free (hi);

				hi = env[i];
				env[i] = ft_strjoin(env[i], "\"");
				free (hi);
			}
			else
			{
				hi = env[i];
				env[i] = ft_strjoin(env[i], value);
				free (hi);
			}
			return (env);
		}
		else if (ft_strncmp(env[i], variable, ft_strlen(variable)) == 0
				&& env[i][ft_strlen(variable)] == '\0')
		{
			free(temp);
			free(env[i]);
			join_helper = ft_strdup(variable);
			if (join_helper)
			{
				env[i] = ft_strjoin(join_helper, "=");
				free(join_helper);
				if (env[i])
				{
					char *temp = env[i];
					if (strchr(value, ' '))
					{
						env[i] = ft_strjoin(env[i], "\"");
						free(temp);
						temp = env[i];
						env[i] = ft_strjoin(env[i], value);
						free(temp);
						temp = env[i];
						env[i] = ft_strjoin(env[i], "\"");
						free(temp);
					}
					else
					{
						env[i] = ft_strjoin(env[i], value);
						free(temp);
					}
				}
			}
			// env[i] = ft_strjoin(join_helper, "=");
			// free(join_helper);
			// if (strchr(value, ' '))
			// {
			// 	env[i] = ft_strjoin(env[i], "\"");
			// 	env[i] = ft_strjoin(env[i], value);
			// 	env[i] = ft_strjoin(env[i], "\"");
			// }
			// else
			// 	env[i] = ft_strjoin(env[i], value);
			// free(temp);
			return (env);
		}
		free(temp);
		i++;
	}
	return (NULL);
}

int	is_valid_var_name(const char *name)
{
	int	i;

	i = 0;
	if (!name || name[0] == '\0')
		return 0;

	if (!(ft_isalpha((unsigned char)name[0]) || name[0] == '_'))
		return 0;

	while (name[++i] != '\0' && name[i] != '=')
	{
		if (!(ft_isalnum((unsigned char)name[i]) || name[i] == '_'))
			return 0;
	}
	return 1;
}

void	ft_export(t_token **token, t_main *main)
{
	char	**splitted_argument;
	int		len;

	ft_del_first_node(token);
	if (!*token || (*token)->content[0] == '|')
		return (print_env_export(main), (void)0);
	splitted_argument = ft_split((*token)->content, '=');
	if (splitted_argument)
	{
		// if (ft_strchr(splitted_argument[0], '-'))
		if (is_valid_var_name(splitted_argument[0]) == 0)
		{
			free_orig_linked_list(token);
			free_mtx(splitted_argument);
			g_global = 1;
			print_error("export: not a valid identifier", NULL, NULL);
			return ;
		}
		free_mtx(splitted_argument);
	}
	while (*token && (*token)->type != 3)
	{
		if (!ft_strchr((*token)->content, '='))
		{
			//TODO: in case the variable exists dont do anything
			if (ft_check_for_already_existing_variable(main->env, (*token)->content))
			{
				ft_del_first_node(token);
				continue ;
			}
			len = ft_strlen_mtx(main->env);
			main->env = ft_realloc(main->env, len + 1);
			main->env[len] = ft_strdup((*token)->content);
			ft_del_first_node(token);
			continue ;
		}
		splitted_argument = ft_split((*token)->content, '=');
		if (splitted_argument[1] == NULL)
		{
			//TODO: if the variable exists then remove the value and set it to "" 
			if (ft_check_for_already_existing_variable(main->env, splitted_argument[0]))
			{
				main->env = ft_export_var_reassign(main->env, splitted_argument[0], "");
			}
			else
			{
				len = ft_strlen_mtx(main->env);
				// printf("here am here before: %d\n", ft_strlen_mtx(main->env));
				main->env = ft_realloc(main->env, len + 1);
				// printf("here am here after: %d\n", ft_strlen_mtx(main->env));
				main->env[len] = ft_strdup((*token)->content);
			}
		}
		else
		{
			//TODO: if the variable exists then remove the value and set it to the new value
			if (ft_check_for_already_existing_variable(main->env, splitted_argument[0]))
			{
				main->env = ft_export_var_reassign(main->env, splitted_argument[0], splitted_argument[1]);
			}
			else
			{
				len = ft_strlen_mtx(main->env);
				main->env = ft_realloc(main->env, len + 1);
				main->env[len] = ft_strdup(splitted_argument[0]);
				char *tmp = main->env[len];
				main->env[len] = ft_strjoin(main->env[len], "=");
				free (tmp);
				tmp = main->env[len];
				main->env[len] = ft_strjoin(main->env[len], splitted_argument[1]);
				free (tmp);
			}
		}
		if (splitted_argument)
			free_mtx(splitted_argument);
		ft_del_first_node(token);
	}
	g_global = 0;
	return ;
}
