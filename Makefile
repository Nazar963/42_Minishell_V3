# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/02 19:06:12 by naal-jen          #+#    #+#              #
#    Updated: 2024/12/04 12:44:59 by naal-jen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc -g
FLAGS = -Wall -Wextra -Werror
READLINE = -lreadline

NAME = minishell
RM = rm -rf

SRC = main.c parser/parsing_main.c parser/parsing_utils.c utils/free.c signals.c
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