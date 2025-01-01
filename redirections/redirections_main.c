/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 21:26:55 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/01 17:08:37 by naal-jen         ###   ########.fr       */
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

// void	ft_heredoc(t_token **token, t_main *main, t_token *temp)
// {
// 	char	*heredoc_input;
// 	int		fd;

// 	(void) main;
// 	ft_del_node(token, temp);
// 	temp = *token;
// 	fd = open("/tmp/heredoc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
// 	while (temp)
// 	{
// 		if (temp->type == TOKEN_DELIMITER)
// 		{
// 			set_signals_heredoc(main);
// 			while (1)
// 			{
// 				heredoc_input = readline("> ");
// 				if (!heredoc_input || g_global == 666)
// 				{
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
// 	close(fd);

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


void	ft_heredoc(t_token **token, t_main *main, t_token *temp)
{
	int			fd;
	int			pid;
	int			status;
	char		*heredoc_input;
	t_delimeter	*delimeter;

	(void) main;
	ft_del_node(token, temp);
	temp = *token;
	fd = open("/tmp/heredoc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	temp->heredoc_file = ft_strdup("/tmp/heredoc.txt");
	temp = temp->next;
	delimeter = ft_lstnew_delimeter(temp->content);
	ft_del_node(token, temp);
	temp = *token;
	while (temp)
	{
		if (temp->type == TOKEN_DELIMITER)
		{
			ft_lstadd_back_delimeter(&delimeter, ft_lstnew_delimeter(temp->content));
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
	pid = fork();
	if (pid == 0)
	{
		set_signals_heredoc(main, token, delimeter);
		while (1)
		{
			heredoc_input = readline("> ");
			if (g_global == 130)
			{
				free_all(main, token);
				// free_linked_list(token);
				free_linked_list_delimeter(&delimeter);
				exit(130);
			}
			if (!heredoc_input)
			{
				printf("bash: warning: here-document at line 31 delimited by end-of-file (wanted`%s')\n", delimeter->delimeter);
				free_linked_list(token);
				free_linked_list_delimeter(&delimeter);
				exit(0);
			}
			if (ft_strncmp(heredoc_input, delimeter->delimeter, ft_strlen(delimeter->delimeter)) == 0
				&& ft_strlen(heredoc_input) == ft_strlen(delimeter->delimeter) && ft_lstsize_delimeter(delimeter) == 1)
			{
				free(heredoc_input);
				free_linked_list(token);
				free_linked_list_delimeter(&delimeter);
				exit(0);
			}
			if (ft_strncmp(heredoc_input, delimeter->delimeter, ft_strlen(delimeter->delimeter)) == 0
				&& ft_strlen(heredoc_input) == ft_strlen(delimeter->delimeter) && ft_lstsize_delimeter(delimeter) != 1)
			{
				ft_del_first_node_delimeter(&delimeter);
			}
			else if (ft_lstsize_delimeter(delimeter) == 1)
			{
				ft_putstr_fd(heredoc_input, fd);
				ft_putstr_fd("\n", fd);
			}
			free(heredoc_input);
		}
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 130)
		{
			write(1, "\n", 1);
			free_linked_list(token);
		}
	}
	g_global = WEXITSTATUS(status);
	close(fd);
}


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