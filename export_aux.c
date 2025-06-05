#include "minishell.h"

 char	**bublesort(char **new_envp)
{
	int		i;
	int		j;
	char	*swap;
	int		cont;

	cont = 0;
	j = 0;
	while (new_envp[j])
	{
		i = 0;
		while (new_envp[i])
		{
			if (ft_strncmp(new_envp[j], new_envp[i],
					ft_strlen(new_envp[j])) < 0)
			{
				swap = new_envp[j];
				new_envp[j] = new_envp[i];
				new_envp[i] = swap;
			}
			i++;
			cont++;
		}
		j++;
	}
	return (new_envp);
}

 int	is_valid_var_name(const char *name, int len)
{
	int	i;

	if (len == 0 || (!ft_isalpha(name[0]) && name[0] != '_'))
		return (0);
	for (i = 1; i < len; i++)
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
	}
	return (1);
}

void	print_export_env(char **envp)
{
	int		i;
	char	*eq_pos;
	char	**temp;

	i = 0;
	temp = bublesort(envp);
	while (temp && temp[i])
	{
		printf("declare -x ");
		eq_pos = ft_strchr(temp[i], '=');
		if (eq_pos)
		{
			printf("%.*s", (int)(eq_pos - temp[i]), temp[i]);
			printf("=\"%s\"\n", eq_pos + 1);
		}
		else
		{
			printf("%s\n", temp[i]);
		}
		i++;
	}
}