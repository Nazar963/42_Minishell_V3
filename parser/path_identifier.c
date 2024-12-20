/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_identifier.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:21:45 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/19 20:58:18 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h" 

void	ft_del_first_node(t_token **token)
{
	t_token	*temp;

	if (token == NULL || *token == NULL)
		return;

	temp = *token;
	if ((*token)->next == NULL)
	{
		*token = (*token)->next;
		*token = NULL;
	}
	else {
		*token = (*token)->next;
	}
	free(temp->content);
	free(temp);
}

void	ft_del_node(t_token **token, t_token *delete)
{
	t_token	*temp;

	if (token == NULL || *token == NULL || delete == NULL)
		return;

	if (*token == delete) {
		*token = delete->next;
		free(delete->content);
		free(delete);
		return;
	}

	temp = *token;
	while (temp->next != NULL && temp->next != delete)
		temp = temp->next;
	temp->next = delete->next;
	free(delete->content);
	free(delete);
}

void	ft_echo(t_token **token)
{
	ft_del_first_node(token);
	while (*token && (*token)->type != TOKEN_PIPE)
	{
		if ((*token)->type == TOKEN_OPTION)
		{
			ft_del_first_node(token);
			if (ft_strncmp((*token)->content, "$?", 2) == 0
				&& ft_strlen((*token)->content) == 2)
				printf("%d ", g_global);
			else
				if (*token && (*token)->content)
					printf("%s", (*token)->content);
			ft_del_first_node(token);
		}
		else if ((*token)->type == TOKEN_ARGUMENT && (*token)->next == NULL)
		{
			if (ft_strncmp((*token)->content, "$?", 2) == 0
				&& ft_strlen((*token)->content) == 2)
				printf("%d\n", g_global);
			else
				printf("%s\n", (*token)->content);
			ft_del_first_node(token);
		}
		else if ((*token)->type == TOKEN_ARGUMENT)
		{
			if (ft_strncmp((*token)->content, "$?", 2) == 0
				&& ft_strlen((*token)->content) == 2)
				printf("%d ", g_global);
			else
				printf("%s ", (*token)->content);
			ft_del_first_node(token);
		}
		else
			*token = (*token)->next;
	}
}

void	ft_pwd(t_token **token)
{
	char	*pwd;

	ft_del_first_node(token);
	pwd = getcwd(NULL, 0);
	printf("%s\n", pwd);
	free(pwd);
}

void	ft_cd(t_token **token)
{
	ft_del_first_node(token);
	if (*token && !((*token)->type == 1))
		return ;
	if (!*token || (ft_strncmp((*token)->content, "~", 1) == 0)
	|| ((*token)->content[0] == '|'))
	{
		if (chdir(getenv("HOME")) == -1)
			printf("cd: HOME not set\n");
	}
	else
		if (chdir((*token)->content) == -1)
			printf("cd: %s: No such file or directory\n", (*token)->content);
	ft_del_first_node(token);
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

char **ft_export_variable_reassign(char **env, char *variable, char *value)
{
	int i;
	char *temp;

	i = 0;
	while (env[i])
	{
		temp = ft_strjoin(variable, "=");
		if (ft_strncmp(env[i], temp, ft_strlen(temp)) == 0)
		{
			free(env[i]);
			env[i] = ft_strdup(variable);
			env[i] = ft_strjoin(env[i], "=");
			if (strchr(value, ' '))
			{
				env[i] = ft_strjoin(env[i], "\"");
				env[i] = ft_strjoin(env[i], value);
				env[i] = ft_strjoin(env[i], "\"");
			}
			else
				env[i] = ft_strjoin(env[i], value);
			free(temp);
			return (env);
		}
		else if (ft_strncmp(env[i], variable, ft_strlen(variable)) == 0
				&& env[i][ft_strlen(variable)] == '\0')
		{
			free(env[i]);
			env[i] = ft_strdup(variable);
			env[i] = ft_strjoin(env[i], "=");
			if (strchr(value, ' '))
			{
				env[i] = ft_strjoin(env[i], "\"");
				env[i] = ft_strjoin(env[i], value);
				env[i] = ft_strjoin(env[i], "\"");
			}
			else
				env[i] = ft_strjoin(env[i], value);
			free(temp);
			return (env);
		}
		free(temp);
		i++;
	}
	return (NULL);
}

void	ft_export(t_token **token, t_main *main)
{
	char	**splitted_argument;
	int		len;

	ft_del_first_node(token);
	if (!*token || (*token)->content[0] == '|')
		return (print_env_export(main), (void)0);
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
				main->env = ft_export_variable_reassign(main->env, splitted_argument[0], "");
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
				main->env = ft_export_variable_reassign(main->env, splitted_argument[0], splitted_argument[1]);
			}
			else
			{
				len = ft_strlen_mtx(main->env);
				main->env = ft_realloc(main->env, len + 1);
				main->env[len] = ft_strdup(splitted_argument[0]);
				main->env[len] = ft_strjoin(main->env[len], "=");
				main->env[len] = ft_strjoin(main->env[len], splitted_argument[1]);
			}
		}
		if (splitted_argument)
			free_mtx(splitted_argument);
		ft_del_first_node(token);
	}
	return ;
}

void	ft_unset(t_token **token, t_main *main)
{
	int	i;

	ft_del_first_node(token);
	if (!*token)
		return ;
	while (*token)
	{
		i = 0;
		while (main->env[i])
		{
			if (ft_strncmp(main->env[i], (*token)->content, ft_strlen((*token)->content)) == 0
				&& main->env[i][ft_strlen((*token)->content)] == '=')
			{
				ft_del_first_node(token);
				free(main->env[i]);
				while (main->env[i])
				{
					main->env[i] = main->env[i + 1];
					i++;
				}
				break ;
			}
			else if (ft_strncmp(main->env[i], (*token)->content, ft_strlen((*token)->content)) == 0
				&& main->env[i][ft_strlen((*token)->content)] == '\0')
			{
				printf("hi am here man\n");
				free(main->env[i]);
				while (main->env[i])
				{
					main->env[i] = main->env[i + 1];
					i++;
				}
				break ;
			}
			i++;
			if (main->env[i] == NULL){
				ft_del_first_node(token);
				break ;
			}
		}
	}
}

void	ft_env(t_token **token, t_main *main)
{
	int	i;

	i = -1;
	ft_del_first_node(token);
	// if (token && token->content[0] == '|') //? Can it be that env | something else
	// 	return ;
	while (main->env[++i])
	{
		if (!ft_strchr(main->env[i], '='))
			continue ;
		printf("%s\n", main->env[i]);
	}
}

void	ft_exit(t_token **token)
{
	ft_del_first_node(token);
	if (*token && (*token)->type == 1)
		exit(ft_atoi((*token)->content));
	exit(0);
}

int	ft_check_for_builtin(t_token **token, t_main *main)
{
	if (ft_strncmp((*token)->content, "echo", 4) == 0)
		ft_echo(token);
	else if (ft_strncmp((*token)->content, "pwd", 3) == 0)
		ft_pwd(token);
	else if (ft_strncmp((*token)->content, "cd", 2) == 0)
		ft_cd(token);
	else if (ft_strncmp((*token)->content, "export", 6) == 0)
		ft_export(token, main);
	else if (ft_strncmp((*token)->content, "unset", 5) == 0)
		ft_unset(token, main);
	else if (ft_strncmp((*token)->content, "env", 3) == 0)
		ft_env(token, main);
	else if (ft_strncmp((*token)->content, "exit", 4) == 0)
		ft_exit(token);
	return (0);
}

void	ft_path_identifier(t_token *token, t_main *main)
{
	if (ft_pipes_main(&token, main) == true)
		return ;
	ft_redirections_main(&token, main);
	ft_check_for_builtin(&token, main);
	ft_execve_main(&token, main);
	dup2(main->orig_fd[0], STDIN_FILENO);
	dup2(main->orig_fd[1], STDOUT_FILENO);
}