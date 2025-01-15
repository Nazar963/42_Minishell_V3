# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nakoriko <nakoriko@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/02 19:06:12 by naal-jen          #+#    #+#              #
#    Updated: 2025/01/13 12:41:50 by nakoriko         ###   ########.fr        #
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
	builtins/export_pipes_utils.c\
	parser/parsing_check.c parser/preparsing_split.c parser/parsing_main.c parser/parsing_types.c parser/parsing_utils.c parser/parsing_tokens_checks.c parser/path_identifier.c\
	redirections/redirections_main.c redirections/redirections_main_utils0.c redirections/redirections_main_utils1.c redirections/redirections_main_pipes.c redirections/redirections_main_pipes_utils0.c\
	utils/free.c utils/utils.c utils/print.c utils/utils1.c\
	execve/execve_main.c execve/execve_main_pipes.c execve/execve_main_pipes_utils0.c execve/execve_main_pipes_utils1.c execve/execve_main_pipes_utils2.c\
	pipes/pipes_main.c pipes/pipes_main_utils.c parser/quotes_and_expansion.c parser/token_list_creation.c utils/free_lists.c
OBJ = $(SRC:%.c=$(OBJDIR)/%.o)

OBJDIR = .objectFiles
HEADER = minishell.h

all: LIBFT $(NAME)

LIBFT:
	@make -C libft

$(NAME): $(OBJ) LIBFT
	@$(CC) $(FLAGS) $(SRC) -o $(NAME) libft/libft.a $(READLINE)

$(OBJDIR)/%.o: %.c $(HEADER)
	@mkdir -p $(dir $@)
	@$(CC) $(FLAGS) $(READLINE) -c $< -o $@

clean:
	@$(RM) $(OBJDIR)
	@$(RM) $(OBJ)
	@make -C libft clean

fclean: clean
	@$(RM) $(NAME)
	@make -C libft fclean

re: fclean all

.PHONY: all clean fclean re