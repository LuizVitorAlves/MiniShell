/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:18:18 by lalves-d          #+#    #+#             */
/*   Updated: 2025/04/07 15:18:21 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void ft_exit(t_token *tokens, char *input)
{
    free(tokens);
    free(input);
    exit(0);
}
void   ft_echo(t_token *tokens)
{
    int check;
   
    check = 0;
    tokens = tokens->next;
    if(ft_strncmp(tokens->value, "-n",2) == 0)
    {
        check =1;
        tokens = tokens->next;
    }
    while(tokens->next)
    {   
        
        printf("%s ", tokens->value);
        tokens = tokens->next;
    }
    if(check ==0)
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
// void create_env_arr(char **env_vars)
// {
//     static int ctrl_env = 0;
//     if(ctrl_env > 0)
//     {
//         int i;

//         i = 0;
//         while(i <= 1024)
//         {
//             env_vars[i] = malloc(sizeof(*char));
//             i++;
//         }
//         i = 0;
//         while(i <= 1024)
//         {
//             env_vars[i] = NULL;
//             i++;
//         }
//         ctrl_env++;
//     }

// }
// int max_vars()
// {
//     static int num_vars = -1;

//     num_vars++;
//     if(num_vars <= 1024)
//         return (num_vars);
//     return (-1);

// }

// void ft_export(char *arg)
// {
//     static  char **env_vars;
//     char    *eq_pos;
//     int name_len;
//     char *name;
//     int i;
//     int ret_search;
//     int max_vars_num;

//     eq_pos = ft_strchr(arg, '=');
//     name_len = eq_pos - arg;
//     name = malloc(sizeof(char) * name_len + 1);
//     i = 0;
//     ret_search = 0;
//     ft_strncpy(name, arg,  eq_pos);
//     create_env_arr(&env_vars);
//     while(env_vars[i]!= NULL)
//     {
//         if(strcmp(env_vars[i], name) == 0)
//             ret_search++;
//         i++;
//     }
//     if(ret_search == 0)
//     {
//         env_vars[i] = malloc(sizeof(char) * name_len + 1);
//         ft_strlcpy(env_vars[i], name, name_len);
//     }
//     else
//     {
//         max_vars_num = max_vars;
//         if(max_vars_num != -1)
//             env_vars[max_vars()] = ft_strdup(arg);
//         //if not are there a expected behavious?
//     }


// }


// void create_env_arr()
// {
//     static int ctrl_env = 0;
//     int i;

//     if(ctrl_env == 0) // Corrigido: inicializa só uma vez
//     {
//         *env_vars = malloc(sizeof(char *) * 1025);
//         i = 0;
//         while (i <= 1024)
//         {
//             (*env_vars)[i] = NULL; // sem malloc antes!
//             i++;
//         }
//         ctrl_env = 1;
//     }
// }


void create_env_arr(char ***env_vars)
{
    int i;

    if (!*env_vars)
    {
        *env_vars = malloc(sizeof(char *) * 1025); // aloca array de 1025 ponteiros
        if (!*env_vars)
        {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }

        i = 0;
        while (i < 1025)
        {
            (*env_vars)[i] = NULL; // agora é seguro atribuir
            i++;
        }
    }
}


int max_vars()
{
    static int num_vars = -1;

    num_vars++;
    if(num_vars <= 1024)
        return (num_vars);
    return (-1);
}



void ft_export(char *arg)
{
    char    *eq_pos;
    int name_len;
    char *name;
    int i;
    int ret_search;
    int max_vars_num;

    eq_pos = ft_strchr(arg, '=');
    if (!eq_pos)
    return ; // ignorar variáveis sem '='
name_len = eq_pos - arg;
    name = malloc(sizeof(char) * (name_len + 1));
    ft_strlcpy(name, arg, name_len);
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
        max_vars_num = max_vars();
        if (max_vars_num != -1)
        env_vars[max_vars_num] = ft_strdup(arg);
}
    printf("adicionada: %s\n", env_vars[max_vars_num]);
    free(name);
}

//remove it after tests
void print_export()
{
    int i = 0;
    printf("entrou \n");
    printf("env_vars em print_export: %p\n", (void *)env_vars);
    printf("declare -x %s\n", env_vars[i]);

    while (env_vars && env_vars[i])
    {
        printf("declare -x %s\n", env_vars[i]);
        i++;
    }
}

// void ft_unset(char *arg)
// {
//     int i;

//     i = 0;
//     while(env_vars[i] != NULL)
//     {
//         if(ft_strncmp(arg, env_vars[i], ft_strlen(arg)) == 0)
//         {
//             ft_bzero(env_vars[i]);
//         }
//         i++;
//     }
// }


void ft_unset(char *arg)
{
    char *equal_sign;
    int var_name_len;
    int i = 0;
    int j;

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
                free(env_vars[i]);
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
