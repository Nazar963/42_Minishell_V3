/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 14:51:30 by nakoriko          #+#    #+#             */
/*   Updated: 2024/12/30 15:36:24 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void error_num( int n, const char *arg)
{
	if (n == 1)
		print_error("Error: syntax error near unexpected token `", arg, NULL);
	else if (n == 2)
		print_error("Error: Invalid or misplaced redirection operators", arg, NULL);
	else if (n == 3)
		print_error("Error: too many open files", arg, NULL);
	else if (n == 4)
		print_error("Error: syntax error near unexpected token `newline'", 
		arg, NULL);
	else if (n == 5)
		print_error("Error: Unsupported character or operator `", arg, NULL);
	else if (n == 6)
		print_error("Error: too many pipes", arg, NULL);
	g_global = 2;
}


int ft_pipe_parsig_mtxcheck(char *str, t_check *check)
{
	
	if (check->last_was_operator == 1)
	{
		error_num(1, str);
		return(1);
	}
	check->pipe_count++;
	if(check->pipe_count > check->max_open_files)
	{
		error_num(6, NULL);
		return(1);
	}
	check->last_was_operator = 1; 
	return(0);
}

int ft_red_parsig_check(char *str, char *next, int i, t_check *check)
{
	(void) i;
	check->redirection_count++;
	check->open_files_count++;
	if (check->redirection_count > 1)
		return(error_num(1, str), 1);
	// else if (check->last_was_operator && i > 0)
	// 	return(error_num(2, NULL), 1);
	else if (check->open_files_count > check->max_open_files)
		return(error_num(3, NULL), 1);
	else if (!next)
		return(error_num(4, NULL), 1);
	else if (ft_is_operator(next))
		return(error_num(1, next), 1);
	// else if (!next)
	// 	return(error_num(4, NULL), 1);
	//controliamo che prima era argomento (almeno non operatore, il prossimo token esiste equesto non e un operatore)
	check->last_was_operator = 1; 
	return(0);
}

void t_check_init(t_check *check)
{
	check->last_was_operator = 1;
	check->redirection_count = 0;
	check->open_files_count = 0;
	check->pipe_count = 0;
	check->max_open_files = 256;
	check->error_index = 0;

}

int ft_tokens_check(char **tokens)
{
	t_check check;	
	int i;

	i = 0;
	t_check_init(&check);
	while (tokens[i]) // o passare quantita di tokens esatta
	{
		if (ft_is_pipe(tokens[i]))
		{
			if (ft_pipe_parsig_mtxcheck(tokens[i], &check) == 1) // iniziamo dal pipe, perche non puo essere il primo argomento
				return(1);
		}
		else if(ft_is_redirection(tokens[i]))
		{
			if (ft_red_parsig_check(tokens[i], tokens[i + 1], 
			i, &check) == 1)
				return (1);
		}
		else
		{
			check.last_was_operator = 0;
			check.redirection_count = 0;
		}
		i++;
	}
	if (check.last_was_operator)
		return(error_num(4, NULL), 1);
	return(0);
}
