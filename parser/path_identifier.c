/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_identifier.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:21:45 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/15 19:51:31 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_handle_exit_status_command_main(t_token **token)
{
	if (ft_strncmp((*token)->content, "$?", 2) == 0
		&& ft_strlen((*token)->content) == 2
		&& ft_lstsize(*token) == 1)
	{
		if (g_global == 667)
			printf("%d: command not found\n", 0);
		else
			printf("%d: command not found\n", g_global);
		ft_del_first_node(token);
	}
}

void	ft_path_identifier(t_token **token, t_main *main)
{
	if (ft_pipes_main(token, main) == TRUE)
		return ;
	if (*token)
		ft_handle_exit_status_command_main(token);
	if (*token)
		ft_redirections_main(token, main);
	if (*token)
		ft_builtins_main(token, main);
	if (*token)
		ft_execve_main(token, main);
	dup2(main->orig_fd[0], STDIN_FILENO);
	dup2(main->orig_fd[1], STDOUT_FILENO);
}
