/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 22:07:24 by uviana-b          #+#    #+#             */
/*   Updated: 2025/06/14 22:55:02 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	***get_main_envp_addr(void)
{
	static char	**main_envp;

	return (&main_envp);
}

int	set_env_var(char ***env, const char *key, const char *value)
{
	char	*new_entry;
	size_t	key_len;
	int		i;

	key_len = ft_strlen(key);
	new_entry = malloc(ft_strlen(key) + ft_strlen(value) + 2);
	if (!new_entry)
		return (1);
	sprintf(new_entry, "%s=%s", key, value);
	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], key, key_len) == 0
			&& (*env)[i][key_len] == '=')
		{
			free((*env)[i]);
			(*env)[i] = new_entry;
			return (0);
		}
		i++;
	}
	return (append_env_var(env, new_entry, i));
}

int	append_env_var(char ***env, char *new_entry, int i)
{
	char	**new_env_array;
	int		j;

	new_env_array = malloc(sizeof(char *) * (i + 2));
	if (!new_env_array)
	{
		free(new_entry);
		return (1);
	}
	j = -1;
	while (++j < i)
		new_env_array[j] = (*env)[j];
	new_env_array[i] = new_entry;
	new_env_array[i + 1] = NULL;
	free(*env);
	*env = new_env_array;
	return (0);
}

char	**dup_env(char **envp)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	while (envp[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1024));
	if (!new_env)
	{
		return (NULL);
	}
	i = 0;
	while (envp[i])
	{
		new_env[i] = ft_strdup(envp[i]);
		i++;
	}
	j = i;
	while (i < j + 1024)
	{
		new_env[i] = NULL;
		i++;
	}
	return (new_env);
}
