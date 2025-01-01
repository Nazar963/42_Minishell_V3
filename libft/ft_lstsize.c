/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 14:21:03 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/31 17:47:28 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_token *lst)
{
	t_token	*fun;
	int		i;

	if (!lst)
	{
		return (0);
	}
	i = 1;
	fun = lst;
	while (fun->next != NULL)
	{
		fun = fun->next;
		i++;
	}
	return (i);
}

int	ft_lstsize_delimeter(t_delimeter *lst)
{
	t_delimeter	*fun;
	int		i;

	if (!lst)
	{
		return (0);
	}
	i = 1;
	fun = lst;
	while (fun->next != NULL)
	{
		fun = fun->next;
		i++;
	}
	return (i);
}
