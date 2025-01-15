/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_main_utils1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 20:02:48 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/15 20:13:23 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_g_global_status_norm0(int last, int counter, int status, int i)
{
	g_global = WEXITSTATUS(status);
	if (g_global == 0 && last == 130)
	{
		if (i == counter + 1)
			g_global = 130;
	}
	if (g_global == 7)
		g_global = 0;
}

void	ft_g_global_status_norm1(int *last, int *counter, int status, int i)
{
	g_global = 128 + WTERMSIG(status);
	if (g_global == 130)
	{
		*last = g_global;
		*counter = i;
	}
}
