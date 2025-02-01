/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_main_pipes.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 21:26:55 by naal-jen          #+#    #+#             */
/*   Updated: 2025/02/01 11:18:35 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_r(t_delimeter **d, t_token **token, t_main *main, t_token *temp)
{
	char	*h_in;
	char	*new_input;

	while (1)
	{
		h_in = readline("> ");
		if (g_global == 130)
			return (free(h_in), ft_r_norm0(d, token, main, &temp));
		if (!h_in)
			return (ft_r_norm1(*d), free_linked_list_delimeter(d), 0);
		if (ft_strncmp(h_in, (*d)->del, ft_strlen((*d)->del)) == 0
			&& ft_strlen(h_in) == ft_strlen((*d)->del) && ft_lsts_d(*d) == 1)
			return (free(h_in), free_linked_list_delimeter(d), 0);
		if (ft_strncmp(h_in, (*d)->del, ft_strlen((*d)->del)) == 0
			&& ft_strlen(h_in) == ft_strlen((*d)->del) && ft_lsts_d(*d) != 1)
			ft_del_first_node_delimeter(d);
		else if (ft_lsts_d(*d) == 1)
			ft_r_norm2(*d, &new_input, main, h_in);
		free(h_in);
	}
	return (0);
}

void	ft_heredoc_pipes_norm2(t_token **token, t_token **temp)
{
	ft_del_node(token, *temp);
	*temp = *token;
	while ((*token) && (*token)->next
		&& (*token)->next->type != TOKEN_DELIMITER)
		*token = (*token)->next;
}

int	ft_heredoc_pipes(t_token **token, t_main *main, t_token *temp)
{
	int			i;
	t_delimeter	*delimeter;

	i = 1;
	ft_heredoc_pipes_norm2(token, &temp);
	while (*token)
	{
		delimeter = ft_h_p_norm0(i, token, &temp, main);
		ft_h_p_norm1(token, &temp, &delimeter);
		if (ft_r(&delimeter, token, main, temp) == 1)
			return (1);
		close(main->fd);
		i++;
		while ((*token))
		{
			if ((*token)->next)
				if ((*token)->next->type == TOKEN_HEREDOC)
					break ;
			*token = (*token)->next;
		}
		*token = temp;
		return (close(main->fd), dup2(main->orig_fd[0], STDIN_FILENO), 0);
	}
	return (0);
}

int	ft_herdoc_pipes_main(t_token **token, t_main *main)
{
	t_token	*temp;

	temp = *token;
	while (temp)
	{
		if (temp->type == TOKEN_HEREDOC)
		{
			if (ft_heredoc_pipes(token, main, temp) == 1)
				return (1);
			temp = *token;
		}
		if (temp)
			temp = temp->next;
	}
	return (0);
}
