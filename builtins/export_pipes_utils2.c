/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_pipes_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:02:16 by naal-jen          #+#    #+#             */
/*   Updated: 2025/02/06 18:02:22 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_export_pipes_variable_handling(t_token **token, t_main *main)
{
	char	**splitted_argument;

	splitted_argument = ft_split((*token)->content, '=');
	if (splitted_argument[1] == NULL)
		ft_export_pipes_norm1(token, main, splitted_argument);
	else
		ft_export_pipes_norm2(main, splitted_argument);
	if (splitted_argument)
		free_mtx(&splitted_argument);
}

void	ft_g_global_final_assigning(int flag)
{
	if (flag == TRUE)
		g_global = 1;
	else
		g_global = 0;
}
