/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 20:44:33 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/26 18:15:34 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_realloc(char **mtx, int size)
{
	char	**new_mtx;
	int		i;

	i = 0;
	new_mtx = (char **)malloc(sizeof(char *) * (size + 1));
	if (new_mtx == NULL)
		return (NULL);
	while (i < size - 1)
	{
		new_mtx[i] = ft_strdup(mtx[i]);
		i++;
	}
	new_mtx[size] = NULL;
	free_mtx(&mtx);
	return (new_mtx);
}

char	*ft_realloc_str(char *mtx, int x, int size)
{
	char	*new_mtx;
	int		i;
	int		len;

	(void)x;
	i = 0;
	if (mtx)
		len = size + ft_strlen(mtx);
	else
		len = size;
	new_mtx = (char *)malloc(sizeof(char ) * len);
	ft_memset(new_mtx, 0, len);
	if (new_mtx == NULL)
		return (NULL);
	if (mtx)
	{
		while (mtx[i])
		{
			new_mtx[i] = mtx[i];
			i++;
		}
	}
	free_str(mtx);
	return (new_mtx);
}

// char	*ft_realloc_str(char *mtx, int x, int size)
// {
// 	char	*new_mtx;
// 	int		i;

// 	i = 0;
// 	new_mtx = (char *)malloc(sizeof(char ) * (size));
// 	if (new_mtx == NULL)
// 		return (NULL);
// 	while (i < x)
// 	{
// 		new_mtx[i] = mtx[i];
// 		i++;
// 	}
// 	new_mtx[i] = '\0';
// 	free_str(mtx);
// 	return (new_mtx);
// }

int	ft_strlen_mtx(char **mtx)
{
	int	i;

	i = 0;
	while (mtx[i])
		i++;
	return (i);
}

int	ft_control_int(char *str)
{
	int	i;
	int	loco;

	i = -1;
	loco = 0;
	if (!str[0])
		return (0);
	while (str[++i])
	{
		if ((str[i] == '+' || str[i] == '-') && loco == 0)
			loco++;
		else if (ft_isdigit(str[i]) == 0)
			return (0);
	}
	return (1);
}

void	ft_del_first_node(t_token **token)
{
	t_token	*temp;

	if (token == NULL || *token == NULL)
		return ;
	temp = *token;
	if ((*token)->next == NULL)
	{
		*token = (*token)->next;
		*token = NULL;
	}
	else
	{
		*token = (*token)->next;
	}
	free(temp->content);
	free(temp);
}
