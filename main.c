/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakoriko <nakoriko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:19:00 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/13 18:18:25 by nakoriko         ###   ########.fr       */
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

void	terminal_signals_and_init(struct termios *term, t_main *main, char **env)
{
	(void) env;
	tcgetattr(STDIN_FILENO, term);
	term->c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, term);
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
	struct termios	term;

	(void) **av;
	if (ac != 1)
		return (printf("Error\n"), 0);
	terminal_signals_and_init(&term, &main, env);
	// tcgetattr(STDIN_FILENO, &term);
	// term.c_lflag &= ~(ECHOCTL);
	// tcsetattr(STDIN_FILENO, TCSAFLUSH, &term); 
	// main.token = NULL; 
	//set_signals(); 
	// ft_env_copier(env, &main);-- spostato dentro funzione precedente
	//init_orig_fd(&main);
	token = NULL;
	while (1)
	{
		input = readline("minishell> ");
		ctrl_d(input, &token, &main);
		if (ft_strlen(input) == 0 || check_only_spaces(&input) == 0)
			continue ;
		add_history(input);
		token = ft_tokenizer_main(input, &main);
		// if (g_global == 667) - spostato in funzione sopra
		// 	g_global = 0;
		if (token == NULL)
			continue ;
		ft_path_identifier(&token, &main);
		free_token_and_input(input, &token);
		// free_linked_list(&token); - spostato in funzione precedente
		// free_str(input);
	}
	rl_clear_history();
	// if (input) - sembra imposibile che a questo punto sara diverso da null
	// 	free(input);
	return (free_all(&main, &token), 0);
}

// int	main(int ac, char **av, char **env)
// {
// 	char			*input;
// 	t_main			main;
// 	t_token			*token;
// 	struct termios	term;

// 	(void) **av;
// 	tcgetattr(STDIN_FILENO, &term);
// 	term.c_lflag &= ~(ECHOCTL);
// 	tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
// 	main.token = NULL;
// 	if (ac != 1)
// 		return (printf("Error\n"), 0);
// 	set_signals();
// 	ft_env_copier(env, &main);
// 	init_orig_fd(&main);
// 	token = NULL;
// 	while (1)
// 	{
// 		input = readline("minishell> ");
// 		ctrl_d(input, &token, &main);
// 		if (ft_strlen(input) == 0 || check_only_spaces(&input) == 0)
// 			continue ;
// 		add_history(input);
// 		token = ft_tokenizer_main(input, &main);
// 		if (g_global == 667)
// 			g_global = 0;
// 		if (token == NULL)
// 			continue ;
// 		ft_path_identifier(&token, &main);
// 		free_linked_list(&token);
// 		free_str(input);
// 	}
// 	rl_clear_history();
// 	if (input)
// 		free(input);
// 	free_all(&main, &token);
// 	return (0);
// }
