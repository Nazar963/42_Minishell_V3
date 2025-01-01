/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:51:41 by nakoriko          #+#    #+#             */
/*   Updated: 2025/01/01 18:25:01 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/ioctl.h>


extern int rl_done;

void	ft_signals(int sig)
{
	if (sig == SIGINT)
	{
		if (g_global != 666)
		{
			printf("hello \n ");
			write(1, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
		(void)sig;
	}
}

void	set_signals()
{
	signal(SIGINT, ft_signals);
	signal(SIGQUIT, SIG_IGN);
}


void	ft_signals_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		exit(130);
		g_global = 130;
		(void)sig;
	}
}

void	set_signals_heredoc(t_main *main, t_token **token, t_delimeter *delimeter)
{
	free_linked_list_delimeter(&delimeter);
	free_all(main, token);
	signal(SIGINT, ft_signals_heredoc);
	signal(SIGQUIT, SIG_IGN);
}