/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d@student.42.rio <lalves-d>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 18:09:02 by lalves-d          #+#    #+#             */
/*   Updated: 2025/06/05 18:11:41 by lalves-d@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset_aux(char *arg_to_unset, char ***new_envp)
{
	int		k;
	int		j;
	int		var_name_len;
	char	*equal_sign;

	k = 0;
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
			for (j = k; (*new_envp)[j]; j++)
				(*new_envp)[j] = (*new_envp)[j + 1];
		}
		else
			k++;
	}
}

int	ft_unset(t_command *cmd_info, char ***new_envp)
{
	int		i;
	char	*arg_to_unset;
	int		unset_status;

	i = 1;
	unset_status = 0;
	while ((arg_to_unset = cmd_info->args[i]))
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
	}
	get_all_env((*new_envp));
	return (unset_status);
}
