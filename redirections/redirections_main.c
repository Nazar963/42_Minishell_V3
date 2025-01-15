/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 21:26:55 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/15 20:35:50 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_redirection_out(t_token **token, t_token *temp)
{
	int	fd;

	ft_del_node(token, temp);
	temp = *token;
	while (temp)
	{
		if (temp->type == TOKEN_FILENAME)
		{
			if (ft_access_check(temp, token) == 1)
				return (1);
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
			return (close(fd), 0);
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

int	ft_append_out(t_token **token, t_token *temp)
{
	int	fd;

	ft_del_node(token, temp);
	temp = *token;
	while (temp)
	{
		if (temp->type == TOKEN_FILENAME)
		{
			if (ft_access_check(temp, token) == 1)
				return (1);
			fd = open(temp->content, O_WRONLY | O_CREAT | O_APPEND, 0666);
			if (fd < 0)
			{
				ft_free_linked_list_until_pipe(token);
				perror("open");
				g_global = 1;
				return (1);
			}
			ft_del_node(token, temp);
			dup2(fd, STDOUT_FILENO);
			return (close(fd), 0);
		}
		temp = temp->next;
	}
	return (0);
}

int	ft_heredoc(t_token **token, t_main *main, t_token *temp)
{
	t_delimeter	*delimeter;

	ft_h_ini(token, &temp, &delimeter, main);
	main->exit = g_global;
	g_global = 666;
	if (ft_r(&delimeter, token, main, temp) == 1)
		return (1);
	g_global = 0;
	return (0);
}

int	ft_redirections_main(t_token **token, t_main *main)
{
	t_token	*temp;

	temp = *token;
	while (temp)
	{
		if (ft_redirections_main_norm14(token, main, &temp) == 1)
			return (1);
		if (temp)
			temp = temp->next;
	}
	temp = *token;
	while (temp)
	{
		if (ft_redirections_main_norm14(token, main, &temp) == 1)
			return (1);
		if (ft_redirections_main_norm0(token, main, &temp) == 1)
			return (1);
		if (temp)
			temp = temp->next;
	}
	return (0);
}
