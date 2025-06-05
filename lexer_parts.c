#include "minishell.h"
char **get_all_env(char **envp)
{
	static char **env;
	if(envp)
		env = envp;
	return(env);
}
char *my_get_env(char *name)
{
	char **new_env;
	int i;
	char *result;
	char **new_env_token;

	result = "\0";
	i = 0;
	new_env = get_all_env(NULL);
	while(new_env[i])
	{
		new_env_token=ft_split(new_env[i], '=');
		if(ft_strncmp(new_env[i], name, ft_strlen(new_env_token[0])) == 0) //DAR FREE MESSA TESTE
		{
			result = ft_strchr(new_env[i],'=');
			if(result && result[0])
				result++;
			break;
		}
		i++;
	}
	//dar free aqui
	return(result);
}
static void	expand_env_var(const char *line, int *i, t_builder *builder )
{
	int		start;
	char	*var_name;
	char	*var_value;

	start = *i;
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
		(*i)++;
	if (*i == start)
	{
		builder_append_char(builder, '$');
		return ;
	}
	var_name = ft_substr(line, start, *i - start);
	var_value = my_get_env(var_name);
	free(var_name);
	if (var_value)
		builder_append_str(builder, var_value);
}
int	exit_status(int status)
{
	static int new_status;
	
	if(status >= 0)
		new_status =  status;
	return(new_status);
}
void	handle_expansion(const char *line, int *i, t_builder *builder)
{
	char	*var_value;

	(*i)++;
	if (line[*i] == '?')
	{
		var_value = ft_itoa(exit_status(-1));
		builder_append_str(builder, var_value);
		free(var_value);
		(*i)++;
	}
	else
	{
		expand_env_var(line, i, builder);
	}
}

void	get_unquoted_part(const char *line, int *i, t_builder *builder)
{
	if (line[*i] == '$')
		handle_expansion(line, i, builder);
	else
	{
		builder_append_char(builder, line[*i]);
		(*i)++;
	}
}

int	get_quoted_part(const char *line, int *i, t_builder *builder)
{
	char	quote_type;

	quote_type = line[*i];
	(*i)++;
	while (line[*i] && line[*i] != quote_type)
	{
		if (quote_type == '\"' && line[*i] == '$')
			handle_expansion(line, i, builder);
		else
		{
			builder_append_char(builder, line[*i]);
			(*i)++;
		}
	}
	if (line[*i] == quote_type)
	{
		(*i)++;
		return (1);
	}
	ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
	return (0);
}
