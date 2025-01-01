/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_main_pipes.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 21:26:55 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/30 15:14:45 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_heredoc_pipes(t_token **token, t_token *temp)
{
	char	*heredoc_input;
	int		fd;

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
					temp = *token;
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

int	ft_herdoc_pipes_main(t_token **token)
{
	t_token	*temp;

	temp = *token;
	while (temp)
	{
		if (temp->type == TOKEN_HEREDOC)
		{
			ft_heredoc_pipes(token, temp);
			temp = *token;
		}
		if (temp)
			temp = temp->next;
	}
	return (0);
}