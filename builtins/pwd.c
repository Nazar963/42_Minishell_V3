/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 15:28:47 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/29 15:29:03 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_pwd(t_token **token)
{
	char	*pwd;

	ft_del_first_node(token);
	pwd = getcwd(NULL, 0);
	printf("%s\n", pwd);
	free(pwd);
	while (*token && (*token)->type != TOKEN_PIPE)
		ft_del_first_node(token);
}
