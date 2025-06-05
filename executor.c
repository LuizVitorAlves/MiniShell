#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include <string.h> 
#include <errno.h>  
#include "minishell.h" 

int executor(t_node *node, char ***new_envp)
{
    int status = 0; 
    int saved_fds[2] = {-1, -1}; 

    if (!node)
        return 0; 

    if (node->type == NODE_PIPE)
    {
        int pipe_fd[2];
        pid_t pid_left, pid_right;

        if (pipe(pipe_fd) == -1) {
            perror("minishell: pipe");
            return (1); 
        }

        pid_left = fork();
        if (pid_left == -1) 
        { 
            perror("minishell: fork (left)");
            close(pipe_fd[0]); 
            close(pipe_fd[1]); 
            return (1); 
        }
        if (pid_left == 0) { 
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

        pid_right = fork();
        if (pid_right == -1) 
        {
             perror("minishell: fork (right)");
             close(pipe_fd[0]); 
             close(pipe_fd[1]); 
             waitpid(pid_left, NULL, 0); 
             return (1); 
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

        close(pipe_fd[0]);
        close(pipe_fd[1]);
        
        waitpid(pid_left, NULL, 0); 
        waitpid(pid_right, &status, 0); 
        
        if (WIFEXITED(status)) {
            status = WEXITSTATUS(status);
        } else {
            status = 1; 
        }

    }
    else if (node->type == NODE_COMMAND)
    {
        t_command *cmd = node->command;
        if (!cmd || !cmd->args || !cmd->args[0]) {
            if (handle_redirections(cmd->redirs, saved_fds) == -1) {
                restore_fds(saved_fds); 
                return 1;
            }
            restore_fds(saved_fds);
            return 0;
        }
    
        
        char *command_name = cmd->args[0];

        if (handle_redirections(cmd->redirs, saved_fds) == -1) {
            restore_fds(saved_fds); 
            return 1;
        }

        if (ft_strcmp(command_name, "echo") == 0)
            status =ft_echo(cmd);
        else if (ft_strcmp(command_name, "cd") == 0)
            status = ft_cd(cmd, new_envp);
        else if (ft_strcmp(command_name, "pwd") == 0)
            status = ft_pwd(cmd);
        else if (ft_strcmp(command_name, "export") == 0)
           status =  ft_export(cmd, new_envp);
        else if (ft_strcmp(command_name, "unset") == 0)
           status =  ft_unset(cmd, new_envp);
        else if (ft_strcmp(command_name, "env") == 0)
           status =  ft_env(cmd, *new_envp);
        else if (ft_strcmp(command_name, "exit") == 0)
           status =  ft_exit(cmd);
        else
        {
            pid_t pid = fork();
            if (pid == -1) { 
                perror("minishell: fork (external cmd)"); 
                status = 1;
            }
            else if (pid == 0) {
                signal(SIGINT, SIG_DFL);
                signal(SIGQUIT, SIG_DFL);
                char *cmd_path = get_cmd_path(command_name, *new_envp); 
                if (cmd_path) {
                    execve(cmd_path, cmd->args, *new_envp);
                    fprintf(stderr, "minishell: %s: %s\n", command_name, strerror(errno)); //ver se o badfile vem daqui
                    free(cmd_path);
                    exit(126); 
                } else {
                    fprintf(stderr, "minishell: %s: command not found\n", command_name);
                    exit(127); 
                }
            } else { 
                waitpid(pid, &status, 0);
                if (WIFEXITED(status)) {
                    status = WEXITSTATUS(status);
                } else if (WIFSIGNALED(status)) {
                    status = 128 + WTERMSIG(status); 
                } else {
                    status = 1; 
                }
            }
        }
        
        restore_fds(saved_fds);
    }
    exit_status(status);
    return status;
}
