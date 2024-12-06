/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 14:14:47 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/06 15:57:20 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h> //* PRINTF
#include <readline/readline.h>// * READLINE
#include <readline/history.h> //* READLINE
#include <fcntl.h> //* OPEN
#include <unistd.h> //* READ
#include <stdlib.h> //* MALLOC | FREE
#include <signal.h> //* SIGNAL
#include "libft/libft.h"

typedef struct s_token t_token;

//£ ---------------------------------- main ---------------------------------- */
void		welcome_mat(void);
int			main(int ac, char **av, char **env);

//£ ---------------------------------- signals ---------------------------------- */
void		ft_signals(int sig);
void		set_signals();

//* -------------------------------------------------------------------------- */
//*                                    utils                                   */
//* -------------------------------------------------------------------------- */
//£ ---------------------------------- free ---------------------------------- */
void		free_mtx(char **mtx);

//* -------------------------------------------------------------------------- */
//*                                  builtins                                  */
//* -------------------------------------------------------------------------- */
//£ ------------------------------ builtins_main ----------------------------- */
int			builtins_main();

//* -------------------------------------------------------------------------- */
//*                                   parser                                   */
//* -------------------------------------------------------------------------- */
//£ ------------------------------ parsing_utils ----------------------------- */
char		*remove_begin_end_whitespaces(char *input);
void		print_mtx(char **mtx, char *name);
//£ ------------------------------ parsing_main ------------------------------ */
char 		**ft_token_generator(char *input);
t_token		*ft_token_list_without_redirection(char **tokens);
char		**ft_tokenizer_main(char *input);

#endif