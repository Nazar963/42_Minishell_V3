/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakoriko <nakoriko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 14:14:47 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/03 18:06:28 by nakoriko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h> //* PRINTF
# include <readline/readline.h>// * READLINE
# include <readline/history.h> //* READLINE
# include <fcntl.h> //* OPEN
# include <unistd.h> //* READ
# include <stdlib.h> //* MALLOC | FREE
# include <signal.h> //* SIGNAL
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>
# include "libft/libft.h"

# define TRUE 1
# define FALSE 0

typedef struct s_token	t_token;
typedef struct s_delimeter	t_delimeter;

extern int				g_global;

typedef struct s_main
{
	char		**env;
	t_token		*token;
	int			orig_fd[2];
	int			pos_fd;
	int			pipe_count;
	int			*pids;
}				t_main;

//* -------------------------------- Natalie ------------------------------- */
typedef struct s_mtx
{
	char	*str; // stringa pulita dal spazi al inizio e il fine
	char	**tokens; // matrice di tokens
	char	*buffer; // array di tokens
	char	quote; // per tenere virgolette ed apici
	char	twin_char;
	int		len;
	int		i; // contatore di input
	int		y; //contatore di stringhe dentro char **tokens
	int		j; // contatore di caratteri dentro buffer
	int		k; // contatore per allocare le array dentro matrice(tokens)
	int		check;
	int arg_count;
	int delim_flag;
}	t_mtx;

typedef struct s_check // 
{
	int	i;
	int	last_was_operator;
	int	redirection_count;
	int	open_files_count;
	int	pipe_count;
	int	max_open_files;
	int	error_index;
}	t_check;

typedef enum error_type
{
	ERR_UNEXPECTED_PIPE,
	ERR_UNEXPECTED_REDIRECTION,
	ERR_TOO_MANY_PIPES,
	ERR_TOO_MANY_FILES,
	ERR_MISSING_FILENAME,
	ERR_UNSUPPORTED_CHAR,
	ERR_UNEXPECTED_NEWLINE
}	t_error_type;
/* -------------------------------------------------------------------------- */

//£ --------------------------------- main --------------------------------- */
int			main(int ac, char **av, char **env);

//£ -------------------------------- signals -------------------------------- */
void		ft_signals(int sig);
void		set_signals(void);
void		ft_signals_heredoc(int sig);
void		set_signals_heredoc(t_main *main, t_token **token, t_delimeter *delimeter);

//* ------------------------------------------------------------------------ */
//*                                   utils                                  */
//* ------------------------------------------------------------------------ */
//£ --------------------------------- free --------------------------------- */
void		free_mtx(char **mtx);
void		free_linked_list(t_token **token);
void		free_linked_list_delimeter(t_delimeter **delimeter);
void		free_orig_linked_list(t_token **token);
void		free_all(t_main *main, t_token **token);
void		free_str(char *str);
void		free_all_preparsing(t_mtx *data);
void		ft_free_linked_list_until_pipe(t_token **token);
//£ --------------------------------- utils -------------------------------- */
char		**ft_realloc(char **mtx, int size);
int			ft_strlen_mtx(char **mtx);
int			ft_control_int(char *str);
void		ft_del_first_node(t_token **token);
void		ft_del_first_node_delimeter(t_delimeter **delimeter);
void		ft_del_first_node_and_restructure_temp(t_token **token, t_token *temp);
void		ft_del_node(t_token **token, t_token *delete);
//£ --------------------------------- print -------------------------------- */
void		print_error(const char *str, const char *arg, char *other);
void		print_mtx(char **mtx, char *name);
void		print_token_info(t_token *token);
void		print_token_list(t_token *token);

//* ------------------------------------------------------------------------ */
//*                                 builtins                                 */
//* ------------------------------------------------------------------------ */
//£ ----------------------------- builtins_main ---------------------------- */
int			ft_builtins_main(t_token **token, t_main *main);
//£ --------------------------- builtins_main_pipes ------------------------- */
int			ft_builtins_main_pipes(t_token **token, t_main *main, int **fds);
//£ ---------------------------------- cd ---------------------------------- */
void		ft_update_env(t_main *main);
void		ft_cd(t_token **token, t_main *main);
//£ ------------------------------- cd_pipes ------------------------------- */
void		ft_update_env_pipes(t_main *main);
void		ft_cd_pipes(t_token **token, t_main *main);
//£ --------------------------------- echo --------------------------------- */
void		ft_echo(t_token **token);
//£ ------------------------------ echo_pipes ------------------------------ */
void		ft_echo_pipes(t_token **token, t_main *main);
//£ ---------------------------------- env --------------------------------- */
void		ft_env(t_token **token, t_main *main);
//£ ------------------------------- env_pipes ------------------------------ */
void		ft_env_pipes(t_token **token, t_main *main);
//£ --------------------------------- exit --------------------------------- */
int			ft_correct_exit_status_calc(int val);
void		ft_exit(t_token **token, t_main *main);
//£ ------------------------------ exit_pipes ------------------------------ */
int			ft_correct_exit_status_calc_pipes(int val);
void		ft_exit_pipes(t_token **token, t_main *main, int **fds);
//£ -------------------------------- export -------------------------------- */
void		print_env_export(t_main *main);
int			ft_check_for_already_existing_variable(char **env, char *variable);
char		**ft_export_var_reassign(char **env, char *variable, char *value);
int			is_valid_var_name(const char *name);
void		ft_export(t_token **token, t_main *main);
//£ ----------------------------- export_pipes ----------------------------- */
void		print_env_export_pipes(t_main *main);
int			ft_check_already_existing_var_pipes(char **env, char *variable);
char		**ft_export_var_reassign_p(char **env, char *variable, char *value);
int			is_valid_var_name_pipes(const char *name);
void		ft_export_pipes(t_token **token, t_main *main);
//£ ---------------------------------- pwd --------------------------------- */
void		ft_pwd(t_token **token);
//£ ------------------------------- pwd_pipes ------------------------------ */
void		ft_pwd_pipes(t_token **token);
//£ -------------------------------- unset --------------------------------- */
void		ft_unset(t_token **token, t_main *main);
//£ ------------------------------ unset_pipes ----------------------------- */
void		ft_unset_pipes(t_token **token, t_main *main);

//* ------------------------------------------------------------------------ */
//*                                  parser                                  */
//* ------------------------------------------------------------------------ */
//£ -------------------------- quotes and expansion ------------------------ */
void		ft_expand(t_mtx *data, char **env_test);
void		ft_expand_global(t_mtx *data);
void		ft_token_quote(t_mtx *data, char **env_test);
char		*check_content_quotes(char *str);

//£ ----------------------------- parsing_check ---------------------------- */
void		error_num( int n, const char *arg);
int			ft_pipe_parsig_mtxcheck(char *str, t_check *check);
int			ft_red_parsig_check(char *str, char *next, int i, t_check *check);
void		t_check_init(t_check *check);
int			ft_tokens_check(char **tokens);
//£ --------------------------- parsing_generator -------------------------- */
void		ft_tokendata_init(t_mtx *data, char *input);
void		ft_token_space(t_mtx *data);
void		ft_add_to_buffer(t_mtx *data, char *var_value);
int			ft_var_len(char *str);
char		*ft_get_env(char *str, char **env_test, int var_len);



void		ft_token_operator(t_mtx *data);
void		ft_tokens_finish(t_mtx *data);
//£ ----------------------------- parsing_main ----------------------------- */
void		preparsing_check_and_split_input(t_mtx *data, char **env);
char		**ft_token_generator(char *input, char **env);
int			is_a_builtin_check(t_token *token);
void		assign_token_type(t_token *token, t_token *prev_token);
t_token		*ft_token_list_creation(char **tokens);
t_token		*ft_tokenizer_main(char *input, t_main *main);
//£ ----------------------------- parsing_types ---------------------------- */
int			ft_is_pipe(char *token);
int			ft_is_redirection(char *token);
int			ft_is_operator(char *token);
int			ft_is_tab(char c, char next);
//£ ----------------------------- parsing_utils ---------------------------- */
int			check_only_spaces(char **str);
char		*remove_begin_end_whitespaces(char *input);
//£ ---------------------------- path_identifier --------------------------- */
void		ft_handle_exit_status_command_main(t_token **token);
void		ft_path_identifier(t_token **token, t_main *main);

//* ------------------------------------------------------------------------ */
//*                               redirections                               */
//* ------------------------------------------------------------------------ */
//£ --------------------------- redirections_main ------------------------ */
int			ft_redirection_out(t_token **token, t_main *main, t_token *temp);
int			ft_redirection_in(t_token **token, t_main *main, t_token *temp);
int			ft_append_out(t_token **token, t_main *main, t_token *temp);
char		*ft_getenv(char *name, t_main *main);
char		*ft_expaned_var(char *str, t_main *main);
void		ft_heredoc(t_token **token, t_main *main, t_token *temp);
int			ft_redirections_main(t_token **token, t_main *main);
//£ ------------------------ redirections_main_pipes ----------------------- */
void		ft_heredoc_pipes(t_token **token, t_main *main, t_token *temp);
int			ft_herdoc_pipes_main(t_token **token, t_main *main);

//* ------------------------------------------------------------------------ */
//*                                  execve                                  */
//* ------------------------------------------------------------------------ */
//£ --------------------------- execve_main_pipes -------------------------- */
void		execute_cmd_pipes(char *exec_path, char **cmd, char **envp);
int			error_file_dir_pipes(char *cmd);
int			ft_no_special_characters_pipes(char *cmd);
int			add_slash_pipes(char **new_path, char **cmd, char **envp);
char		**ft_from_list_to_array_pipes(t_token **token);
void		handle_path_pipes(t_token **token, t_main *main, int **fds);
int			child_pipes(t_token **list, t_main *main, int **fds, int pos);
int			ft_exe_main_pipes(t_token **list, t_main *main, int **fds, int pos);
//£ ----------------------------- execve_main ---------------------------- */
void		execute_cmd(char *exec_path, char **cmd, t_main *main);
int			error_file_dir(char *cmd);
int			ft_no_special_characters(char *cmd);
int			add_slash(char **new_path, char **cmd, t_main *main);
char		**ft_from_list_to_array(t_token **token);
void		handle_path(t_token **token, t_main *main);
void		child(t_token **token, t_main *main);
void		ft_execve_main(t_token **token, t_main *main);

//* ------------------------------------------------------------------------ */
//*                                   pipes                                  */
//* ------------------------------------------------------------------------ */
//£ ------------------------------ pipes_main ------------------------------ */
int			ft_check_for_pipes(t_token **token);
int			ft_count_pipes(t_token *token);
int			ft_pip_handler(t_token **list, t_main *main, int **fds, int pos_fd);
void		free_fds(int **fds, int pipe_count);
int			ft_pipes_main(t_token **token, t_main *main);

#endif