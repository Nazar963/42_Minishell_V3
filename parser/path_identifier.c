/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_identifier.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:21:45 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/23 16:23:07 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h" 
#include <dirent.h>

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

void expand_wildcard() {
	DIR *dir;
	struct dirent *entry;

	dir = opendir(".");
	if (dir == NULL) {
		perror("opendir");
		return;
	}

	while ((entry = readdir(dir)) != NULL) {
		// Skip the "." and ".." entries
		if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
			printf("%s ", entry->d_name);
		}
	}
	closedir(dir);
}

void	ft_echo(t_token **token)
{
	ft_del_first_node(token);
	if (!*token)
		printf("\n");
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
			g_global = 0;
			ft_del_first_node(token);
		}
		else if ((*token)->type == TOKEN_ARGUMENT && (*token)->next == NULL)
		{
			if (ft_strncmp((*token)->content, "$?", 2) == 0
				&& ft_strlen((*token)->content) == 2)
				printf("%d\n", g_global);
			// else if (ft_strncmp((*token)->content, "*", 1) == 0
			// 	&& ft_strlen((*token)->content) == 1)
			// 	{
			// 		expand_wildcard();
			// 		printf("\n");
			// 		ft_del_first_node(token);
			// 	}
			else
				printf("%s\n", (*token)->content);
			g_global = 0;
			ft_del_first_node(token);
		}
		else if ((*token)->type == TOKEN_ARGUMENT)
		{
			if (ft_strncmp((*token)->content, "$?", 2) == 0
				&& ft_strlen((*token)->content) == 2)
				printf("%d ", g_global);
			else
				printf("%s ", (*token)->content);
			g_global = 0;
			ft_del_first_node(token);
		}
		else
			*token = (*token)->next;
	}
}

void	ft_update_env(t_main *main)
{
	int i;

	i = 0;
	while (main->env[i])
	{
		if (ft_strncmp(main->env[i], "OLDPWD=", 7) == 0)
		{
			free(main->env[i]);
			main->env[i] = ft_strjoin("OLDPWD=", getcwd(NULL, 0));
		}
		if (ft_strncmp(main->env[i], "PWD=", 4) == 0)
		{
			free(main->env[i]);
			main->env[i] = ft_strjoin("PWD=", getcwd(NULL, 0));
		}
		i++;
	}
}

void	ft_pwd(t_token **token)
{
	char	*pwd;

	ft_del_first_node(token);
	pwd = getcwd(NULL, 0);
	printf("%s\n", pwd);
	free(pwd);
	while (*token && (*token)->type != TOKEN_PIPE)
		ft_del_first_node(token);
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
	g_global = 0;
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
	while (main->env[++i])
	{
		if (!ft_strchr(main->env[i], '='))
			continue ;
		printf("%s\n", main->env[i]);
	}
}

int	ft_correct_exit_status_calc(int val)
{
	int	mod_value;

	mod_value = val % 256;
	if (mod_value < 0)
		mod_value += 256;

	return (val);
}

void	ft_exit(t_token **token)
{
	ft_del_first_node(token);
	if (*token && (*token)->type == 1 && (*token)->next == NULL)
	{
		if (ft_control_int((*token)->content) == 0)
		{
			printf("exit\n");
			print_error("bash: exit: ", (*token)->content, ": numeric argument required");
			exit(2);
		}
		else
			exit(ft_atoi((*token)->content));
	}
	else if (*token)
	{
		if (ft_control_int((*token)->content) == 0)
		{
			printf("exit\nbash: exit: %s: numeric argument required\n", (*token)->content);
			exit(2);
		}
		else if (ft_atoi((*token)->content) < 0)
		{
			printf("exit\n");
			exit (ft_correct_exit_status_calc(ft_atoi((*token)->content)));
		}
		print_error("bash: exit: too many arguments", NULL, NULL);
		exit(1);
	}
	exit(0);
}

int	ft_check_for_builtin(t_token **token, t_main *main)
{
	int	len;

	len = ft_strlen((*token)->content);
	if (ft_strncmp((*token)->content, "echo", len) == 0)
		ft_echo(token);
	else if (ft_strncmp((*token)->content, "pwd", len) == 0)
		ft_pwd(token);
	else if (ft_strncmp((*token)->content, "cd", len) == 0)
		ft_cd(token, main);
	else if (ft_strncmp((*token)->content, "export", len) == 0)
		ft_export(token, main);
	else if (ft_strncmp((*token)->content, "unset", len) == 0)
		ft_unset(token, main);
	else if (ft_strncmp((*token)->content, "env", len) == 0)
		ft_env(token, main);
	else if (ft_strncmp((*token)->content, "exit", len) == 0)
		ft_exit(token);
	return (0);
}

void	ft_handle_exit_status_command_main(t_token **token)
{
	if (ft_strncmp((*token)->content, "$?", 2) == 0
		&& ft_strlen((*token)->content) == 2
		&& ft_lstsize(*token) == 1)
	{
		printf("%d: command not found\n", g_global);
		ft_del_first_node(token);
	}
}

void	ft_path_identifier(t_token *token, t_main *main)
{
	if (ft_pipes_main(&token, main) == true)
		return ;
	ft_handle_exit_status_command_main(&token);
	if (token)
		ft_redirections_main(&token, main);
	if (token)
		ft_check_for_builtin(&token, main);
	if (token)
		ft_execve_main(&token, main);
	dup2(main->orig_fd[0], STDIN_FILENO);
	dup2(main->orig_fd[1], STDOUT_FILENO);
}