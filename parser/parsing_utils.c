/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:49:23 by nakoriko          #+#    #+#             */
/*   Updated: 2024/12/05 15:26:02 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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


char	*remove_begin_end_whitespaces(char *input)
{
	int		j;
	char	*new;
	int		end;
	int		start;

	end = ft_strlen(input) - 1;
	start = 0;
	while(input[start] == ' ')
		start++;
	while (input[end] == ' ' && end > start)
		end--;
	new = malloc(sizeof(char) * (end - start) + 2);
	if (!new)
		return (NULL);
	j = 0;
	while (start <= end)
		new[j++] = input[start++];
	new[j] = '\0';
	return (new);
}
