/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d@student.42.rio <lalves-d>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:33:41 by lalves-d          #+#    #+#             */
/*   Updated: 2025/06/05 17:35:00 by lalves-d@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int	old_pwd_home(char *current_pwd_buffer, char **old_pwd_val,
		t_command *cmd_info, char **target_path)
{
	if (getcwd(current_pwd_buffer, PATH_MAX) != NULL)
	{
		*old_pwd_val = ft_strdup(current_pwd_buffer);
		if (!(*old_pwd_val))
		{
			perror("minishell: cd: strdup error");
			return (1);
		}
	}
	if (!cmd_info->args[1] || (cmd_info->args[1][0] == '~'
		&& cmd_info->args[1][1] == '\0'))
	{
		*target_path = getenv("HOME");
		if (!(*target_path))
		{
			fprintf(stderr, "minishell: cd: HOME not set\n");
			free(*old_pwd_val);
			return (1);
		}
	}
	return (0);
}

static char	*back_dir_many_args(t_command *cmd_info, char *old_pwd_val)
{
	char	*target_path;

	if (ft_strcmp(cmd_info->args[1], "-") == 0)
	{
		target_path = getenv("OLDPWD");
		if (!target_path)
		{
			fprintf(stderr, "minishell: cd: OLDPWD not set\n");
			free(old_pwd_val);
			return (NULL);
		}
		printf("%s\n", target_path);
	}
	else
	{
		if (cmd_info->args[2])
		{
			fprintf(stderr, "minishell: cd: too many arguments\n");
			free(old_pwd_val);
			return (NULL);
		}
		target_path = cmd_info->args[1];
	}
	return (target_path);
}

static int	set_oldpwd(char *old_pwd_val, char **envp)
{
	if (old_pwd_val)
	{
		if (set_env_var(&envp, "OLDPWD", old_pwd_val) != 0)
			fprintf(stderr, "minishell: cd: error setting OLDPWD\n");
		free(old_pwd_val);
	}
	else
	{
		set_env_var(&envp, "OLDPWD", "");
	}
	return (0);
}

int	ft_cd(t_command *cmd_info, char ***envp)
{
	char	*target_path;
	char	current_pwd_buffer[1024];
	char	*old_pwd_val;

	old_pwd_val = NULL;
	old_pwd_home(current_pwd_buffer, &old_pwd_val, cmd_info, &target_path);
	target_path = back_dir_many_args(cmd_info, old_pwd_val);
	if (!target_path)
		return (1);
	if (chdir(target_path) != 0)
	{
		fprintf(stderr, "minishell: cd: %s: %s\n", target_path,
			strerror(errno));
		free(old_pwd_val);
		return (errno);
	}
	if (getcwd(current_pwd_buffer, sizeof(current_pwd_buffer)) != NULL)
	{
		if (set_env_var(envp, "PWD", current_pwd_buffer) != 0)
			fprintf(stderr, "minishell: cd: error setting PWD\n");
	}
	else
		perror("minishell: cd: getcwd error after chdir");
	set_oldpwd(old_pwd_val, *envp);
	return (0);
}
