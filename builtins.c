/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d@student.42.rio <lalves-d>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:18:18 by lalves-d          #+#    #+#             */
/*   Updated: 2025/05/08 14:36:33 by lalves-d@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_exit(t_token *tokens, char *input)
{
    const t_token *token = tokens;

    while(token)
    {
        tokens = tokens->next;
        free(token->value);
        free((t_token*)token);
        token = tokens;
    }
    free(input);
    exit(0);
}
void   ft_echo(t_token *tokens)
{
    int check;
    int i;
   
    i = 0;
    check = 0;
    tokens = tokens->next;
    if(ft_strncmp(tokens->value, "-n",2) == 0)
    {
        check =1;
        tokens = tokens->next;
    }
    while(tokens->next)
    {
        if(ft_strncmp(tokens->value, "$", 1) == 0)
        {
    
            while(env_vars[i])
            {
                ft_strncmp(env_vars[i], tokens->value+1, ft_strlen(tokens->value) == 0);
                {
                 

                        printf("%s", ft_strchr(env_vars[i], '=')+1);
                }
                i++;
            }
        }
        else
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
// int ft_cd(t_token *tokens, char *path_name)
// {
//     tokens = tokens->next;
//     if (!tokens->value) {
//         printf("cd: missing argument\n");
//         return 1;
//     }

//     if (chdir(tokens->value) != 0) {
//         printf("cd: %s error \n", tokens->value);
//         return 1;
//     }
//     ft_strlcpy(path_name+11, tokens->value, (ft_strlen(tokens->value)));
//     ft_strlcpy(path_name+(11 + ft_strlen(tokens->value)), "$", 1);

//     return 0;
// }


// int ft_cd(t_token *tokens, char *path_name, char ***envp)
// {
//     char cwd[PATH_MAX];
//     char *old_pwd;
//     tokens = tokens->next;
//     (void)path_name;

//     if (!tokens || !tokens->value) {
//         printf("cd: missing argument\n");
//         return 1;
//     }

//     // Pega o diretório atual antes de mudar
//     if (!getcwd(cwd, sizeof(cwd))) {
//         perror("getcwd");
//         return 1;
//     }
//     old_pwd = strdup(cwd);
//     if (!old_pwd)
//         return 1;

//     // Tenta mudar de diretório
//     if (chdir(tokens->value) != 0) {
//         printf("cd: %s: No such file or directory\n", tokens->value);
//         free(old_pwd);
//         return 1;
//     }

//     // Pega novo diretório após mudança
//     if (!getcwd(cwd, sizeof(cwd))) {
//         perror("getcwd");
//         free(old_pwd);
//         return 1;
//     }

//     // Atualiza OLD_PWD e PWD no ambiente
//     set_env_var(envp, "OLDPWD", old_pwd);
//     set_env_var(envp, "PWD", cwd);

//     free(old_pwd);
//     return 0;
// }



int ft_cd(t_token *tokens, char *path_name, char ***envp)
{
    char cwd[PATH_MAX];
    char *old_pwd;

    (void)path_name;
    if (!tokens) {
        printf("cd: no tokens passed\n");
        return 1;
    }

    tokens = tokens->next;
    if (!tokens || !tokens->value) {
        printf("cd: missing argument\n");
        return 1;
    }

    if (!getcwd(cwd, sizeof(cwd))) {
        perror("getcwd error before chdir");
        return 1;
    }

    old_pwd = strdup(cwd);
    if (!old_pwd) {
        perror("strdup failed");
        return 1;
    }

    if (chdir(tokens->value) != 0) {
        printf("cd: %s: No such file or directory\n", tokens->value);
        free(old_pwd);
        return 1;
    }

    if (!getcwd(cwd, sizeof(cwd))) {
        perror("getcwd error after chdir");
        free(old_pwd);
        return 1;
    }

    if (set_env_var(envp, "OLDPWD", old_pwd)) {
        fprintf(stderr, "Error setting OLDPWD\n");
        free(old_pwd);
        return 1;
    }
    if (set_env_var(envp, "PWD", cwd)) {
        fprintf(stderr, "Error setting PWD\n");
        free(old_pwd);
        return 1;
    }

    free(old_pwd);
    return 0;
}


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

// void ft_export(t_token *token, char ***new_envp)
// {
//     char    *eq_pos;
//     int name_len;
//     char *name;
//     int i;
//     char *arg = token->next->value;

//     eq_pos = ft_strchr(arg, '=');
//     if (!eq_pos)
//     return ;
//     name_len = eq_pos - arg;
//     name = malloc(sizeof(char) * (name_len + 1));
//     ft_strlcpy(name, arg, name_len);
//     name[name_len] = '\0';
//     i = 0;
//     while ((* new_envp)[i] != NULL)
//     {
//         if (ft_strncmp((* new_envp)[i], name, name_len) == 0 && (* new_envp)[i][name_len] == '=')
//         {
//             free((* new_envp)[i]);
//             (* new_envp)[i] = ft_strdup(arg);
//             break;
//         }
//         i++;
//     }
//     free(name);
// }


void ft_export(t_token *token, char ***new_envp)
{
    char *eq_pos;
    int name_len;
    char *name;
    int i;
    int found = 0;
    char *arg;

    if (!token || !token->next || !token->next->value)
        return;

    arg = token->next->value;
    eq_pos = ft_strchr(arg, '=');
    if (!eq_pos)
        return;

    name_len = eq_pos - arg;
    name = malloc(sizeof(char) * (name_len + 1));
    if (!name)
        return;

    ft_strlcpy(name, arg, name_len + 1); // inclui o '\0'
    i = 0;
    while ((*new_envp)[i] != NULL)
    {
        if (ft_strncmp((*new_envp)[i], name, name_len) == 0 &&
            (*new_envp)[i][name_len] == '=')
        {
            free((*new_envp)[i]);
            (*new_envp)[i] = ft_strdup(arg);
            found = 1;
            break;
        }
        i++;
    }

    if (!found)
    {
        char **new_env = malloc(sizeof(char *) * (i + 2));
        if (!new_env)
        {
            free(name);
            return;
        }

        for (int j = 0; j < i; j++)
            new_env[j] = (*new_envp)[j];

        new_env[i] = ft_strdup(arg);
        new_env[i + 1] = NULL;

        free(*new_envp);
        *new_envp = new_env;
    }

    free(name);
}



//remove it after tests
void ft_env(char **envp_copy)
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
    i = 0;
    while(envp_copy && envp_copy[i])
    {
        printf("%s \n", envp_copy[i]);
        i++;
    }
}

void ft_unset(t_token *token, char ***new_envp)
{
    char *equal_sign;
    int var_name_len;
    int i = 0;
    int j;
    char *arg=token->next->value;

    if (!arg)
        return;
    while ((* new_envp)[i])
    {
        equal_sign = ft_strchr((*new_envp)[i], '=');
        if (equal_sign)
        {
            var_name_len = equal_sign - (* new_envp)[i];
            if (ft_strlen(arg) == (size_t)var_name_len &&
                ft_strncmp((*new_envp)[i], arg, var_name_len) == 0)
            {
                max_vars(-1);
                ft_bzero((* new_envp)[i], ft_strlen((* new_envp)[i]));
                j = i;
                while ((* new_envp)[j])
                {
                    (*new_envp)[j] = (* new_envp)[j + 1];
                    j++;
                }
                continue;
            }
        }
        i++;
    }
}
