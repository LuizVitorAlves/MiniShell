/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 18:09:02 by lalves-d          #+#    #+#             */
/*   Updated: 2025/06/14 19:47:25 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	suport_aux(char ***new_envp, char *arg_to_unset, int k, int j)
{
	char	*equal_sign;
	int		var_name_len;

	while ((*new_envp)[k])
	{
		equal_sign = ft_strchr((*new_envp)[k], '=');
		if (equal_sign)
			var_name_len = equal_sign - (*new_envp)[k];
		else
			var_name_len = ft_strlen((*new_envp)[k]);
		if (ft_strlen(arg_to_unset) == (size_t)var_name_len
			&& ft_strncmp((*new_envp)[k], arg_to_unset, var_name_len) == 0)
		{
			free((*new_envp)[k]);
			while ((*new_envp)[j])
			{
				j = k;
				(*new_envp)[j] = (*new_envp)[j + 1];
				j++;
			}
		}
		else
			k++;
	}
}

void	unset_aux(char *arg_to_unset, char ***new_envp)
{
	int	k;
	int	j;

	k = 0;
	j = 0;
	suport_aux(new_envp, arg_to_unset, k, j);
}

int	ft_unset(t_command *cmd_info, char ***new_envp)
{
	int		i;
	char	*arg_to_unset;
	int		unset_status;

	i = 1;
	unset_status = 0;
	arg_to_unset = cmd_info->args[i];
	while (arg_to_unset)
	{
		if (!is_valid_var_name(arg_to_unset, ft_strlen(arg_to_unset)))
		{
			fprintf(stderr, "minishell: unset: `%s': not a valid identifier\n",
				arg_to_unset);
			unset_status = 1;
		}
		else
		{
			unset_aux(arg_to_unset, new_envp);
		}
		i++;
		arg_to_unset = cmd_info->args[i];
	}
	get_all_env((*new_envp));
	return (unset_status);
}
