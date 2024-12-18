/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 14:51:30 by nakoriko          #+#    #+#             */
/*   Updated: 2024/12/14 20:23:04 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_tokens_check(char **tokens)
{
	int i;
	int last_was_operator;
	int redirection_count;
	int open_files_count;
	int pipe_count;
	int max_open_files;
	
	i = 0;
	last_was_operator = 1;
	redirection_count = 0;
	open_files_count = 0;
	pipe_count = 0;
	max_open_files = 256;

	while (tokens[i]) // o passare quantita di tokens esatta
	{
		if (ft_is_pipe(tokens[i])) // iniziamo dal pipe, perche non puo essere il primo argomento
		{
			if (last_was_operator == 1)
			{
				print_error("Error: syntax error near unexpected token `|'", NULL);
				return(1);
			}
			pipe_count++;
			if(pipe_count > max_open_files)
			{
					print_error("Error: too many pipes", NULL);
					return(1);
			}
			last_was_operator = 1; 
		}
		else if(ft_is_redirection(tokens[i]))
		{
			redirection_count++;
			open_files_count++;
			if (redirection_count > 1)
			{
				print_error("Error: syntax error near unexpected token '", tokens[i]);
				return (1);
			}
			if (last_was_operator)
			{
				print_error("Error: Invalid or misplaced redirection operators", NULL);
				return(1);
			}
			if (open_files_count > max_open_files)
			{
				print_error("Error: too many open files", NULL);
				return(1);
			}
			if (!tokens[i + 1])
			{
				print_error("Error: syntax error near unexpected token `newline'", NULL);
				return(1);
			}
			if (ft_is_operator(tokens[i + 1]))
			{
				print_error("Error: syntax error near unexpected token `", tokens[i + 1]);
				return(1);
			}
			//controliamo che prima era argomento (almeno non operatore, il prossimo token esiste equesto non e un operatore)
			last_was_operator = 1; 
		}
		else if (ft_is_special_char(tokens[i]))
		{
			print_error("Error: Unsupported character or operator `", tokens[i]);
			return(1);
		}
		else
		{
			last_was_operator = 0;
			redirection_count = 0;
		}
		i++;
	}
	// if (last_was_operator)
	// {
	// 	print_error("Error: syntax error near unexpected token `newline'", NULL);
	// 	return(1);
	// }
	return(0);
}
