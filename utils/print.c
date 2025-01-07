/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakoriko <nakoriko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 16:38:31 by nakoriko          #+#    #+#             */
/*   Updated: 2025/01/07 16:19:20 by nakoriko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_error(const char *str, const char *arg, char *other)
{
	if (arg && !other)
	{
		write(2, str, ft_strlen(str));
		write(2, arg, ft_strlen(arg));
		write(2, "'\n", 2);
	}
	else if (arg && other)
	{
		write(2, str, ft_strlen(str));
		write(2, arg, ft_strlen(arg));
		write(2, other, ft_strlen(other));
		write(2, "\n", 1);
	}
	else
	{
		write(2, str, ft_strlen(str));
		write(2, "\n", 1);
	}
}

void	print_mtx(char **mtx, char *name)
{
	int	i;

	i = 0;
	while (mtx[i])
	{
		printf("%s %d: %s\n", name, i, mtx[i]);
		i++;
	}
}

// void	print_token_info(t_token *token)
// {
// 	char	*str;

// 	if (token->type == TOKEN_COMMAND)
// 		str = "TOKEN_COMMAND";
// 	else if (token->type == TOKEN_ARGUMENT)
// 		str = "TOKEN_ARGUMENT";
// 	else if (token->type == TOKEN_OPTION)
// 		str = "TOKEN_OPTION";
// 	else if (token->type == TOKEN_PIPE)
// 		str = "TOKEN_PIPE";
// 	else if (token->type == TOKEN_REDIRECTION_IN)
// 		str = "TOKEN_REDIRECTION_IN";
// 	else if (token->type == TOKEN_REDIRECTION_OUT)
// 		str = "TOKEN_REDIRECTION_OUT";
// 	else if (token->type == TOKEN_APPEND_OUT)
// 		str = "TOKEN_APPEND_OUT";
// 	else if (token->type == TOKEN_HEREDOC)
// 		str = "TOKEN_HEREDOC";
// 	else if (token->type == TOKEN_FILENAME)
// 		str = "TOKEN_FILENAME";
// 	else if (token->type == TOKEN_DELIMITER)
// 		str = "TOKEN_DELIMITER";
// 	else if (token->type == TOKEN_BUILTIN)
// 		str = "TOKEN_BUILTIN";
// 	else if (token->type == TOKEN_UNKNOWN)
// 		str = "TOKEN_UNKNOWN";
// 	printf("%s\n", str);
// }

// void	print_token_list(t_token *token)
// {
// 	int	i;

// 	i = 0;
// 	while (token != NULL)
// 	{
// 		printf ("Type %d is ", i);
// 		print_token_info(token);
// 		token = token->next;
// 		i++;
// 	}
// }
