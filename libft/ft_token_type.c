/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token_type.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:56:12 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/06 15:56:48 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

TokenType	ft_token_type(char *token)
{
	if (ft_strncmp(token, ">", 2) == 0)
		return (TOKEN_REDIRECTION_OUT);
	else if (ft_strncmp(token, "-", 1) == 0)
		return (TOKEN_OPTION);
	else if (ft_strncmp(token, ">>", 2) == 0)
		return (TOKEN_APPEND_OUT);
	else if (ft_strncmp(token, "<", 2) == 0)
		return (TOKEN_REDIRECTION_IN);
	else if (ft_strncmp(token, "|", 2) == 0)
		return (TOKEN_PIPE);
	else if (ft_strncmp(token, ">>", 2) == 0)
		return (TOKEN_APPEND_OUT);
	else if (ft_strncmp(token, "<<", 2) == 0)
		return (TOKEN_HEREDOC);
	else if (ft_strncmp(token, " ", 2) == 0)
		return (TOKEN_DELIMITER);
	else
		return (TOKEN_ARGUMENT);
}