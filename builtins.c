#include "minishell.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void exit_aux(t_command *cmd_info, int i)
{
    	while (cmd_info->args[1][i])
		{
			if (!ft_isdigit(cmd_info->args[1][i]))
			{
				 fprintf(stderr, "minishell: exit: "
                "%s: numeric argument required\n", cmd_info->args[1]);

				exit(255);
			}
			i++;
		}
}
int	ft_exit(t_command *cmd_info)
{
	int		exit_code;
	long	code_val;
	int		i;

	exit_code = 0;
	i = 0;
	if (cmd_info->args[1])
	{
		if (cmd_info->args[1][0] == '-' || cmd_info->args[1][0] == '+')
			i++;
        exit_aux(cmd_info, i);
		if (cmd_info->args[2])
		{
			fprintf(stderr, "minishell: exit: too many arguments\n");
			return (1);
		}
		code_val = ft_atol(cmd_info->args[1]);
		exit_code = (int)(code_val & 0xFF);
	}
	exit(exit_code);
}

static void echo_aux(t_command *cmd_info, int *i, int all_n, int *newline)
{
	int j;

	j = 0;
		while (cmd_info->args[*i] && ft_strncmp(cmd_info->args[*i], "-n", 2) == 0)
	{
		j = 1;
		all_n = 1;
		while (cmd_info->args[*i][j])
		{
			if (cmd_info->args[*i][j] != 'n')
			{
				all_n = 0;
				break ;
			}
			j++;
		}
		if (all_n && j > 1)
		{
			*newline = 0;
			(*i)++;
		}
		else
			break ;
	}
}

int	ft_echo(t_command *cmd_info)
{
	int	i;
	int	newline;

	int	all_n;

	all_n = 0;
	i = 1;
	newline = 1;
	echo_aux(cmd_info, &i, all_n, &newline);
	while (cmd_info->args[i])
	{
		printf("%s", cmd_info->args[i]);
		i++;
		if (cmd_info->args[i])
			printf(" ");
	}
	if (newline)
		printf("\n");
	return (0);
}

int	ft_pwd(t_command *cmd_info)
{
	char	buffer[PATH_MAX];

	(void)cmd_info;
	if (getcwd(buffer, sizeof(buffer)) != NULL)
	{
		printf("%s\n", buffer);
	}
	else
	{
		perror("minishell: pwd");
	}
	return (0);
}


int	ft_env(t_command *cmd_info, char **envp_copy)
{
	int	i;

	i = 0;
	if (cmd_info->args[1])
	{
		fprintf(stderr, "env: '%s': No such file or directory\n",
			cmd_info->args[1]);
		return (1);
	}
	while (envp_copy && envp_copy[i])
	{
		if (ft_strchr(envp_copy[i], '='))
		{
			printf("%s\n", envp_copy[i]);
		}
		i++;
	}
	return (0);
}



