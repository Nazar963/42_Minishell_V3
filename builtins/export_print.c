/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 12:40:01 by naal-jen          #+#    #+#             */
/*   Updated: 2024/12/09 17:00:51 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_env_export(t_main *main)
{
	char	**var;
	int		i;

	i = 0;
	while (main->env[i])
	{
		var = ft_split(main->env[i], '=');
		// if (var[1] && var[1][0] && ft_strncmp(main->env[i], "hi", 2) == 0)
			// printf("thello this is the result: var[0]: %c\n ", var[1][0]);
			// printf("thello this is the result: var[0]: %s,  var[1]: %s\n ", var[0], var[1]);
		if (!ft_strchr(main->env[i], '='))
			printf("declare -x %s\n", var[0]);
		else if (var[1] && var[1][0] && var[1][0] == '\"'/*  && var[1][ft_strlen(var[1] - 2)] == '\"' */) //* Which means the variable was first assigned to " " and so it has the " already inside.
			printf("declare -x %s=%s\n", var[0], var[1]);
		else if (var[1])
			printf("declare -x %s=\"%s\"\n", var[0], var[1]);
		else
			printf("declare -x %s=\"\"\n", var[0]);
		free_mtx(var);
		i++;
	}
	return ;
}