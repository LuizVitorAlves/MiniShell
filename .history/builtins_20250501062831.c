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
    ft_strlcpy(path_name+(11 + ft_strlen(tokens->value)), "$", 1);e

    return 0;
}
void create_env_arr(char **env_vars)
{
    static int ctrl_env = 0;
    if(ctrl_env > 0)
    {
        int i;

        i = 0;
        while(i <= 1024)
        {
            env_vars[i] = malloc(sizeof(*char));
            i++;
        }
        i = 0;
        while(i <= 1024)
        {
            env_vars[i] = NULL;
            i++;
        }
        ctrl_env++;
    }

}
void ft_export(char *arg)
{
    static  char **env_vars;
    char    *eq_pos;
    int name_len;
    char *name;
    int i;
    int ret_search;

    eq_pos = ft_strchr(arg, '=');
    name_len = eq_pos - arg;
    name = malloc(sizeof(char) * name_len + 1);
    i = 0;
    ret_search = 0;
    ft_strncpy(name, arg,  eq_pos);
    create_env_arr(&env_vars);
    while(env_vars[i]!= NULL)
    {
        if(strcmp(env_vars[i], name) == 0)
            ret_search++;
        i++;
    }
    if(ret_search == 0)
    {
        env_vars[i] = malloc(sizeof(char) * name_len + 1);
        ft_strlcpy(env_vars[i], name, name_len);
    }
    else
    {
        env_vars[i] = ft_strdup(arg);
    }


}
