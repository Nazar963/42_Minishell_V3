/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_types.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakoriko <nakoriko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 16:44:11 by nakoriko          #+#    #+#             */
/*   Updated: 2025/01/12 20:45:43 by nakoriko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_is_pipe(char *token)
{
	int	n;

	n = ft_strlen(token);
	if (ft_strncmp(token, "|", n) == 0)
		return (1);
	return (0);
}

int	ft_is_redirection(char *token)
{
	int	n;

	n = ft_strlen(token);
	if ((ft_strncmp(token, ">", n) == 0
			|| ft_strncmp(token, "<", n) == 0
			|| ft_strncmp(token, ">>", n) == 0)
		|| ft_strncmp(token, "<<", n) == 0)
		return (1);
	return (0);
}

int	ft_is_operator(char *token)
{
	int	n;

	n = ft_strlen(token);
	if ((ft_strncmp(token, "|", n) == 0)
		|| (ft_strncmp(token, ">", n) == 0)
		|| (ft_strncmp(token, ">", n) == 0)
		|| (ft_strncmp(token, ">>", n) == 0)
		|| (ft_strncmp(token, "<<", n) == 0))
		return (1);
	return (0);
}

int	ft_is_tab(char c, char next)
{
	if ((c == '\\' && next == 'n') || (c == '\\' && next == 'v')
		|| (c == '\\' && next == 'b') || (c == '\\' && next == 'r')
		|| (c == '\\' && next == 't') || (c == '\\' && next == 'f'))
		return (1);
	return (0);
}

int	ft_is_space(char c)
{
	if (c == ' ' || c == '\n' || c == '\t')
		return (1);
	return (0);
}
