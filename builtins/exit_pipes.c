/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 13:05:14 by naal-jen          #+#    #+#             */
/*   Updated: 2025/02/06 16:13:30 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_correct_exit_status_calc_pipes(int val)
{
	int	mod_value;

	mod_value = val % 256;
	if (mod_value < 0)
		mod_value += 256;
	return (val);
}

void	ft_free_exit_pipes(t_main *main, t_token **token, int **fds)
{
	free(main->pids);
	free_fds(fds, main->pipe_count);
	free_all(main, token);
}

void	ft_exit_pipes_norm0(t_main *main, t_token **token, int **fds)
{
	int	exit_num;

	if (ft_control_int((*token)->content) == 0 && (*token)->type != TOKEN_PIPE)
	{
		print_error("bash: exit: ", (*token)->content,
			": numeric argument required");
		ft_free_exit_pipes(main, token, fds);
		exit(2);
	}
	else if (ft_atoi((*token)->content) < 0 && (*token)->type != TOKEN_PIPE)
	{
		printf("exit\n");
		exit_num = ft_correct_exit_status_calc(ft_atoi((*token)->content));
		ft_free_exit_pipes(main, token, fds);
		exit (exit_num);
	}
	print_error("bash: exit: too many arguments", NULL, NULL);
	while ((*token) && (*token)->type != TOKEN_PIPE)
	{
		ft_del_first_node(token);
	}
	ft_free_exit_pipes(main, token, fds);
	exit(1);
}

void	ft_exit_pipes(t_token **token, t_main *main, int **fds)
{
	int	exit_num;

	ft_del_first_node(token);
	if (*token && (*token)->type == 1 && (*token)->next == NULL)
	{
		if (ft_control_int((*token)->content) == 0)
		{
			print_error("bash: exit: ", (*token)->content,
				": numeric argument required");
			ft_free_exit_pipes(main, token, fds);
			exit(2);
		}
		else
		{
			exit_num = ft_atoi((*token)->content);
			ft_free_exit_pipes(main, token, fds);
			exit(exit_num);
		}
	}
	else if (*token)
		ft_exit_pipes_norm0(main, token, fds);
	ft_free_exit_pipes(main, token, fds);
	exit(0);
}
