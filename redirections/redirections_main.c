/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakoriko <nakoriko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 21:26:55 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/03 13:54:47 by nakoriko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_redirection_out(t_token **token, t_main *main, t_token *temp)
{
	int	fd;
	// struct stat	file_stat;

	(void)main;
	ft_del_node(token, temp);
	temp = *token;
	while (temp)
	{
		if (temp->type == TOKEN_FILENAME)
		{
			// if (stat(temp->content, &file_stat) == 0)
			if (access(temp->content, W_OK) < 0)
			{
				// if ((file_stat.st_mode & S_IWUSR) == 0)
				if (errno != ENOENT) // If the error is not "No such file or directory"
				{
					ft_free_linked_list_until_pipe(token);
					perror("access");
					g_global = 1;
					return (1);
				}
			}
			fd = open(temp->content, O_WRONLY | O_CREAT | O_TRUNC, 0666);
			if (fd < 0)
			{
				ft_free_linked_list_until_pipe(token);
				perror("open");
				g_global = 1;
				return (1);
			}
			ft_del_node(token, temp);
			dup2(fd, STDOUT_FILENO);
			close(fd);
			return (0);
		}
		temp = temp->next;
	}
	return (0);

}

int	ft_redirection_in(t_token **token, t_main *main, t_token *temp)
{
	int	fd;

	(void)main;
	ft_del_node(token, temp);
	temp = *token;
	while (temp)
	{
		if (temp->type == TOKEN_FILENAME)
		{
			fd = open(temp->content, O_RDONLY);
			if (fd < 0)
			{
				ft_free_linked_list_until_pipe(token);
				perror("open");
				g_global = 1;
				return (1);
			}
			ft_del_node(token, temp);
			dup2(fd, STDIN_FILENO);
			close(fd);
			return (0);
		}
		temp = temp->next;
	}
	return (0);
}

int	ft_append_out(t_token **token, t_main *main, t_token *temp)
{
	int	fd;

	(void)main;
	ft_del_node(token, temp);
	temp = *token;
	while (temp)
	{
		if (temp->type == TOKEN_FILENAME)
		{
			if (access(temp->content, W_OK) < 0)
			{
				// if ((file_stat.st_mode & S_IWUSR) == 0)
				if (errno != ENOENT) // If the error is not "No such file or directory"
				{
					ft_free_linked_list_until_pipe(token);
					perror("access");
					g_global = 1;
					return (1);
				}
			}
			fd = open(temp->content, O_WRONLY | O_CREAT | O_APPEND, 0666);
			if (fd < 0) //TODO: Check how you want to create this error
			{
				ft_free_linked_list_until_pipe(token);
				perror("open");
				g_global = 1;
				return (1);
			}
			ft_del_node(token, temp);
			dup2(fd, STDOUT_FILENO);
			close(fd);
			return (0);
		}
		temp = temp->next;
	}
	return (0);
}

char	*ft_getenv(char *name, t_main *main)
{
    int		i;
    int		j;

    i = 0;
    while (main->env[i])
    {
        j = 0;
        while (main->env[i][j] == name[j] && main->env[i][j] != '=')
            j++;
        if (main->env[i][j] == '=')
        {
            return (main->env[i] + j + 1);
        }
        i++;
    }
    return ("");
}

// char	*ft_expaned_var(char *str, t_main *main)
// {
// 	char	*new_str;
// 	char	*var;
// 	int		i;
// 	int		j;
// 	int		y;

// 	i = 0;
// 	j = -1;
// 	y = -1;
// 	new_str = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1000));
// 	var = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1000));
// 	while (str[i])
// 	{
// 		if (str[i] == '$' && str[i + 1] == '?')
// 		{
// 			new_str = ft_strjoin(new_str, ft_itoa(g_global));
// 			j = ft_strlen(new_str) - 1;
// 			++i;
// 		}
// 		else if (str[i] == '$')
// 		{
// 			new_str[++j] = '\0';
// 			i = i + 2;
// 			while (str[i] && str[i] != ' ')
// 				var[++y] = str[i++];
// 			var[++y] = '\0';
// 			new_str = ft_strjoin(new_str, ft_getenv(var, main));
// 			free(var);
// 			var = NULL;
// 			y = -1;
// 			j = ft_strlen(new_str) - 1;
// 		}
// 		else
// 			new_str[++j] = str[i];
// 		i++;
// 	}
// 	if (var)
// 		free(var);
// 	new_str[++j] = '\0';
// 	return (new_str);
// }

char	*ft_expaned_var(char *str, t_main *main)
{
    char	*new_str;
    char	*var;
    int		i;
    int		j;
    int		y;

    i = 0;
    j = 0;
    y = 0;
    new_str = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1000));
    if (!new_str)
        return (NULL);
    var = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1000));
    if (!var)
    {
        free(new_str);
        return (NULL);
    }
    while (str[i])
    {
        if (str[i] == '$' && str[i + 1] == '?')
        {
            char *temp = ft_itoa(g_global);
            char *joined_str = ft_strjoin(new_str, temp);
            free(temp);
            free(new_str);
            new_str = joined_str;
            j = ft_strlen(new_str);
            i += 2;
        }
        else if (str[i] == '$')
        {
            new_str[j] = '\0';
            i++;
            y = 0;
            while (str[i] && str[i] != ' ')
                var[y++] = str[i++];
            var[y] = '\0';
            const char *env_var = ft_getenv(var, main);
            char *env_var_copy = ft_strdup(env_var);
            char *joined_str = ft_strjoin(new_str, env_var_copy);
            free(env_var_copy);
            free(new_str);
            new_str = joined_str;
            j = ft_strlen(new_str);
        }
        else
        {
            new_str[j++] = str[i++];
        }
    }
    new_str[j] = '\0';
    free(var);
    return (new_str);
}

void	ft_heredoc(t_token **token, t_main *main, t_token *temp)
{
	char	*heredoc_input;
	char	*new_input;
	int		fd;
	t_delimeter	*delimeter;

	ft_del_node(token, temp);
	temp = *token;
	fd = open("/tmp/heredoc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	(*token)->heredoc_file = ft_strdup("/tmp/heredoc.txt");
	temp = temp->next;
	delimeter = ft_lstnew_delimeter(temp->content);
	delimeter->expaned = temp->expaned_del;
	ft_del_node(token, temp);
	temp = *token;
	while (temp)
	{
		if (temp->type == TOKEN_DELIMITER)
		{
			ft_lstadd_back_delimeter(&delimeter, ft_lstnew_delimeter(temp->content));
			delimeter->expaned = temp->expaned_del;
			ft_del_node(token, temp);
			temp = *token;
		}
		else if (temp->type == TOKEN_HEREDOC)
		{
			ft_del_node(token, temp);
			temp = *token;
		}
		if (temp)
			temp = temp->next;
	}
	g_global = 666;
	// set_signals_heredoc(main, token, delimeter);
	while (1)
	{
		heredoc_input = readline("> ");
		if (g_global == 130)
		{
			free(heredoc_input);
			free_linked_list_delimeter(&delimeter);
			close(fd);
			free_linked_list(token);
			dup2(main->orig_fd[0], STDIN_FILENO);
			g_global = 0;
			rl_done = 0;
			return ;
		}
		if (!heredoc_input)
		{
			printf("bash: warning: here-document at line 31 delimited by end-of-file (wanted`%s')\n", delimeter->delimeter);
			free_linked_list_delimeter(&delimeter);
			g_global = 0;
			rl_done = 0;
			return ;
		}
		if (ft_strncmp(heredoc_input, delimeter->delimeter, ft_strlen(delimeter->delimeter)) == 0
			&& ft_strlen(heredoc_input) == ft_strlen(delimeter->delimeter) && ft_lstsize_delimeter(delimeter) == 1)
		{
			free(heredoc_input);
			free_linked_list_delimeter(&delimeter);
			g_global = 0;
			rl_done = 0;
			return ;
		}
		if (ft_strncmp(heredoc_input, delimeter->delimeter, ft_strlen(delimeter->delimeter)) == 0
			&& ft_strlen(heredoc_input) == ft_strlen(delimeter->delimeter) && ft_lstsize_delimeter(delimeter) != 1)
		{
			ft_del_first_node_delimeter(&delimeter);
		}
		else if (ft_lstsize_delimeter(delimeter) == 1)
		{
			if (delimeter->expaned)
			{
				new_input = ft_expaned_var(heredoc_input, main);
				ft_putstr_fd(new_input, fd);
				ft_putstr_fd("\n", fd);
				free (new_input);
			}
			else
			{
				ft_putstr_fd(heredoc_input, fd);
				ft_putstr_fd("\n", fd);
			}
		}
		free(heredoc_input);
	}
}

// void	ft_heredoc(t_token **token, t_main *main, t_token *temp) //* FIRST
// {
// 	char	*heredoc_input;
// 	int		fd;
// 	t_delimeter	*delimeter;

// 	delimeter = NULL;
// 	(void) main;
// 	ft_del_node(token, temp);
// 	temp = *token;
// 	fd = open("/tmp/heredoc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
// 	g_global = 666;
// 	while (temp)
// 	{
// 		if (temp->type == TOKEN_DELIMITER)
// 		{
// 			set_signals_heredoc(main, token, delimeter);
// 			while (1)
// 			{
// 				heredoc_input = readline("> ");
// 				if (!heredoc_input || g_global == 130)
// 				{
// 					// printf("hello you there\n");
// 					ft_del_node(token, temp);
// 					temp = *token;
// 					close(fd);
// 					free(heredoc_input);
// 					fd = open("/tmp/heredoc.txt", O_RDONLY);
// 					if (fd < 0) {
// 						perror("open");
// 						return;
// 					}

// 					if (dup2(fd, STDIN_FILENO) < 0) {
// 						perror("dup2");
// 						close(fd);
// 						return;
// 					}
// 					return ;
// 				}
// 				if (!heredoc_input)
// 				{
// 					printf("bash: warning: here-document at line 31 delimited by end-of-file (wanted`%s')\n", temp->content);
// 					ft_del_node(token, temp);
// 					temp = *token;
// 					break ;
// 				}
// 				if (ft_strncmp(heredoc_input, temp->content, ft_strlen(temp->content)) == 0
// 					&& ft_strlen(heredoc_input) == ft_strlen(temp->content))
// 				{
// 					free(heredoc_input);
// 					ft_del_node(token, temp);
// 					temp = *token;
// 					break ;
// 				}
// 				ft_putstr_fd(heredoc_input, fd);
// 				ft_putstr_fd("\n", fd);
// 				free(heredoc_input);
// 			}
// 		}
// 		temp = temp->next;
// 	}
// 	fd = open("/tmp/heredoc.txt", O_RDONLY);
// 	if (fd < 0) {
// 		perror("open");
// 		return;
// 	}

// 	if (dup2(fd, STDIN_FILENO) < 0) {
// 		perror("dup2");
// 		close(fd);
// 		return;
// 	}

// 	close(fd);
// }


// void	ft_heredoc(t_token **token, t_main *main, t_token *temp) //* SECOND
// {
// 	int			fd;
// 	int			pid;
// 	int			status;
// 	char		*heredoc_input;
// 	t_delimeter	*delimeter;

// 	(void) main;
// 	ft_del_node(token, temp);
// 	temp = *token;
// 	fd = open("/tmp/heredoc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
// 	temp->heredoc_file = ft_strdup("/tmp/heredoc.txt");
// 	temp = temp->next;
// 	delimeter = ft_lstnew_delimeter(temp->content);
// 	ft_del_node(token, temp);
// 	temp = *token;
// 	while (temp)
// 	{
// 		if (temp->type == TOKEN_DELIMITER)
// 		{
// 			ft_lstadd_back_delimeter(&delimeter, ft_lstnew_delimeter(temp->content));
// 			ft_del_node(token, temp);
// 			temp = *token;
// 		}
// 		else if (temp->type == TOKEN_HEREDOC)
// 		{
// 			ft_del_node(token, temp);
// 			temp = *token;
// 		}
// 		if (temp)
// 			temp = temp->next;
// 	}
// 	g_global = 666;
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		set_signals_heredoc(main, token, delimeter);
// 		while (1)
// 		{
// 			heredoc_input = readline("> ");
// 			if (g_global == 130)
// 			{
// 				// printf("exit man am out\n");
// 				free(heredoc_input);
// 				free_all(main, token);
// 				// free_linked_list(token);
// 				free_linked_list_delimeter(&delimeter);
// 				exit(130);
// 			}
// 			if (!heredoc_input)
// 			{
// 				printf("bash: warning: here-document at line 31 delimited by end-of-file (wanted`%s')\n", delimeter->delimeter);
// 				free_linked_list(token);
// 				free_linked_list_delimeter(&delimeter);
// 				exit(0);
// 			}
// 			if (ft_strncmp(heredoc_input, delimeter->delimeter, ft_strlen(delimeter->delimeter)) == 0
// 				&& ft_strlen(heredoc_input) == ft_strlen(delimeter->delimeter) && ft_lstsize_delimeter(delimeter) == 1)
// 			{
// 				free(heredoc_input);
// 				free_linked_list(token);
// 				free_linked_list_delimeter(&delimeter);
// 				exit(0);
// 			}
// 			if (ft_strncmp(heredoc_input, delimeter->delimeter, ft_strlen(delimeter->delimeter)) == 0
// 				&& ft_strlen(heredoc_input) == ft_strlen(delimeter->delimeter) && ft_lstsize_delimeter(delimeter) != 1)
// 			{
// 				ft_del_first_node_delimeter(&delimeter);
// 			}
// 			else if (ft_lstsize_delimeter(delimeter) == 1)
// 			{
// 				ft_putstr_fd(heredoc_input, fd);
// 				ft_putstr_fd("\n", fd);
// 			}
// 			free(heredoc_input);
// 		}
// 	}
// 	waitpid(pid, &status, 0);
// 	if (WIFEXITED(status))
// 	{
// 		if (WEXITSTATUS(status) == 130)
// 		{
// 			write(1, "\n", 1);
// 			free_linked_list(token);
// 		}
// 	}
// 	g_global = WEXITSTATUS(status);
// 	free_linked_list_delimeter(&delimeter);
// 	close(fd);
// }


int	ft_redirections_main(t_token **token, t_main *main)
{
	t_token	*temp;

	temp = *token;
	while (temp)
	{
		if (temp->type == TOKEN_REDIRECTION_OUT)
		{
			if (ft_redirection_out(token, main, temp) == 1)
				return (1);
			temp = *token;
		}
		else if (temp->type == TOKEN_REDIRECTION_IN)
		{
			if (ft_redirection_in(token, main, temp) == 1)
				return (1);
			temp = *token;
		}
		else if (temp->type == TOKEN_APPEND_OUT)
		{
			if (ft_append_out(token, main, temp) == 1)
				return (1);
			temp = *token;
		}
		else if (temp->type == TOKEN_HEREDOC)
		{
			ft_heredoc(token, main, temp);
			temp = *token;
		}
		if (temp)
			temp = temp->next;
	}
	return (0);
}