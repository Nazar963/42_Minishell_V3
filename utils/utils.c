/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 20:44:33 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/31 18:56:41 by naal-jen         ###   ########.fr       */
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
	// printf("hello am here\n");
	while (i < size - 1)
	{
		new_mtx[i] = ft_strdup(mtx[i]);
		i++;
	}
	new_mtx[size] = NULL;
	free_mtx(mtx);
	return (new_mtx);
}

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

	i = -1;
	while (str[++i])
	{
		if (ft_isdigit(str[i]) == 1)
			return (1);
	}
	return (0);
}

void	ft_del_first_node(t_token **token)
{
	t_token	*temp;

	if (token == NULL || *token == NULL)
		return;

	temp = *token;
	if ((*token)->next == NULL)
	{
		*token = (*token)->next;
		*token = NULL;
	}
	else {
		*token = (*token)->next;
	}
	free(temp->content);
	free(temp);
}

void	ft_del_first_node_delimeter(t_delimeter **delimeter)
{
	t_delimeter	*temp;

	if (delimeter == NULL || *delimeter == NULL)
		return;

	temp = *delimeter;
	if ((*delimeter)->next == NULL)
	{
		*delimeter = (*delimeter)->next;
		*delimeter = NULL;
	}
	else {
		*delimeter = (*delimeter)->next;
	}
	free(temp->delimeter);
	free(temp);
}

void	ft_del_node(t_token **token, t_token *delete)
{
	t_token	*temp;

	if (token == NULL || *token == NULL || delete == NULL)
		return;

	if (*token == delete) {
		*token = delete->next;
		free(delete->content);
		delete->content = NULL;
		free(delete);
		return;
	}

	temp = *token;
	while (temp->next != NULL && temp->next != delete)
		temp = temp->next;
	temp->next = delete->next;
	free(delete->content);
	delete->content = NULL;
	free(delete);
}