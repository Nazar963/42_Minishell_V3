/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 13:07:41 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/02 19:54:50 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen_gnl(char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
	{
		i++;
	}
	return (i);
}

char	*ft_strchr_gnl(char *s, int c)
{
	if (!s)
		return (0);
	while (*s != (char)c)
	{
		if (*s == '\0')
			return (NULL);
		s++;
	}
	return ((char *)s);
}

char	*ft_strjoin_gnl(char *str, char *buff)
{
	size_t	i;
	size_t	j;
	char	*new_str;

	i = 1;
	if (!str)
	{
		str = (char *)malloc(1 * sizeof(*str));
		str[0] = '\0';
	}
	j = ft_strlen(str) + ft_strlen(buff);
	if (!str || !buff)
		return (NULL);
	new_str = malloc((j + 1) * sizeof(*new_str));
	if (new_str == NULL)
		return (NULL);
	if (str)
		while (*str != '\0' && i++)
			*(new_str++) = *(str++);
	while (*buff != '\0')
		*(new_str++) = *(buff++);
	*new_str = '\0';
	str -= (i - 1);
	free(str);
	return (new_str - j);
}
