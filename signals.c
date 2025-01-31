/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:51:41 by nakoriko          #+#    #+#             */
/*   Updated: 2025/01/31 20:52:21 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/ioctl.h>

void	ft_signals(int sig)
{
	if (sig == SIGINT)
	{
		if (g_global != 666)
		{
			if (g_global != 667)
				write(1, "\n", 1);
			if (g_global != 7)
				rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
		else if (g_global == 666)
		{
			write(1, "\n", 1);
			close(STDIN_FILENO);
			g_global = 130;
			rl_done = 1;
		}
		(void)sig;
	}
}

void	set_signals(void)
{
	signal(SIGINT, ft_signals);
	signal(SIGQUIT, SIG_IGN);
}
