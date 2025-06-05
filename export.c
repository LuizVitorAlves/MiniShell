
#include "minishell.h"

int aux_support(char *arg, char **name, char **value, char *eq_pos)
{
    int name_len;

    name_len = eq_pos - arg;
    if (name_len == 0)
    {
        fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", arg);
        return (1);
    }
    *name = ft_strndup(arg, name_len);
    if (!(*name))
        return (1);
    *value = eq_pos + 1;
    if (!is_valid_var_name(*name, name_len))
    {
        fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", arg);
        free(*name);
        return (1);
    }
    return (0);
}
static void error_set_var(int *export_status, char *arg)
{
    fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", arg);
    *export_status = 1;

}
static int export_aux_fun(char *eq_pos, char *arg, char ***new_envp)
{
    int		export_status = 0;
    char	*name;
    char	*value;

    if (eq_pos)
    {
        if (aux_support(arg, &name, &value, eq_pos))
            return (1);
        if (set_env_var(new_envp, name, value) != 0)
        {
            fprintf(stderr, "minishell: export: error setting variable %s\n", name);
            export_status = 1;
        }
        free(name);
    }
    else
    {
        if (!is_valid_var_name(arg, ft_strlen(arg)))
            error_set_var(&export_status, arg);
    }
    return (export_status);
}



int	ft_export(t_command *cmd_info, char ***new_envp)
{
	int		i;
	char	*arg;
	char	*eq_pos;
    int		export_status = 0;

	if (!cmd_info->args[1])
	{
		print_export_env(*new_envp);
		return (0);
	}
	i = 1;
	while ((arg = cmd_info->args[i]))
	{
		eq_pos = ft_strchr(arg, '=');
		if (export_aux_fun(eq_pos, arg, new_envp) != 0)
			export_status = 1;
		i++;
	}
	get_all_env(*new_envp);
	return (export_status);
}
