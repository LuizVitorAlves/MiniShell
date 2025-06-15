/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 22:07:37 by uviana-b          #+#    #+#             */
/*   Updated: 2025/06/14 22:55:39 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_cmd_path_aux(char **paths, char *cmd)
{
	int		i;
	size_t	buffer_size;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		buffer_size = ft_strlen(paths[i]) + ft_strlen(cmd) + 2;
		full_path = malloc(buffer_size);
		if (!full_path)
			return (NULL);
		ft_strlcpy(full_path, paths[i], buffer_size);
		ft_strlcat(full_path, "/", buffer_size);
		ft_strlcat(full_path, cmd, buffer_size);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*get_cmd_path(char *cmd, char **envp)
{
	int		i;
	char	*path_var;
	char	**paths;
	char	*full_path;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	path_var = envp[i] + 5;
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	full_path = get_cmd_path_aux(paths, cmd);
	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
	return (full_path);
}
