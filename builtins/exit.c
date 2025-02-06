/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 13:05:14 by naal-jen          #+#    #+#             */
/*   Updated: 2025/02/03 22:51:29 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_correct_exit_status_calc(int val)
{
	int	mod_value;

	mod_value = val % 256;
	if (mod_value < 0)
		mod_value += 256;
	return (val);
}

void	exit_close_fds(t_main *main, int exit_num)
{
	close(main->orig_fd[0]);
	close(main->orig_fd[1]);
	exit(exit_num);
}

void	ft_exit_with_multi_args(t_token **token, t_main *main)
{
	int	exit_num;

	if (ft_control_int((*token)->content) == 0)
	{
		print_error("bash: exit: ", (*token)->content,
			": numeric argument required");
		free_all(main, token);
		exit_close_fds(main, 2);
	}
	else if (ft_atoi((*token)->content) < 0)
	{
		printf("exit\n");
		exit_num = ft_correct_exit_status_calc(ft_atoi((*token)->content));
		free_all(main, token);
		exit_close_fds(main, exit_num);
	}
	printf("exit\nminishell: exit: too many arguments\n");
	if (g_global == 0)
		g_global = 1;
	free_linked_list(token);
}

void	ft_exit(t_token **token, t_main *main)
{
	int	exit_num;

	ft_del_first_node(token);
	if (*token && (*token)->type == 1 && (*token)->next == NULL)
	{
		if (ft_control_int((*token)->content) == 0)
		{
			printf("exit\n");
			print_error("bash: exit: ", (*token)->content,
				": numeric argument required");
			return (free_all(main, token), exit_close_fds(main, 2), (void)0);
		}
		else
		{
			exit_num = ft_atoi((*token)->content);
			free_all(main, token);
			return (printf("exit\n"), exit_close_fds(main, exit_num), (void)0);
		}
	}
	else if (*token)
		return (ft_exit_with_multi_args(token, main), (void)0);
	free_all(main, token);
	exit(0);
}
