/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d@student.42.rio <lalves-d>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:18:18 by lalves-d          #+#    #+#             */
/*   Updated: 2025/05/15 10:24:30 by lalves-d@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void ft_exit(t_token *tokens, char *input)
{
    const t_token *token = tokens;

    while (token)
    {
        tokens = tokens->next;
        free(token->value);
        free((t_token*)token);
        token = tokens;
    }
    free(input);
    exit(0);
}

char *get_my_env(t_token *tokens)
{
    int i;
     char *var_name = tokens->value + 1;
            int found = 0;
            i = 0;

            while (env_vars[i])
            {
                char *eq = ft_strchr(env_vars[i], '=');
                if (eq && (size_t)(eq - env_vars[i]) == ft_strlen(var_name) &&
                    ft_strncmp(env_vars[i], var_name, eq - env_vars[i]) == 0)
                {
                    return( eq + 1); // valor da variável
                    found = 1;
                    break;
                }
                i++;
            }
            if (!found)
                return("");
            return("");
}
void ft_echo(t_token *tokens)
{
    int check = 0;
   

    tokens = tokens->next; // pula "echo"

    // verifica se tem flag -n
    if (tokens && ft_strncmp(tokens->value, "-n", 3) == 0)
    {
        check = 1;
        tokens = tokens->next;
    }

    while (tokens)
    {
        if (tokens->value[0] == '$')
        {
            printf("%s",get_my_env(tokens));
        }
        else
        {
            printf("%s", tokens->value);
        }

        if (tokens->next)
            printf(" ");
        tokens = tokens->next;
    }

    if (!check)
        printf("\n");
}

void ft_pwd()
{
    char *buffer;

    buffer =  malloc(1024);
    if(!buffer)
    {
        exit(1);
    }
    if(getcwd(buffer, 1024)!= NULL)
    {
        printf("%s \n", buffer);
    }
    else
    {
        free(buffer);
        exit(1);
    }
}
int ft_cd(t_token *tokens, char *path_name)
{
    tokens = tokens->next;
    if (!tokens->value) {
        printf("cd: missing argument\n");
        return 1;
    }

    if (chdir(tokens->value) != 0) {
        printf("cd: %s error \n", tokens->value);
        return 1;
    }
    ft_strlcpy(path_name+11, tokens->value, (ft_strlen(tokens->value)));
    ft_strlcpy(path_name+(11 + ft_strlen(tokens->value)), "$", 1);

    return 0;
}


void create_env_arr(char ***env_vars, char **env)
{
    int i = 0;

    if (!*env_vars)
    {
        *env_vars = malloc(sizeof(char *) * 1025);
        if (!*env_vars)
        {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }

        while (i < 1025)
        {
            (*env_vars)[i] = NULL;
            i++;
        }
    }

    i = 0;
    while (env[i])
    {
        size_t len = ft_strlen(env[i]) + 1;
        (*env_vars)[i] = malloc(len);
        if (!(*env_vars)[i])
        {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }

        ft_strlcpy((*env_vars)[i], env[i], len);
        i++;
    }
    (*env_vars)[i] = NULL; // termina com NULL para ser um array de strings válido
}


int max_vars(int num)
{
    static int num_vars = -1;
    if(num == 1)
        num_vars++;
    else
        num_vars--;
    if(num_vars <= 1024)
        return (num_vars);
    return (-1);
}
int test_namevar(char *name)
{
    if(ft_isalpha(name[0]) != 0 || ft_strncmp(&name[0], "_", 1) == 0)
        return(0);
    return(-1);
}


void ft_export(t_token *token)
{
    char    *eq_pos;
    int name_len;
    char *name;
    int i;
    int ret_search;
    int max_vars_num;
    char *arg = token->next->value;

    eq_pos = ft_strchr(arg, '=');
    if (!eq_pos)
    return ; // ignorar variáveis sem '='
    name_len = eq_pos - arg;
    name = malloc(sizeof(char) * (name_len + 1));
    ft_strlcpy(name, arg, name_len);
    
    if(test_namevar(name) != 0)
        write(1,"forbiden name", 13);
    else
    {
        name[name_len] = '\0';
        i = 0;
        ret_search = 0;
        while (env_vars[i] != NULL)
        {
            if (ft_strncmp(env_vars[i], name, name_len) == 0 && env_vars[i][name_len] == '=')
            {
                // mesma variável, substituir valor
                free(env_vars[i]);
                env_vars[i] = ft_strdup(arg);
                ret_search = 1;
                break;
            }
            i++;
        }
        if (ret_search == 0)
        {
            max_vars_num = max_vars(1);
            if (max_vars_num != -1)
            env_vars[max_vars_num] = ft_strdup(arg);
        }
    }
    free(name);
}

//remove it after tests
void ft_env(void)
{
    int i = 0;
    // printf("entrou \n");
    // printf("env_vars em print_export: %p\n", (void *)env_vars);
    // printf("declare -x %s\n", env_vars[i]);

    while (env_vars && env_vars[i])
    {
        printf("%s\n", env_vars[i]);
        i++;
    }
}

void ft_unset(t_token *token)
{
    char *equal_sign;
    int var_name_len;
    int i = 0;
    int j;
    char *arg=token->next->value;

    if (!arg)
        return;
    while (env_vars[i])
    {
        equal_sign = ft_strchr(env_vars[i], '=');
        if (equal_sign)
        {
            var_name_len = equal_sign - env_vars[i];
            if (ft_strlen(arg) == (size_t)var_name_len &&
                ft_strncmp(env_vars[i], arg, var_name_len) == 0)
            {
                max_vars(-1);
                ft_bzero(env_vars[i], ft_strlen(env_vars[i]));
                j = i;
                while (env_vars[j])
                {
                    env_vars[j] = env_vars[j + 1];
                    j++;
                }
                continue;
            }
        }
        i++;
    }
}
