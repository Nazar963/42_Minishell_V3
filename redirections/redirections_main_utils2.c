/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_main_utils2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 20:36:41 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/15 20:36:49 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_redirections_main_norm14(t_token **token, t_main *main, t_token **temp)
{
	if ((*temp)->type == TOKEN_HEREDOC)
	{
		if (ft_heredoc(token, main, (*temp)) == 1)
			return (1);
		(*temp) = *token;
	}
	return (0);
}
