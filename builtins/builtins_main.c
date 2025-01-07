/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakoriko <nakoriko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 22:40:05 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/05 18:37:13 by nakoriko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_builtins_main(t_token **token, t_main *main)
{
	int	len;

	len = ft_strlen((*token)->content);
	// if (len == 0)
	// 	return (0);
	if (ft_strncmp((*token)->content, "echo", len) == 0)
		ft_echo(token);
	else if (ft_strncmp((*token)->content, "pwd", len) == 0)
		ft_pwd(token);
	else if (ft_strncmp((*token)->content, "cd", len) == 0)
		ft_cd(token, main);
	else if (ft_strncmp((*token)->content, "export", len) == 0)
		ft_export(token, main);
	else if (ft_strncmp((*token)->content, "unset", len) == 0)
		ft_unset(token, main);
	else if (ft_strncmp((*token)->content, "env", len) == 0)
		ft_env(token, main);
	else if (ft_strncmp((*token)->content, "exit", len) == 0)
		ft_exit(token, main);
	return (0);
}
