#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "minishell.h"

void handle_pipe_left(t_node *node, char ***new_envp, int *pipe_fd)
{
    pid_t pid_left = fork();
    if (pid_left == -1)
    {
        perror("minishell: fork (left)");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        exit(1);
    }
    if (pid_left == 0)
    {
        signal(SIGINT, SIG_DFL);
        close(pipe_fd[0]);
        if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
        {
            perror("minishell: dup2 stdout to pipe");
            exit(1);
        }
        close(pipe_fd[1]);
        exit(executor(node->left, new_envp));
    }
}

void handle_pipe_right(t_node *node, char ***new_envp, int *pipe_fd)
{
    pid_t pid_right = fork();
    if (pid_right == -1)
    {
        perror("minishell: fork (right)");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        exit(1);
    }
    if (pid_right == 0)
    {
        signal(SIGINT, SIG_DFL);
        close(pipe_fd[1]);
        if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
        {
            perror("minishell: dup2 stdin from pipe");
            exit(1);
        }
        close(pipe_fd[0]);
        exit(executor(node->right, new_envp));
    }
}

int execute_pipe(t_node *node, char ***new_envp)
{
    int pipe_fd[2];
    int status = 0;

    if (pipe(pipe_fd) == -1)
    {
        perror("minishell: pipe");
        return 1;
    }
    handle_pipe_left(node, new_envp, pipe_fd);
    handle_pipe_right(node, new_envp, pipe_fd);
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    wait(NULL);
    waitpid(-1, &status, 0);
    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

int execute_builtin(t_command *cmd, char ***new_envp)
{
    char *command_name = cmd->args[0];

    if (ft_strcmp(command_name, "echo") == 0)
        return ft_echo(cmd);
    else if (ft_strcmp(command_name, "cd") == 0)
        return ft_cd(cmd, new_envp);
    else if (ft_strcmp(command_name, "pwd") == 0)
        return ft_pwd(cmd);
    else if (ft_strcmp(command_name, "export") == 0)
        return ft_export(cmd, new_envp);
    else if (ft_strcmp(command_name, "unset") == 0)
        return ft_unset(cmd, new_envp);
    else if (ft_strcmp(command_name, "env") == 0)
        return ft_env(cmd, *new_envp);
    else if (ft_strcmp(command_name, "exit") == 0)
        return ft_exit(cmd);
    return -1;
}

