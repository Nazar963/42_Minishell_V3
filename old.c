// void expand_wildcard() {
// 	DIR *dir;
// 	struct dirent *entry;
// 	dir = opendir(".");
// 	if (dir == NULL) {
// 		perror("opendir");
// 		return;
// 	}
// 	while ((entry = readdir(dir)) != NULL) {
// 		// Skip the "." and ".." entries
// 		if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
// 			printf("%s ", entry->d_name);
// 		}
// 	}
// 	closedir(dir);
// }

// void	ft_echo(t_token **token) {
// 	ft_del_first_node(token);
// 	if (!*token)
// 		printf("\n");
// 	while (*token && (*token)->type != TOKEN_PIPE)
// 	{
// 		if ((*token)->type == TOKEN_OPTION && (ft_strncmp((*token)->content, "-n", 2) == 0))
// 		{
// 			ft_del_first_node(token);
// 			if (ft_strncmp((*token)->content, "$?", 2) == 0
// 				&& ft_strlen((*token)->content) == 2)
// 				printf("%d ", g_global);
// 			else
// 			{
// 				while ()
// 			}
// 				if (*token && (*token)->content)
// 					printf("%s", (*token)->content);
// 			g_global = 0;
// 			ft_del_first_node(token);
// 		}
// 		else if ((*token)->next == NULL)
// 		{
// 			if (ft_strncmp((*token)->content, "$?", 2) == 0
// 				&& ft_strlen((*token)->content) == 2)
// 				printf("%d\n", g_global);
// 			// else if (ft_strncmp((*token)->content, "*", 1) == 0
// 			// 	&& ft_strlen((*token)->content) == 1)
// 			// 	{
// 			// 		expand_wildcard();
// 			// 		printf("\n");
// 			// 		ft_del_first_node(token);
// 			// 	}
// 			else
// 				printf("%s\n", (*token)->content);
// 			g_global = 0;
// 			ft_del_first_node(token);
// 		}
// 		else
// 		{
// 			if (ft_strncmp((*token)->content, "$?", 2) == 0
// 				&& ft_strlen((*token)->content) == 2)
// 				printf("%d ", g_global);
// 			else
// 				printf("%s ", (*token)->content);
// 			g_global = 0;
// 			ft_del_first_node(token);
// 			*token = (*token)->next;
// 		}
// 	}
// }

// void ft_del_node(t_token **token, t_token *delete) {
//     t_token *temp;
//     t_token *prev;
//     if (token == NULL || *token == NULL || delete == NULL)
//         return;
//     temp = *token;
//     prev = NULL;
//     while (temp != NULL) {
//         if (temp == delete) {
//             if (prev == NULL) {
//                 // The node to delete is the first node
//                 *token = temp->next;
//             } else {
//                 // The node to delete is in the middle or end
//                 prev->next = temp->next;
//             }
//             free(temp->content);
//             free(temp);
//             return;
//         }
//         prev = temp;
//         temp = temp->next;
//     }
// }

// void	ft_clean_path(char **path) {
// 	char	*temp;
// 	char	*new_str;
// 	int		i;
// 	i = 0;
// 	temp = ft_strrchr(*path, '/');
// 	while (temp[i])
// 		i++;
// 	new_str = (char *)malloc(sizeof(char) * (i + 1));
// 	if (!new_str)
// 		return ;
// 	i = 1;
// 	while (temp[i])
// 	{
// 		new_str[i - 1] = temp[i];
// 		i++;
// 	}
// 	new_str[i - 1] = '\0';
// 	free(*path);
// 	*path = ft_strdup(new_str);
// }
