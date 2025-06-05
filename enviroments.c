/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroments.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d@student.42.rio <lalves-d>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 00:52:43 by lalves-d          #+#    #+#             */
/*   Updated: 2025/06/05 17:36:29 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_all_env(char **envp)
{
	static char	**env;

	if (envp)
		env = envp;
	return (env);
}

static int	process_env_entry(const char *env_entry_str,
		const char *name_to_find, char **value_output)
{
	char	**split_entry;
	char	*equal_sign_pos;

	split_entry = ft_split(env_entry_str, '=');
	if (!split_entry)
		return (0);
	if (split_entry[0] && ft_strcmp(split_entry[0], name_to_find) == 0)
	{
		equal_sign_pos = ft_strchr(env_entry_str, '=');
		if (equal_sign_pos)
			*value_output = equal_sign_pos + 1;
		else
			*value_output = "\0";
		free_string_array(split_entry);
		return (1);
	}
	free_string_array(split_entry);
	return (0);
}

char	*my_get_env(char *name)
{
	char	**env_array;
	char	*value_found;
	int		i;

	i = 0;
	value_found = "\0";
	env_array = get_all_env(NULL);
	if (!name || !env_array)
		return (value_found);
	while (env_array[i])
	{
		if (process_env_entry(env_array[i], name, &value_found))
		{
			break ;
		}
		i++;
	}
	return (value_found);
}
