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
    }

}
void ft_export(void)
{
    static  char **env_vars;
    

}
