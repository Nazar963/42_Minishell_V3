/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:19:00 by naal-jen          #+#    #+#             */
/*   Updated: 2025/02/01 16:23:09 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <termios.h>

int	g_global = 0;

static void	ft_env_copier(char **env, t_main *main)
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

static void	ctrl_d(char *input, t_token **token, t_main *main)
{
	if (input == NULL)
	{
		close(main->orig_fd[0]);
		close(main->orig_fd[1]);
		rl_clear_history();
		free_all(main, token);
		printf("exit\n");
		unlink("/tmp/heredoc.txt");
		exit(0);
	}
}

static void	init_orig_fd(t_main *main)
{
	main->orig_fd[0] = dup(STDIN_FILENO);
	main->orig_fd[1] = dup(STDOUT_FILENO);
}

void	terminal_signals_and_init(t_main *main, char **env)
{
	main->token = NULL;
	set_signals();
	ft_env_copier(env, main);
	init_orig_fd(main);
}

int	main(int ac, char **av, char **env)
{
	char			*input;
	t_main			main;
	t_token			*token;

	(void) **av;
	if (ac != 1)
		return (printf("Error\n"), 0);
	terminal_signals_and_init(&main, env);
	token = NULL;
	while (1)
	{
		input = readline("minishell> ");
		ctrl_d(input, &token, &main);
		if (ft_strlen(input) == 0 || check_only_spaces(&input) == 0)
			continue ;
		add_history(input);
		token = ft_tokenizer_main(input, &main);
		if (token == NULL)
			continue ;
		ft_path_identifier(&token, &main);
		free_token_and_input(input, &token);
	}
	rl_clear_history();
	free_str(input);
	return (free_all(&main, &token), 0);
}
