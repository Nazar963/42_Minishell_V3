/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_main_utils1.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 21:18:20 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/15 20:36:11 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_redirections_main_norm0(t_token **token, t_main *main, t_token **temp)
{
	if ((*temp)->type == TOKEN_REDIRECTION_OUT)
	{
		if (ft_redirection_out(token, (*temp)) == 1)
			return (1);
		(*temp) = *token;
	}
	else if ((*temp)->type == TOKEN_REDIRECTION_IN)
	{
		if (ft_redirection_in(token, main, (*temp)) == 1)
			return (1);
		(*temp) = *token;
	}
	else if ((*temp)->type == TOKEN_APPEND_OUT)
	{
		if (ft_append_out(token, (*temp)) == 1)
			return (1);
		(*temp) = *token;
	}
	else if ((*temp)->type == TOKEN_HEREDOC)
	{
		if (ft_heredoc(token, main, (*temp)) == 1)
			return (1);
		(*temp) = *token;
	}
	return (0);
}

char	*ft_expaned_var(char *str, t_main *main)
{
	t_ex	ex;

	ft_expaned_var_ini(&ex, str);
	while (str[ex.i])
	{
		if (str[ex.i] == '$' && str[ex.i + 1] == '?')
		{
			ex.temp = ft_itoa(main->exit);
			ex.joined_str = ft_strjoin(ex.new_str, ex.temp);
			free(ex.temp);
			free(ex.new_str);
			ex.new_str = ex.joined_str;
			ex.i += 2;
		}
		else if (str[ex.i] == '$')
			ft_expaned_var_norm1(&ex, str, main);
		else
			ft_expaned_var_norm0(&ex, str);
	}
	free(ex.var);
	return (ex.new_str);
}

void	ft_h_ini_norm0(t_token **token, t_token **temp, t_delimeter **d)
{
	while ((*temp))
	{
		if ((*temp)->type == TOKEN_DELIMITER || (*temp)->type == TOKEN_HEREDOC)
		{
			if ((*temp)->type == TOKEN_DELIMITER)
			{
				ft_lstadd_back_delimeter(d, ft_lstnew_d((*temp)->content));
				(*d)->expaned = (*temp)->expaned_del;
			}
			ft_del_node(token, (*temp));
			(*temp) = *token;
		}
		if ((*temp))
			(*temp) = (*temp)->next;
	}
}

void	ft_h_ini(t_token **token, t_token **temp, t_delimeter **d, t_main *main)
{
	ft_del_node(token, (*temp));
	(*temp) = *token;
	if ((*temp)->type != 9 && ((*temp)->type == 0 || (*temp)->type == 10))
	{
		main->fd = open("/tmp/heredoc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
		(*token)->heredoc_file = ft_strdup("/tmp/heredoc.txt");
		(*temp) = (*temp)->next;
	}
	else
	{
		while ((*token) && (*token)->type != 0 && (*token)->type != 10)
			(*token) = (*token)->next;
		main->fd = open("/tmp/heredoc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (*token)
			(*token)->heredoc_file = ft_strdup("/tmp/heredoc.txt");
		*token = *temp;
	}
	while ((*temp)->type != TOKEN_DELIMITER)
		(*temp) = (*temp)->next;
	(*d) = ft_lstnew_d((*temp)->content);
	(*d)->expaned = (*temp)->expaned_del;
	ft_del_node(token, (*temp));
	(*temp) = *token;
	ft_h_ini_norm0(token, temp, d);
}

void	ft_h_p_norm01(t_token **token, t_token *temp_token, char *file)
{
	if (temp_token->type != 9
		&& (temp_token->type == 0 || temp_token->type == 10))
		(*token)->heredoc_file = ft_strdup(file);
	else
	{
		while ((*token) && (*token)->type != TOKEN_COMMAND
			&& (*token)->type != TOKEN_BUILTIN)
			(*token) = (*token)->next;
		if (*token)
			(*token)->heredoc_file = ft_strdup(file);
		*token = temp_token;
	}
}
