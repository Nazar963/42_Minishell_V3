# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/02 19:06:12 by naal-jen          #+#    #+#              #
#    Updated: 2024/12/16 18:17:18 by naal-jen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc -g
FLAGS = -Wall -Wextra -Werror
READLINE = -lreadline

NAME = minishell
RM = rm -rf

SRC = main.c signals.c\
	builtins/export_print.c\
	parser/parsing_check.c parser/parsing_generator.c parser/parsing_main.c parser/parsing_types.c parser/parsing_utils.c parser/path_identifier.c\
	redirections/redirections_main.c\
	utils/free.c utils/utils.c utils/print.c\
	execve/execve_main.c\
	pipes/pipes_main.c
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