# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/02 19:06:12 by naal-jen          #+#    #+#              #
#    Updated: 2025/02/06 18:03:20 by naal-jen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc -g
FLAGS = -Wall -Wextra -Werror
READLINE = -lreadline

NAME = minishell
RM = rm -rf

SRC = main.c signals.c\
	builtins/builtins_main.c builtins/cd.c builtins/echo.c builtins/env.c builtins/exit.c builtins/export.c builtins/pwd.c builtins/unset.c\
	builtins/builtins_main_pipes.c builtins/cd_pipes.c builtins/echo_pipes.c builtins/env_pipes.c builtins/exit_pipes.c builtins/export_pipes.c builtins/pwd_pipes.c\
	builtins/export_pipes_utils.c builtins/export_pipes_utils1.c builtins/export_pipes_utils2.c\
	parser/parsing_check.c parser/preparsing_split.c parser/parsing_main.c parser/parsing_types.c parser/parsing_utils.c parser/parsing_tokens_checks.c parser/path_identifier.c\
	redirections/redirections_main.c redirections/redirections_main_utils0.c redirections/redirections_main_utils1.c redirections/redirections_main_utils2.c redirections/redirections_main_pipes.c redirections/redirections_main_pipes_utils0.c\
	utils/free.c utils/utils.c utils/print.c utils/utils1.c\
	execve/execve_main.c execve/execve_main_pipes.c execve/execve_main_pipes_utils0.c execve/execve_main_pipes_utils1.c execve/execve_main_pipes_utils2.c execve/execve_main_pipes_cl_fr.c\
	pipes/pipes_main.c pipes/pipes_main_utils.c pipes/pipes_main_utils1.c parser/quotes_and_expansion.c parser/token_list_creation.c utils/free_lists.c
OBJ = $(SRC:%.c=$(OBJDIR)/%.o)

OBJDIR = .objectFiles
HEADER = minishell.h

all: $(NAME)

$(NAME): $(OBJ)
	@make -C libft
	@$(CC) $(FLAGS) $(SRC) -o $(NAME) libft/libft.a $(READLINE)

$(OBJDIR)/%.o: %.c $(HEADER)
	@mkdir -p $(dir $@)
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJDIR)
	@$(RM) $(OBJ)
	@make -C libft clean

fclean: clean
	@$(RM) $(NAME)
	@make -C libft fclean

re: fclean all

.PHONY: all clean fclean re