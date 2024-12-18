/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:19:00 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/15 17:37:57 by naal-jen         ###   ########.fr       */
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


void	ft_env_copier(char **env, t_main *main)
{
	int		i;
	int		len;

	i = 0;
	len = 0;
	while (env[len])
		len++;
	main->env = (char **)malloc(sizeof(char *) * (len + 1));
	if (main->env == NULL)
		return ;
	while (env[i])
	{
		main->env[i] = ft_strdup(env[i]);
		i++;
	}
	main->env[i] = NULL;
}


static void	ctrl_d(char *input)
{
	if (input == NULL)
	{
		printf("exit\n");
		exit(0);
	}
}

void	init_orig_fd(t_main *main)
{
	main->orig_fd[0] = dup(STDIN_FILENO);
	main->orig_fd[1] = dup(STDOUT_FILENO);
}

//TODO: Cambia il prompt per essere uguale a quello del bash.
int	main(int ac, char **av, char **env)
{
	char	*input;
	t_main	main;
	t_token	*token;

	(void) **av;
	(void) **env;
	if (ac != 1)
		return (printf("Error\n"), 0);
	welcome_mat();
	set_signals();
	ft_env_copier(env, &main);
	init_orig_fd(&main);
	while (1)
	{
		input = readline("︻╦̵̵̿╤─ ҉~• ");
		ctrl_d(input);
		if (ft_strlen(input) == 0 || check_only_spaces(&input) == 0)
			continue ;

		token = ft_tokenizer_main(input, &main);
		ft_path_identifier(token, &main);

		add_history(input);
		free(input);
		input = NULL;
	}
	rl_clear_history();
	if (input)
		free(input);
	free_all(&main);
	return (0);
}
