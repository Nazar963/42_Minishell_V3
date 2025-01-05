/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_main_pipes.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 21:26:55 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/03 17:19:48 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_heredoc_pipes(t_token **token, t_main *main, t_token *temp)
{
	char		*heredoc_input;
	int			fd;
	char		*file;
	int			i;
	t_delimeter	*delimeter;
	char		*new_input;

	i = 1;
	ft_del_node(token, temp);
	temp = *token;
	while ((*token)->next->type != TOKEN_DELIMITER)
		*token = (*token)->next;
	while (*token)
	{
		// file = ft_strdup("/tmp/heredoc"); //! MEMORY LEAK FOR SURE
		// file = ft_strjoin(file, ft_itoa(i));
		// file = ft_strjoin(file, ".txt");
		// fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		// (*token)->heredoc_file = ft_strdup(file);
		// free(file);

		file = ft_strdup("/tmp/heredoc"); // MEMORY LEAK FOR SURE
		char *itoa_str = ft_itoa(i);
		char *temp_file = ft_strjoin(file, itoa_str);
		free(file);
		free(itoa_str);
		file = ft_strjoin(temp_file, ".txt");
		free(temp_file);
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		(*token)->heredoc_file = ft_strdup(file);
		free(file);

		*token = (*token)->next;
		if ((*token)->type == TOKEN_HEREDOC)
			ft_del_first_node_and_restructure_temp(token, temp);

		delimeter = ft_lstnew_delimeter((*token)->content);
		ft_del_first_node_and_restructure_temp(token, temp);
		while (*token && (*token)->type != TOKEN_PIPE)
		{
			if ((*token)->type == TOKEN_DELIMITER)
			{
				ft_lstadd_back_delimeter(&delimeter, ft_lstnew_delimeter((*token)->content));
				ft_del_first_node_and_restructure_temp(token, temp);
			}
			else if ((*token)->type == TOKEN_HEREDOC)
			{
				ft_del_first_node_and_restructure_temp(token, temp);
			}
			else if (*token)
				*token = (*token)->next;
		}

		g_global = 666;
		// set_signals_heredoc(main, token, delimeter);
		while (1)
		{
			heredoc_input = readline("> ");
			// if (g_global == 666)
			if (g_global == 130)
			{
				free(heredoc_input);
				free_linked_list_delimeter(&delimeter);
				close(fd);
				// free_linked_list(token);
				free_linked_list(&temp);
				*token = NULL;
				dup2(main->orig_fd[0], STDIN_FILENO);
				return (1);
			}
			if (!heredoc_input)
			{
				printf("bash: warning: here-document at line 31 delimited by end-of-file (wanted`%s')\n", delimeter->delimeter);
				// free_linked_list(token);
				free_linked_list(&temp);
				*token = NULL;
				// close(fd);
				free_linked_list_delimeter(&delimeter);
				close(fd);
				dup2(main->orig_fd[0], STDIN_FILENO);
				return (1);
			}
			if (ft_strncmp(heredoc_input, delimeter->delimeter, ft_strlen(delimeter->delimeter)) == 0
				&& ft_strlen(heredoc_input) == ft_strlen(delimeter->delimeter) && ft_lstsize_delimeter(delimeter) == 1)
			{
				free(heredoc_input);
				free_linked_list_delimeter(&delimeter);
				// if (flag == TRUE)
				// {
				// 	close(fd);
				// 	dup2(main->orig_fd[0], STDIN_FILENO);
				// 	return ;
				// }
				// else
				break ;
			}
			if (ft_strncmp(heredoc_input, delimeter->delimeter, ft_strlen(delimeter->delimeter)) == 0
				&& ft_strlen(heredoc_input) == ft_strlen(delimeter->delimeter) && ft_lstsize_delimeter(delimeter) != 1)
			{
				ft_del_first_node_delimeter(&delimeter);
			}
			else if (ft_lstsize_delimeter(delimeter) == 1)
			{
				// if (delimeter->expaned)
				// 	new_input = ft_expaned_var(heredoc_input, main);
				// else
				// 	new_input = heredoc_input;
				// ft_putstr_fd(new_input, fd);
				// ft_putstr_fd("\n", fd);
				// if (delimeter->expaned)
				// 	free(new_input);
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
		close(fd);
		i++;
		while ((*token) && (*token)->next->type != TOKEN_HEREDOC)
			*token = (*token)->next;
		if (!(*token))
		{
			close(fd);
			dup2(main->orig_fd[0], STDIN_FILENO);
			*token = temp;
			break ;
		}
	}
	return (0);
}

// void	ft_heredoc_pipes(t_token **token, t_token *temp)
// {
// 	char	*heredoc_input;
// 	int		fd;

// 	ft_del_node(token, temp);
// 	temp = *token;
// 	fd = open("utils/heredoc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
// 	while (temp)
// 	{
// 		if (temp->type == TOKEN_DELIMITER)
// 		{
// 			while (1)
// 			{
// 				heredoc_input = readline("> ");
// 				if (!heredoc_input)
// 				{
// 					ft_del_node(token, temp);
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

// 	fd = open("utils/heredoc.txt", O_RDONLY);
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