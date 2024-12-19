/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:43:05 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/19 10:21:21 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdbool.h>

int	ft_check_for_pipes(t_token **token)
{
	t_token	*temp;

	temp = *token;
	while (temp)
	{
		if (temp->type == TOKEN_PIPE)
			return (true);
		temp = temp->next;
	}
	return (false);
}

int	ft_count_pipes(t_token *token)
{
	int		count;

	count = 0;
	while (token)
	{
		if (token->type == TOKEN_PIPE)
			count++;
		token = token->next;
	}
	return (count);
}

void	ft_pipe_handler(t_token **list, t_main *main, int **fds, int pos_fd)
{
	bool	first;
	bool	last;
	t_token	*temp;
	int		pos;

	first = false;
	last = false;
	temp = *list;
	main->pos_fd = pos_fd;
	while (temp)
	{
		if (temp->type == TOKEN_PIPE && temp->next == NULL)
		{
			ft_del_node(list, temp);
			first = true;
		}
		if (temp->type == TOKEN_PIPE && temp->next != NULL)
		{
			ft_del_node(list, temp);
			last = true;
		}
		temp = temp->next;
	}
	if (first && !last)
	{
		pos = 1;
		ft_execve_main_pipes(list, main, fds, pos);
	}
	else if (first && last)
	{
		pos = 2;
		ft_execve_main_pipes(list, main, fds, pos);

	}
	else if (!first && last)
	{
		pos = 3;
		ft_execve_main_pipes(list, main, fds, pos);
	}
}

int	ft_pipes_main(t_token **token, t_main *main)
{
	t_token	*list;
	bool	done;
	int		**fds;
	int		pipe_count;
	int		i;

	i = -1;
	done = false;

	(void) main;
	if (ft_check_for_pipes(token) == false)
		return (false);

//* ---------------------------------- pipes --------------------------------- */
	pipe_count = ft_count_pipes(*token) + 1;
	fds = (int**)malloc(sizeof(int *) * (pipe_count));
	while (++i < pipe_count)
		fds[i] = (int*)malloc(sizeof(int) * 2);
	i = -1;
	while (++i < pipe_count)
		pipe(fds[i]);
	i = -1;
/* -------------------------------------------------------------------------- */

	list = malloc(sizeof(t_token));
	list = ft_lstnew_for_pipes(*token);
	while (done == false)
	{
		++i;
		while (*token)
		{
			if ((*token)->type == TOKEN_PIPE && ft_lstsize(list) != 1)
			{
				// ft_del_first_node(token);
				break ;
			}
			ft_del_first_node(token);
			if (!*token)
				break ;
			ft_lstadd_back(&list, ft_lstnew_for_pipes(*token));
		}
		ft_pipe_handler(&list, main, fds, i);
		free_linked_list(list);
		if (!*token)
			break ;
		list = malloc(sizeof(t_token));
		list = ft_lstnew_for_pipes(*token);
		if (!*token)
			done = true;
	}
	dup2(main->orig_fd[0], STDIN_FILENO);
	dup2(main->orig_fd[1], STDOUT_FILENO);

/* -------------------------------------------------------------------------- */
	// while (list)
	// {
	// 	printf("this is the content: %s\n", list->content);
	// 	printf("this is the type: %d\n", list->type);
	// 	list = list->next;
	// }

	// printf("\n\n\n");

	// while (*token)
	// {
	// 	printf("this is the content: %s\n", (*token)->content);
	// 	printf("this is the type: %d\n", (*token)->type);
	// 	*token = (*token)->next;
	// }

	// exit(0);
/* -------------------------------------------------------------------------- */
	return (true);
}