/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 14:20:46 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/08 20:34:59 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_token	*ft_lstnew(char *content)
{
	t_token	*temp;

	temp = (t_token *)malloc(sizeof(*temp));
	if (!temp)
		return (NULL);
	temp->content = ft_strdup(content);
	temp->expaned_del = 0;
	temp->heredoc_file = NULL;
	temp->next = NULL;
	return (temp);
}

t_delimeter	*ft_lstnew_d(char *content)
{
	t_delimeter	*temp;

	temp = (t_delimeter *)malloc(sizeof(*temp));
	if (!temp)
		return (NULL);
	temp->del = ft_strdup(content);
	temp->next = NULL;
	return (temp);
}

t_token	*ft_lstnew_for_pipes(t_token *token)
{
	t_token	*temp;

	temp = (t_token *)malloc(sizeof(*temp));
	if (!temp)
		return (NULL);
	temp->content = ft_strdup(token->content);
	if (token->heredoc_file)
		temp->heredoc_file = token->heredoc_file;
	else
		temp->heredoc_file = NULL;
	temp->type = token->type;
	temp->next = NULL;
	return (temp);
}
