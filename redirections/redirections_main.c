/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 21:26:55 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/16 12:38:03 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_redirection_out(t_token **token, t_main *main, t_token *temp)
{
	int	fd;

	(void)main;
	ft_del_node(token, temp);
	temp = *token;
	while (temp)
	{
		if (temp->type == TOKEN_FILENAME)
		{
			fd = open(temp->content, O_WRONLY | O_CREAT, 0666);
			if (fd < 0) //TODO: Check how you want to create this error
			{
				perror("open");
				return ;
			}
			ft_del_node(token, temp);
			dup2(fd, STDOUT_FILENO);
			close(fd);
			return ;
		}
		temp = temp->next;
	}
}

void	ft_redirection_in(t_token **token, t_main *main, t_token *temp)
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
			if (fd < 0) //TODO: Check how you want to create this error
			{
				perror("open");
				return ;
			}
			ft_del_node(token, temp);
			dup2(fd, STDIN_FILENO);
			close(fd);
			return ;
		}
		temp = temp->next;
	}
}

void	ft_append_out(t_token **token, t_main *main, t_token *temp)
{
	int	fd;

	(void)main;
	ft_del_node(token, temp);
	temp = *token;
	while (temp)
	{
		if (temp->type == TOKEN_FILENAME)
		{
			fd = open(temp->content, O_WRONLY | O_CREAT | O_APPEND, 0666);
			if (fd < 0) //TODO: Check how you want to create this error
			{
				perror("open");
				return ;
			}
			ft_del_node(token, temp);
			dup2(fd, STDOUT_FILENO);
			close(fd);
			return ;
		}
		temp = temp->next;
	}
}

void	ft_heredoc(t_token **token, t_main *main, t_token *temp)
{
	char	*heredoc_input;
	int		fd;

	(void) main;
	ft_del_node(token, temp);
	temp = *token;
	fd = open("utils/heredoc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	while (temp)
	{
		if (temp->type == TOKEN_DELIMITER)
		{
			while (1)
			{
				heredoc_input = readline("> ");
				if (!heredoc_input)
				{
					ft_del_node(token, temp);
					break ;
				}
				if (ft_strncmp(heredoc_input, temp->content, ft_strlen(temp->content)) == 0
					&& ft_strlen(heredoc_input) == ft_strlen(temp->content))
				{
					free(heredoc_input);
					ft_del_node(token, temp);
					break ;
				}
				ft_putstr_fd(heredoc_input, fd);
				ft_putstr_fd("\n", fd);
				free(heredoc_input);
			}
		}
		temp = temp->next;
	}
	close(fd);

	fd = open("utils/heredoc.txt", O_RDONLY);
	if (fd < 0) {
		perror("open");
		return;
	}

	if (dup2(fd, STDIN_FILENO) < 0) {
		perror("dup2");
		close(fd);
		return;
	}

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
			// printf("ft_redirections_main\n");
			ft_redirection_out(token, main, temp);
			// temp = token;
			// while (temp)
			// {
			// 	printf("fftemp->content: %s\n", temp->content);
			// 	temp = temp->next;
			// }
			// exit(0);
		}
		else if (temp->type == TOKEN_REDIRECTION_IN)
			ft_redirection_in(token, main, temp);
		else if (temp->type == TOKEN_APPEND_OUT)
			ft_append_out(token, main, temp);
		else if (temp->type == TOKEN_HEREDOC)
			ft_heredoc(token, main, temp);
		temp = temp->next;
	}
	return (0);
}