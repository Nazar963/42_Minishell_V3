/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 14:20:46 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/16 19:36:35 by naal-jen         ###   ########.fr       */
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
	temp->type = ft_token_type(temp->content);
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
	temp->type = token->type;
	temp->next = NULL;
	return (temp);
}
