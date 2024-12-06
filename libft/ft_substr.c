/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 15:58:14 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/06 13:00:47 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
//#include <stdio.h>

static size_t	ft_strnlen(const char *str, size_t n)
{
	const char	*ptr;

	ptr = str;
	while (*ptr && n)
	{
		ptr++;
		n--;
	}
	return (ptr - str);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	length;
	char	*new_str;

	length = ft_strlen(s);
	if (length < start)
		start = length;
	s += start;
	len = ft_strnlen(s, len);
	new_str = (char *)malloc(len + 1);
	if (!new_str)
		return (NULL);
	ft_memcpy(new_str, s, len);
	new_str[len] = '\0';
	return (new_str);
}

/*int	main(void)
{
    char s1[] = "hello my loco";
    printf("%s\n", ft_substr(s1, 2, 5));
    
    // char s2[] = "hello my loco";
    // printf("%s\n", ft_substr(s2, 5, 2));
    
    // char s3[] = "hello my loco";
    // printf("%s\n", ft_substr(s3, 4, 15));
    
    // char s4[] = "hello my loco";
	// printf("%s\n", ft_substr(s4, 0, 13));
	//printf("%s\n", ft_substr("tripouille", 0, 42000));

	return (0);
}*/