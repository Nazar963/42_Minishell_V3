/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_main_pipes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:22:26 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/05 23:07:43 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_builtins_main_pipes(t_token **token, t_main *main, int **fds)
{
	int	len;

	len = ft_strlen((*token)->content);
	if (ft_strncmp((*token)->content, "echo", len) == 0)
		ft_echo_pipes(token);
	else if (ft_strncmp((*token)->content, "pwd", len) == 0)
		ft_pwd_pipes(token);
	else if (ft_strncmp((*token)->content, "cd", len) == 0)
		ft_cd_pipes(token, main);
	else if (ft_strncmp((*token)->content, "export", len) == 0)
		ft_export_pipes(token, main);
	else if (ft_strncmp((*token)->content, "unset", len) == 0)
		ft_unset(token, main);
	else if (ft_strncmp((*token)->content, "env", len) == 0)
		ft_env_pipes(token, main);
	else if (ft_strncmp((*token)->content, "exit", len) == 0)
		ft_exit_pipes(token, main, fds);
	return (0);
}
