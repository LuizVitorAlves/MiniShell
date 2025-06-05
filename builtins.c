#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h> 
#include <errno.h>
#include "minishell.h"

static char **bublesort( char **new_envp)
{
    int i;
    int j;
    char *swap;
    int cont = 0;

    j = 0;
    while(new_envp[j])
    {
        i = 0;
        while(new_envp[i])
        {
            if (ft_strncmp(new_envp[j], new_envp[i], ft_strlen(new_envp[j])) < 0)
            {
                swap = new_envp[j];
                new_envp[j] =  new_envp[i];
                new_envp[i] =  swap;
            }
            i++;
            cont++;
        }
        j++;
    }   
    return(new_envp);
}

static int is_valid_var_name(const char *name, int len)
{
    int i;
    if (len == 0 || (!ft_isalpha(name[0]) && name[0] != '_'))
        return 0;
    for (i = 1; i < len; i++)
    {
        if (!ft_isalnum(name[i]) && name[i] != '_')
            return 0;
    }
    return 1;
}

static void print_export_env(char **envp)
{
    int i = 0;
    char *eq_pos;
    char **temp;

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
//arrumar esta merda
int ft_exit(t_command *cmd_info)
{
    int exit_code = 0; 
    long code_val;
    int i = 0;

    if (cmd_info->args[1]) 
    {
        if (cmd_info->args[1][0] == '-' || cmd_info->args[1][0] == '+')
            i++;
        while(cmd_info->args[1][i])
        {
            if (!ft_isdigit(cmd_info->args[1][i]))
            {
                fprintf(stderr, "minishell: exit: %s: numeric argument required\n", cmd_info->args[1]);
                exit(255); 
            }
            i++;
        }

        if (cmd_info->args[2]) 
        {
            fprintf(stderr, "minishell: exit: too many arguments\n");
            return(1); 
        }
        
        code_val = ft_atol(cmd_info->args[1]);
        exit_code = (int)(code_val & 0xFF); 
    }
    exit(exit_code);
}

int ft_echo(t_command *cmd_info)
{
    int i = 1;
    int newline = 1;
    int only_n_flags = 0; 

    while (cmd_info->args[i] && ft_strncmp(cmd_info->args[i], "-n", 2) == 0)
    {
        int j = 1;
        int all_n = 1;
        while(cmd_info->args[i][j])
        {
            if (cmd_info->args[i][j] != 'n')
            {
                all_n = 0;
                break;
            }
            j++;
        }
        if (all_n && j > 1) 
        {
            newline = 0;
            only_n_flags = 1;
            i++;
        } else {
            break; 
        }
        
    }
    
    while (cmd_info->args[i])
    {
        printf("%s", cmd_info->args[i]);
        i++;
        if (cmd_info->args[i])
        {
            printf(" ");
        }
    }

    if (newline)
    {
        printf("\n");
    }
    return(0);
}

int ft_pwd(t_command *cmd_info)
{
    char buffer[PATH_MAX];

    (void)cmd_info; 

    if (getcwd(buffer, sizeof(buffer)) != NULL)
    {
        printf("%s\n", buffer);
    }
    else
    {
        perror("minishell: pwd");
    }
    return(0);
}

int ft_cd(t_command *cmd_info, char ***envp)
{
    char *target_path;
    char current_pwd_buffer[PATH_MAX];
    char *old_pwd_val = NULL;

    if (getcwd(current_pwd_buffer, sizeof(current_pwd_buffer)) != NULL)
    {
        old_pwd_val = ft_strdup(current_pwd_buffer);
        if (!old_pwd_val) {
             perror("minishell: cd: strdup error");
             return (1);
        }
    } 

    if (!cmd_info->args[1] || (cmd_info->args[1][0] == '~' && cmd_info->args[1][1] == '\0')) 
    {
        target_path = getenv("HOME");
        if (!target_path)
        {
            fprintf(stderr, "minishell: cd: HOME not set\n");
            free(old_pwd_val);
            return (1);
        }
    }
    else if (ft_strcmp(cmd_info->args[1], "-") == 0)
    {
        target_path = getenv("OLDPWD");
        if (!target_path)
        {
            fprintf(stderr, "minishell: cd: OLDPWD not set\n");
            free(old_pwd_val);
            return (1);
        }
        printf("%s\n", target_path); 
    }
    else
    {
        if (cmd_info->args[2])
        {
            fprintf(stderr, "minishell: cd: too many arguments\n");
            free(old_pwd_val);
            return (1);
        }
        target_path = cmd_info->args[1];
    }

    if (chdir(target_path) != 0)
    {
        fprintf(stderr, "minishell: cd: %s: %s\n", target_path, strerror(errno));
        free(old_pwd_val);
        return (errno);
    }

    if (getcwd(current_pwd_buffer, sizeof(current_pwd_buffer)) != NULL)
    {
        if (set_env_var(envp, "PWD", current_pwd_buffer) != 0) {
             fprintf(stderr, "minishell: cd: error setting PWD\n");
        }
    }
    else {
        perror("minishell: cd: getcwd error after chdir");
    }
    
    if (old_pwd_val) { 
        if (set_env_var(envp, "OLDPWD", old_pwd_val) != 0) {
            fprintf(stderr, "minishell: cd: error setting OLDPWD\n");
        }
        free(old_pwd_val);
    } else { 
         set_env_var(envp, "OLDPWD", ""); 
    }
    return (0);
}


int ft_export(t_command *cmd_info, char ***new_envp)
{
    int i = 1;
    char *arg;
    char *eq_pos;
    char *name;
    char *value;
    int name_len;
    int export_status = 0;

    if (!cmd_info->args[1])
    {
        print_export_env(*new_envp);
        return(0);
    }

    while ((arg = cmd_info->args[i]))
    {
        eq_pos = ft_strchr(arg, '=');
        if (eq_pos) 
        {
            name_len = eq_pos - arg;
            if (name_len == 0) 
            {
                fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", arg);
                export_status = 1;
                i++;
                continue;
            }
            name = ft_strndup(arg, name_len);
            if (!name) 
            {
                 export_status = 1;
                  i++; 
                  continue; 
            }
            value = eq_pos + 1;
            if (!is_valid_var_name(name, name_len))
            {
                fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", arg);
                free(name);
                export_status = 1;
                i++;
                continue;
            }
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
            {
                fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", arg);
                export_status = 1;
            }
        }
        i++;

    }
        get_all_env((*new_envp));
        return(export_status);

}

int ft_env(t_command *cmd_info, char **envp_copy)
{
    int i = 0;

    if (cmd_info->args[1])
    {
        fprintf(stderr, "env: '%s': No such file or directory\n", cmd_info->args[1]);
        return(1);
    }

    while (envp_copy && envp_copy[i])
    {
        if (ft_strchr(envp_copy[i], '=')) 
        {
            printf("%s\n", envp_copy[i]);
        }
        i++;
    }
    return(0);
}

int ft_unset(t_command *cmd_info, char ***new_envp)
{
    int i = 1; 
    char *arg_to_unset;
    int k; 
    int j; 
    char *equal_sign;
    int var_name_len;
    int unset_status = 0;

    while ((arg_to_unset = cmd_info->args[i]))
    {
        if (!is_valid_var_name(arg_to_unset, ft_strlen(arg_to_unset)))
        {
            fprintf(stderr, "minishell: unset: `%s': not a valid identifier\n", arg_to_unset);
            unset_status = 1;
            i++;
            continue;
        }

        k = 0;
        while ((*new_envp)[k])
        {
            equal_sign = ft_strchr((*new_envp)[k], '=');
            var_name_len = (equal_sign) ? (equal_sign - (*new_envp)[k]) : ft_strlen((*new_envp)[k]);
            
            if (ft_strlen(arg_to_unset) == (size_t)var_name_len &&
                ft_strncmp((*new_envp)[k], arg_to_unset, var_name_len) == 0)
            {
                free((*new_envp)[k]); 
                for (j = k; (*new_envp)[j]; j++)
                {
                    (*new_envp)[j] = (*new_envp)[j + 1];
                }
                continue; 
            }
            k++; 
        }
        i++; 
    }
        get_all_env((*new_envp));
        return(unset_status);

}