/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:17:53 by lalves-d          #+#    #+#             */
/*   Updated: 2025/04/07 15:17:58 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
static int check_pipes(t_token *tokens)
{   
    int check_pipes;

    check_pipes = 0;
    tokens = tokens->next;
    while(tokens->next)
    {
      if(ft_strncmp(tokens->value, "|",1)== 0)
      {
        check_pipes = 1;
        break;
      }
      tokens = tokens->next;
    }
    return (check_pipes);
}
void executor_echo_with_pipe(t_token tokens, int pipe_fd[2])
{
    pid_t pid = fork();
    if (pid == 0)
    {
        // No filho: redireciona stdout para o lado de escrita do pipe
        close(pipe_fd[0]); // fecha leitura
        dup2(pipe_fd[1], STDOUT_FILENO); // redireciona stdout
        close(pipe_fd[1]); // fecha após redirecionar

        ft_echo(&tokens); // executa o builtin
        exit(0);
    }
    else
    {
        // No pai: fecha lado de escrita do pipe (não vai usar)
        close(pipe_fd[1]);
        waitpid(pid, NULL, 0);
    }
}

void executor( t_token *tokens, char *path_name, char *input)
{
    if(ft_strncmp(tokens->value, "exit", 4) == 0 && (tokens->value[4] == ' ' || tokens->value[4] == '\0'))
      ft_exit(tokens, input);
    if(ft_strncmp(tokens->value, "echo", 4) == 0)
    {
        if(check_pipes(tokens) == 1)
            executor_whith_pipe(tokens);
        else
            ft_echo(tokens);
    }
    if(ft_strncmp(tokens->value, "pwd", 3) == 0)
        ft_pwd();
    if(ft_strncmp(tokens->value, "cd", 2) == 0)
        ft_cd(tokens, path_name);
    if(ft_strncmp(tokens->value, "export", 6) == 0)
        ft_export(tokens);
    if(ft_strncmp(tokens->value, "unset", 5) == 0)
        ft_unset(tokens);
    if(ft_strncmp(tokens->value, "env", 3) == 0)
        ft_env();
}
