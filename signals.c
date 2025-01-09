/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:51:41 by nakoriko          #+#    #+#             */
/*   Updated: 2025/01/09 13:59:40 by naal-jen         ###   ########.fr       */
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
			write(1, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
			g_global = 0;
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

void	set_signals()
{
	signal(SIGINT, ft_signals);
	signal(SIGQUIT, SIG_IGN);
}


// void	ft_signals_heredoc(int sig)
// {
// 	if (sig == SIGINT)
// 	{
// 		if (g_global == 666)
// 		{
// 			write(STDOUT_FILENO, "\n", 1);
// 			close(STDIN_FILENO);
// 			g_global = 130;
// 		}
// 		// if (!isatty(STDIN_FILENO))
// 		// 	printf("STDIN is not a TTY\n");
// 		// else
// 		// 	printf("STDIN is a TTY\n");
// 		// char c = '\n';
// 		// if (ioctl(STDIN_FILENO, TIOCSTI, &c) == -1)
// 		// 	perror("ioctl TIOCSTI failed");
// 		// printf("hello you there\n");

// 		// rl_on_new_line();
// 		// rl_replace_line("", 0);
// 		// rl_done = 1; // forces readline to return
// 		// close(STDOUT_FILENO);
// 		// close(STDERR_FILENO);
// 		// close(0);
// 		// close(1);
// 		// close(2);
// 		// close(3);
// 		// close(4);
// 		// close(5);
// 		// rl_on_new_line();
// 		// rl_replace_line("", 0);
// 		// rl_redisplay();

// 		// exit(130);
// 	}
// }

// void	set_signals_heredoc(t_main *main, t_token **token, t_delimeter *delimeter)
// {
// 	// free_linked_list_delimeter(&delimeter);
// 	// free_all(main, token);
// 	(void)main;
// 	(void)token;
// 	(void)delimeter;
// 	signal(SIGINT, ft_signals_heredoc);
// 	signal(SIGQUIT, SIG_IGN);
// }