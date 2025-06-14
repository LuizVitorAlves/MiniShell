/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d@student.42.rio <lalves-d>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:31:44 by lalves-d          #+#    #+#             */
/*   Updated: 2025/06/14 18:03:51 by lalves-d@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_aux(t_command *cmd_info, int i)
{
	while (cmd_info->args[1][i])
	{
		if (!ft_isdigit(cmd_info->args[1][i]))
		{
			fprintf(stderr,
				"minishell: exit: "
				"%s: numeric argument required\n",
				cmd_info->args[1]);
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
	shell_exit(exit_code);
	return (1);
}
