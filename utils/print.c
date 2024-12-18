/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 16:38:31 by nakoriko          #+#    #+#             */
/*   Updated: 2024/12/14 20:25:03 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_error(const char *str, const char *arg)
{
	if (arg)
	{
		write(2, str, ft_strlen(str));
		write(2, arg, ft_strlen(arg));
		write(2, "'\n", 2);
	}
	else
	{
		write(2, str, ft_strlen(str));
		write(2, "\n", 1);
	}
}

void print_mtx(char **mtx, char *name)
{
	int i;

	i = 0;
	while (mtx[i])
	{
		printf("%s %d: %s\n", name, i, mtx[i]);
		i++;
	}
}