/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:19:00 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/30 18:47:05 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_global = 0;

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

// void ft_env_copier(char **env, t_main *main) {
//     int i;
//     int len;

//     i = 0;
//     len = 0;
//     while (env[len])
//         len++;
//     main->env = (char **)malloc(sizeof(char *) * (len + 1));
//     if (main->env == NULL) {
//         perror("malloc");
//         exit(EXIT_FAILURE);
//     }
//     while (env[i]) {
//         main->env[i] = ft_strdup(env[i]);
//         if (main->env[i] == NULL) {
//             perror("ft_strdup");
//             // Free already allocated memory before exiting
//             while (--i >= 0) {
//                 free(main->env[i]);
//             }
//             free(main->env);
//             exit(EXIT_FAILURE);
//         }
//         i++;
//     }
//     main->env[i] = NULL;
// }


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


int	main(int ac, char **av, char **env)
{
	char	*input;
	t_main	main;
	t_token	*token;

	(void) **av;
	main.token = NULL;
	if (ac != 1)
		return (printf("Error\n"), 0);
	set_signals();
	ft_env_copier(env, &main);
	init_orig_fd(&main);
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
		free_linked_list(&token);
		free(input);
		input = NULL;
	}
	rl_clear_history();
	if (input)
		free(input);
	free_all(&main, &token);
	return (0);
}
