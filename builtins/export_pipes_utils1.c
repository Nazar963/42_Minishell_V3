/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_pipes_utils1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:42:34 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/15 19:42:52 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_clean_var(t_token **token)
{
	int		i;
	int		j;
	char	*new_str;

	if (!ft_strnstr((*token)->content, "+=", ft_strlen((*token)->content)))
		return ;
	new_str = (char *)malloc(sizeof(char) * ft_strlen((*token)->content));
	i = 0;
	j = 0;
	while ((*token)->content[i])
	{
		if ((*token)->content[i] == '+')
		{
			i++;
			while ((*token)->content[i])
				new_str[j++] = (*token)->content[i++];
			new_str[j] = '\0';
			free((*token)->content);
			(*token)->content = new_str;
			return ;
		}
		new_str[j++] = (*token)->content[i++];
	}
}
