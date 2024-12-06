/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 12:25:27 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/05 15:35:32 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	ft_lstclear(t_token **lst, void (*del)(void*))
{
	t_token	*temp1;

	temp1 = *lst;
	if (!*lst)
		return ;
	while (*lst)
	{
		temp1 = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = temp1;
	}
	*lst = NULL;
}
