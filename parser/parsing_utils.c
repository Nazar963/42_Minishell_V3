/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:49:23 by nakoriko          #+#    #+#             */
/*   Updated: 2024/12/14 20:28:28 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int check_only_spaces(char **str)
{
	
	int i  = 0 ;
	while ((*str)[i])
	{
		if ((*str)[i] > 0 && (*str)[i] <= 32)
			i++;
		else
			return(1);
	}
	free(*str);
	*str = NULL;
	return(0);
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


