/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 22:40:05 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/15 19:33:29 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_builtins_main(t_token **token, t_main *main)
{
	size_t	len;
	char	*s;

	len = ft_strlen((*token)->content);
	s = (*token)->content;
	if (len == ft_strlen("echo") && ft_strncmp(s, "echo", len) == 0)
		return (ft_echo(token), 0);
	if (len == ft_strlen("pwd") && ft_strncmp(s, "pwd", len) == 0)
		return (ft_pwd(token), 0);
	if (len == ft_strlen("cd") && ft_strncmp(s, "cd", len) == 0)
		return (ft_cd(token, main), 0);
	if (len == ft_strlen("export") && ft_strncmp(s, "export", len) == 0)
		return (ft_export(token, main), 0);
	if (len == ft_strlen("unset") && ft_strncmp(s, "unset", len) == 0)
		return (ft_unset(token, main), 0);
	if (len == ft_strlen("env") && ft_strncmp(s, "env", len) == 0)
		return (ft_env(token, main), 0);
	if (len == ft_strlen("exit") && ft_strncmp(s, "exit", len) == 0)
		return (ft_exit(token, main), 0);
	return (0);
}
