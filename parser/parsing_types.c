/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_types.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 16:44:11 by nakoriko          #+#    #+#             */
/*   Updated: 2024/12/14 20:15:28 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_is_pipe(char *token)
{
	if (ft_strncmp(token, "|", 1) == 0)
		return (1);
	return (0);
}

int ft_is_redirection(char *token)
{
	if ((ft_strncmp(token, ">", 1) == 0 || ft_strncmp(token, "<", 1) == 0
		|| ft_strncmp(token, ">>", 2) == 0) || ft_strncmp(token, "<<", 2) == 0)
		return (1);
	return (0);
}

int ft_is_operator(char *token)
{
	if ((ft_strncmp(token, "|", 1) == 0) || (ft_strncmp(token, ">", 1) == 0) 
		|| (ft_strncmp(token, ">", 1) == 0) || (ft_strncmp(token, ">>", 2) == 0)
		|| (ft_strncmp(token, "<<", 2) == 0))
		return (1);
	return(0);
}

int ft_is_special_char(char *token)
{
	if ((ft_strncmp(token, ";", 1) == 0) || (ft_strncmp(token, "\\", 1) == 0))
		return(1);
	return(0);
}