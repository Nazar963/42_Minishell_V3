/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakoriko <nakoriko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 14:14:47 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/20 19:05:00 by nakoriko         ###   ########.fr       */
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
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>
#include "libft/libft.h"

#define true 1
#define false 0

typedef struct s_token t_token;
// extern	g_global;

extern int	g_global;


typedef struct s_main
{
	char		**env;
	t_token		*token;
	int			orig_fd[2];
	int			pos_fd;
}				t_main;

//* --------------------------------- Natalie -------------------------------- */
typedef struct s_mtx // 
{
	char *str; // stringa pulita dal spazi al inizio e il fine
	char **tokens; // matrice di tokens
	char *buffer; // array di tokens
	int len;
	int i;  // contatore di input
	int y;//contatore di stringhe dentro char **tokens
	int j; // contatore di caratteri dentro buffer
	char quote; // per tenere virgolette ed apici
	char twin_char;
	int k; // contatore per allocare le array dentro matrice(tokens)
	int check;
} t_mtx;

typedef enum {
	ERR_UNEXPECTED_PIPE,
	ERR_UNEXPECTED_REDIRECTION,
	ERR_TOO_MANY_PIPES,
	ERR_TOO_MANY_FILES,
	ERR_MISSING_FILENAME,
	ERR_UNSUPPORTED_CHAR,
	ERR_UNEXPECTED_NEWLINE
} error_type;
/* -------------------------------------------------------------------------- */

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
void		free_linked_list(t_token *token);
// void		free_all(t_main *main);
void		free_all(t_main *main, t_token **token);
void		free_str(char *str);
void		free_all_preparsing (t_mtx *data);
//£ ---------------------------------- utils --------------------------------- */
char		**ft_realloc(char **mtx, int size);
int			ft_strlen_mtx(char **mtx);
void		free_linked_list(t_token *token);
//void		free_all(t_main *main);
//£ ---------------------------------- print --------------------------------- */
void		print_error(const char *str, const char *arg);
void		print_mtx(char **mtx, char *name);

//* -------------------------------------------------------------------------- */
//*                                  builtins                                  */
//* -------------------------------------------------------------------------- */
//£ ------------------------------ builtins_main ----------------------------- */
int			builtins_main();
//£ ------------------------------ export_print ------------------------------ */
void		print_env_export(t_main *main);

//* -------------------------------------------------------------------------- */
//*                                   parser                                   */
//* -------------------------------------------------------------------------- */
//£ ------------------------------ parsing_check ----------------------------- */
int			ft_tokens_check(char **tokens);
//£ ---------------------------- parsing_generator --------------------------- */
void		ft_tokendata_init(t_mtx *data, char *input);
void		ft_token_space(t_mtx *data);
void		ft_add_to_buffer(t_mtx *data, char *var_value);
int			ft_var_len(char *str);
char		*ft_get_env(char *str, char **env_test, int var_len);
void		ft_expand(t_mtx *data, char **env_test);
void		ft_token_quote(t_mtx *data, char **env_test);
void		ft_token_operator(t_mtx *data);
void		ft_tokens_finish(t_mtx *data);
void		ft_expand_global(t_mtx *data);
//£ ------------------------------ parsing_main ------------------------------ */
void		preparsing_check_and_split_input(t_mtx *data, char **env);
char		**ft_token_generator(char *input, char **env);
void		first_token_type_assigning(t_token **token);
t_token		*ft_token_list_creation(char **tokens);
t_token		*ft_tokenizer_main(char *input, t_main *main);
//£ ------------------------------ parsing_types ----------------------------- */
int			ft_is_pipe(char *token);
int			ft_is_redirection(char *token);
int			ft_is_operator(char *token);
int			ft_is_special_char(char *token);
int			ft_is_tab(char c, char next);
//£ ------------------------------ parsing_utils ----------------------------- */
int			check_only_spaces(char **str);
char		*remove_begin_end_whitespaces(char *input);
//£ ----------------------------- path_identifier ---------------------------- */
void		ft_del_first_node(t_token **token);
void		ft_del_node(t_token **token, t_token *delete);
void		ft_echo(t_token **token);
void		ft_pwd(t_token **token);
void		ft_cd(t_token **token);
int			ft_check_for_already_existing_variable(char **env, char *variable);
char		**ft_export_variable_reassign(char **env, char *variable, char *value);
void		ft_export(t_token **token, t_main *main);
void		ft_unset(t_token **token, t_main *main);
void		ft_env(t_token **token, t_main *main);
void		ft_exit(t_token **token);
int			ft_check_for_builtin(t_token **token, t_main *main);
void		ft_path_identifier(t_token *token, t_main *main);

//* -------------------------------------------------------------------------- */
//*                                redirections                                */
//* -------------------------------------------------------------------------- */
//£ ---------------------------- redirections_main ------------------------- */
void		ft_redirection_out(t_token **token, t_main *main, t_token *temp);
void		ft_redirection_in(t_token **token, t_main *main, t_token *temp);
void		ft_append_out(t_token **token, t_main *main, t_token *temp);
void		ft_heredoc(t_token **token, t_main *main, t_token *temp);
int			ft_redirections_main(t_token **token, t_main *main);
//£ ------------------------- redirections_main_pipes ------------------------ */
void		ft_heredoc_pipes(t_token **token, t_token *temp);
int			ft_herdoc_pipes_main(t_token **token);

//* -------------------------------------------------------------------------- */
//*                                   execve                                   */
//* -------------------------------------------------------------------------- */
//£ ------------------------------ execve_main ----------------------------- */
void		execute_cmd(char *exec_path, char **cmd, char **envp);
void		add_slash(char **new_path, char **cmd, char **envp);
char		**ft_from_list_to_array(t_token **token);
void		handle_path(t_token **token, char **envp);
void		handler(int case_num);
void		child(t_token **token, char **envp);
void		ft_execve_main(t_token **token, t_main *main);
void		error_cmd(char *cmd);
//£ ---------------------------- execve_main_pipes --------------------------- */
void		execute_cmd_pipes(char *exec_path, char **cmd, char **envp);
void		error_cmd_pipes(char *cmd);
void		add_slash_pipes(char **new_path, char **cmd, char **envp);
char		**ft_from_list_to_array_pipes(t_token **token);
void		handle_path_pipes(t_token **token, char **envp);
void		handler_pipes(int case_num);
void		ft_execve_main_pipes(t_token **list, t_main *main, int **fds, int pos);
void		child_pipes(t_token **list, t_main *main, int **fds, int pos);

//* -------------------------------------------------------------------------- */
//*                                    pipes                                   */
//* -------------------------------------------------------------------------- */
//£ ------------------------------- pipes_main ------------------------------- */
int			ft_check_for_pipes(t_token **token);
int			ft_pipes_main(t_token **token, t_main *main);

#endif