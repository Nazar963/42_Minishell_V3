/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_pipes_utils1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naal-jen <naal-jen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:42:34 by naal-jen          #+#    #+#             */
/*   Updated: 2025/01/18 22:04:33 by naal-jen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_clean_var(t_token **token)
{
	int		i;
	int		j;
	char	*new_str;

	if (!ft_strnstr((*token)->content, "+=", ft_strlen((*token)->content)))
		return ;
	new_str = (char *)malloc(sizeof(char) * ft_strlen((*token)->content));
	i = 0;
	j = 0;
	while ((*token)->content[i])
	{
		if ((*token)->content[i] == '+')
		{
			i++;
			while ((*token)->content[i])
				new_str[j++] = (*token)->content[i++];
			new_str[j] = '\0';
			free((*token)->content);
			(*token)->content = new_str;
			return ;
		}
		new_str[j++] = (*token)->content[i++];
	}
}

void	ft_free_con_sp(t_con *con)
{
	free_mtx(&con->splitted);
	free_mtx(&con->splitted_2);
}

void	ft_concatenate_var(t_token **token, t_main *main)
{
	t_con	con;

	con.i = -1;
	con.splitted_2 = ft_split((*token)->content, '=');
	while (main->env[++con.i])
	{
		con.temp = ft_strjoin(con.splitted_2[0], "=");
		if (ft_strncmp(main->env[con.i], con.temp, ft_strlen(con.temp)) == 0)
		{
			con.splitted = ft_split(main->env[con.i], '=');
			con.new_str = ft_strjoin(con.splitted[0], "=");
			con.tempos = con.new_str;
			con.new_str = ft_strjoin(con.tempos, con.splitted[1]);
			free(con.tempos);
			con.tempos = con.new_str;
			con.new_str = ft_strjoin(con.new_str, con.splitted_2[1]);
			free(con.tempos);
			free(con.temp);
			free(main->env[con.i]);
			ft_free_con_sp(&con);
			main->env[con.i] = ft_strdup(con.new_str);
			return (free(con.new_str), free_linked_list(token), (void)0);
		}
		free(con.temp);
	}
}

char	*ft_clean_join(char *var)
{
	char	*new_str;
	int		i;

	i = 0;
	new_str = (char *)malloc(sizeof(char) * (ft_strlen(var) + 1));
	while (var[i] != '+')
	{
		new_str[i] = var[i];
		i++;
	}
	new_str[i] = '=';
	new_str[++i] = '\0';
	return (new_str);
}

int	ft_check_if_exits(char *var, t_main *main)
{
	int		i;
	char	*var_join;

	i = 0;
	var_join = ft_clean_join(var);
	while (main->env[i])
	{
		if (ft_strncmp(main->env[i], var_join, ft_strlen(var_join)) == 0)
			return (free(var_join), 1);
		i++;
	}
	return (free(var_join), 0);
}
