/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:19:00 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/06 15:21:52 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	welcome_mat(void)
{
	int		fd;
	char	*line;
	int		i;

	i = 0;
	fd = open("utils/Design.txt", O_RDONLY);
	line = (char *)malloc(sizeof(char) * 9);
	i = read(fd, line, 8);
	while (i)
	{
		line[i] = '\0';
		printf("%s", line);
		free(line);
		line = NULL;
		line = (char *)malloc(sizeof(char) * 9);
		i = read(fd, line, 8);
	}
	if (line)
		free(line);
}

int	main(int ac, char **av, char **env)
{
	char	*input;
	char	**tokens;


	(void) **av;
	(void) **env;
	if (ac != 1)
		return (printf("Error\n"), 0);
	welcome_mat();
	set_signals();
	while (1)
	{
		input = readline("︻╦̵̵̿╤─ ҉~• ");
		if (input == NULL)
			break ;
		if (ft_strlen(input) == 0)
		{
			free(input);
			continue ;
		}
		tokens = ft_tokenizer_main(input);
		if (tokens != NULL)
		{
			print_mtx(tokens, "Token N ");
			free_mtx(tokens);
		}
		add_history(input);
		free(input);
		input = NULL;
	}
	rl_clear_history();
	if (input)
		free(input);
	return (0);
}
