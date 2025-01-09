/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token_type.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:56:12 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/09 14:14:35 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// t_TokenType	ft_token_type(char *token)
// {
// 	static	int	i;

// 	if (ft_strncmp(token, ">", 2) == 0)
// 	{
// 		i = TOKEN_REDIRECTION_OUT;
// 		return (TOKEN_REDIRECTION_OUT);
// 	}
// 	else if (ft_strncmp(token, "-", 1) == 0)
// 		return (TOKEN_OPTION);
// 	else if (ft_strncmp(token, ">>", 2) == 0)
// 	{
// 		i = TOKEN_APPEND_OUT;
// 		return (TOKEN_APPEND_OUT);
// 	}
// 	else if (ft_strncmp(token, "<", 2) == 0)
// 	{
// 		i = TOKEN_REDIRECTION_IN;
// 		return (TOKEN_REDIRECTION_IN);
// 	}
// 		return (TOKEN_PIPE);
// 	else if (ft_strncmp(token, "<<", 2) == 0)
// 	{
// 		i = TOKEN_HEREDOC;
// 		return (TOKEN_HEREDOC);
// 	}
// 	// else if (ft_strncmp(token, " ", 2) == 0) //! This is not a token type
// 	// 	return (TOKEN_DELIMITER);
// 	{
// 		i = 0;
// 		return (TOKEN_FILENAME);
// 	}
// 	else if (i == TOKEN_HEREDOC)
// 	{
// 		i = 0;
// 		return (TOKEN_DELIMITER);
// 	}
// 	else
// 		return (TOKEN_ARGUMENT);
// }
