/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 13:05:14 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/29 21:57:58 by naal-jen         ###   ########.fr       */
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

void	ft_exit(t_token **token, t_main *main)
{
	int	exit_num;

	ft_del_first_node(token);
	if (*token && (*token)->type == 1 && (*token)->next == NULL)
	{
		if (ft_control_int((*token)->content) == 0)
		{
			printf("exit\n");
			print_error("bash: exit: ", (*token)->content, ": numeric argument required");
			free_all(main, token);
			exit(2);
		}
		else
		{
			exit_num = ft_atoi((*token)->content);
			free_all(main, token);
			exit(exit_num);
		}
	}
	else if (*token)
	{
		if (ft_control_int((*token)->content) == 0)
		{
			print_error("exit\nbash: exit: ", (*token)->content, ": numeric argument required\n");
			free_all(main, token);
			exit(2);
		}
		else if (ft_atoi((*token)->content) < 0)
		{
			printf("exit\n");
			exit_num = ft_correct_exit_status_calc(ft_atoi((*token)->content));
			free_all(main, token);
			exit (exit_num);
		}
		print_error("bash: exit: too many arguments", NULL, NULL);
		free_all(main, token);
		exit(1);
	}
	free_all(main, token);
	exit(0);
}