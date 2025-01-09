/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_main_pipes_utils0.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 17:11:16 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/08 23:29:27 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_delimeter	*ft_h_p_norm0(int i, t_token **token, t_token **temp, t_main *main)
{
	char		*file;
	char		*itoa_str;
	char		*temp_file;
	t_delimeter	*delimeter;

	file = ft_strdup("/tmp/heredoc");
	itoa_str = ft_itoa(i);
	temp_file = ft_strjoin(file, itoa_str);
	free(file);
	free(itoa_str);
	file = ft_strjoin(temp_file, ".txt");
	free(temp_file);
	main->fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	(*token)->heredoc_file = ft_strdup(file);
	free(file);
	*token = (*token)->next;
	if ((*token)->type == TOKEN_HEREDOC)
		ft_del_first_node_and_restructure_temp(token, *temp);
	delimeter = ft_lstnew_d((*token)->content);
	delimeter->expaned = (*token)->expaned_del;
	ft_del_first_node_and_restructure_temp(token, *temp);
	return (delimeter);
}

void	ft_h_p_norm1(t_token **token, t_token **temp, t_delimeter **delimeter)
{
	t_delimeter	*new_del;

	while (*token && (*token)->type != TOKEN_PIPE)
	{
		if ((*token)->type == TOKEN_DELIMITER)
		{
			new_del = ft_lstnew_d((*token)->content);
			new_del->expaned = (*token)->expaned_del;
			ft_lstadd_back_delimeter(delimeter, new_del);
			ft_del_first_node_and_restructure_temp(token, *temp);
		}
		else if ((*token)->type == TOKEN_HEREDOC)
		{
			ft_del_first_node_and_restructure_temp(token, *temp);
		}
		else if (*token)
			*token = (*token)->next;
	}
	g_global = 666;
}

int	ft_r_norm0(t_delimeter **d, t_token **token, t_main *main, t_token **temp)
{
	free_linked_list_delimeter(d);
	free_linked_list(temp);
	free_linked_list(token);
	*token = NULL;
	close(main->fd);
	dup2(main->orig_fd[0], STDIN_FILENO);
	g_global = main->exit;
	return (1);
}

void	ft_r_norm1(t_delimeter *delimeter)
{
	print_error("bash: warning: here-document ", NULL, NULL);
	print_error("at line 31 delimited by end-", NULL, NULL);
	print_error("of-file (wanted`%s')\n", delimeter->del, NULL);
}

void	ft_r_norm2(t_delimeter *d, char **new_input, t_main *main, char *h_in)
{
	if (d->expaned)
	{
		*new_input = ft_expaned_var(h_in, main);
		ft_putstr_fd(*new_input, main->fd);
		ft_putstr_fd("\n", main->fd);
		free (*new_input);
	}
	else
	{
		ft_putstr_fd(h_in, main->fd);
		ft_putstr_fd("\n", main->fd);
	}
}
