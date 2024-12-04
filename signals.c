/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:51:41 by nakoriko          #+#    #+#             */
/*   Updated: 2024/12/04 12:45:33 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//TODO: da gestire il exit_status che sarebbe un variabile globale e dovrebbe essere settata a 1
//TODO: da gestire il caso in qui il rl_redisplay() dovrebbe essere eseguita solo se il prompt è vuoto
void ft_signals(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n",  1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		(void)sig;
	}
}

void set_signals()
{
	signal(SIGINT, ft_signals);
	signal(SIGQUIT, SIG_IGN);
}