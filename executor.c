/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d@student.42.rio <lalves-d>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:17:53 by lalves-d          #+#    #+#             */
/*   Updated: 2025/05/15 04:15:37 by lalves-d@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include <unistd.h>
#include"minishell.h"

void ft_cat_builtin(void)
{
    char buffer[1024];
    ssize_t bytes_read;

    while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
    {
        write(STDOUT_FILENO, buffer, bytes_read);
    }
}
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
#include <sys/wait.h>

void executor_echo_with_pipe(t_token *tokens, int pipe_fd[2])
{
    pid_t pid = fork();

    if (pid == 0)
    {
        // No filho: redireciona stdout para o lado de escrita do pipe
        close(pipe_fd[0]); // fecha leitura
        dup2(pipe_fd[1], STDOUT_FILENO); // redireciona stdout
        close(pipe_fd[1]); // fecha após redirecionar

        ft_echo(tokens); // executa o builtin
        exit(0);
    }
    else
    {
        // No pai: fecha lado de escrita do pipe (não vai usar)
        close(pipe_fd[1]);
        waitpid(pid, NULL, 0);
    }
}

// void executor( t_token *tokens, char *path_name, char *input)
// {
//     int pipe_fd[2];
//     pipe(pipe_fd);

//     if(ft_strncmp(tokens->value, "exit", 4) == 0 && (tokens->value[4] == ' ' || tokens->value[4] == '\0'))
//       ft_exit(tokens, input);
//     if(ft_strncmp(tokens->value, "echo", 4) == 0)
//     {
//         if(check_pipes(tokens) == 1)
//             executor_echo_with_pipe(tokens, pipe_fd);
//         else
//             ft_echo(tokens);
//     }
//     if(ft_strncmp(tokens->value, "pwd", 3) == 0)
//         ft_pwd();
//     if(ft_strncmp(tokens->value, "cd", 2) == 0)
//         ft_cd(tokens, path_name);
//     if(ft_strncmp(tokens->value, "export", 6) == 0)
//         ft_export(tokens);
//     if(ft_strncmp(tokens->value, "unset", 5) == 0)
//         ft_unset(tokens);
//     if(ft_strncmp(tokens->value, "env", 3) == 0)
//         ft_env();
//     if(ft_strncmp(tokens->value, "cat", 3) == 0)
//         ft_cat_builtin();
// }



void executor(t_token *tokens, char *path_name, char *input)
{
    int pipe_fd[2];
    int has_pipe = check_pipes(tokens);

    if (has_pipe)
        pipe(pipe_fd);

    if (ft_strncmp(tokens->value, "echo", 4) == 0 && has_pipe)
    {
        pid_t pid1 = fork();
        if (pid1 == 0)
        {
            // Filho 1: echo → escreve no pipe
            close(pipe_fd[0]); // fecha leitura
            dup2(pipe_fd[1], STDOUT_FILENO);
            close(pipe_fd[1]);
            ft_echo(tokens);
            exit(0);
        }

        pid_t pid2 = fork();
        if (pid2 == 0)
        {
            // Filho 2: cat ← lê do pipe
            close(pipe_fd[1]); // fecha escrita
            dup2(pipe_fd[0], STDIN_FILENO);
            close(pipe_fd[0]);
            ft_cat_builtin();
            exit(0);
        }

        // Pai
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
        return;
    }

    // Casos sem pipe
    if (ft_strncmp(tokens->value, "echo", 4) == 0)
        ft_echo(tokens);
    if (ft_strncmp(tokens->value, "pwd", 3) == 0)
        ft_pwd();
    if (ft_strncmp(tokens->value, "cd", 2) == 0)
        ft_cd(tokens, path_name);
    if (ft_strncmp(tokens->value, "export", 6) == 0)
        ft_export(tokens);
    if (ft_strncmp(tokens->value, "unset", 5) == 0)
        ft_unset(tokens);
    if (ft_strncmp(tokens->value, "env", 3) == 0)
        ft_env();
    if (ft_strncmp(tokens->value, "exit", 4) == 0 && (tokens->value[4] == ' ' || tokens->value[4] == '\0'))
        ft_exit(tokens, input);
    if (ft_strncmp(tokens->value, "cat", 3) == 0)
        ft_cat_builtin();
}
