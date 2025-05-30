#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include <string.h> 
#include <errno.h>  
#include "minishell.h" 

static int handle_redirections(t_redir *redirs, int saved_fds[2])
{
    t_redir *current = redirs;
    int original_stdin_local = -1;
    int original_stdout_local = -1;
    int fd;

    saved_fds[0] = -1; 
    saved_fds[1] = -1;

    while (current)
    {
        if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_HEREDOC) 
        {
            if (original_stdin_local == -1) { 
                original_stdin_local = dup(STDIN_FILENO);
                if (original_stdin_local == -1) { perror("dup STDIN_FILENO"); return -1; }
                saved_fds[0] = original_stdin_local;
            }
            if (current->type == TOKEN_REDIR_IN) {
                 fd = open(current->file, O_RDONLY);
            } else { 
                fd = open(current->file, O_RDONLY);
            }
            if (fd == -1) { 
                fprintf(stderr, "minishell: %s: %s\n", current->file, strerror(errno));
                if (original_stdout_local != -1) { dup2(original_stdout_local, STDOUT_FILENO); close(original_stdout_local); }
                if (original_stdin_local != -1) { dup2(original_stdin_local, STDIN_FILENO); close(original_stdin_local); }
                return -1; 
            }
            if (dup2(fd, STDIN_FILENO) == -1) { perror("dup2 STDIN_FILENO"); close(fd); return -1; }
            close(fd);
        }
        else if (current->type == TOKEN_REDIR_OUT || current->type == TOKEN_APPEND) 
        {
            if (original_stdout_local == -1) { 
                original_stdout_local = dup(STDOUT_FILENO);
                if (original_stdout_local == -1) { perror("dup STDOUT_FILENO"); return -1; }
                saved_fds[1] = original_stdout_local;
            }
            if (current->type == TOKEN_REDIR_OUT)
                fd = open(current->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            else 
                fd = open(current->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            
            if (fd == -1) { 
                fprintf(stderr, "minishell: %s: %s\n", current->file, strerror(errno));
                if (original_stdout_local != -1) { dup2(original_stdout_local, STDOUT_FILENO); close(original_stdout_local); }
                if (original_stdin_local != -1) { dup2(original_stdin_local, STDIN_FILENO); close(original_stdin_local); }
                return -1; 
            }
            if (dup2(fd, STDOUT_FILENO) == -1) { perror("dup2 STDOUT_FILENO"); close(fd); return -1; }
            close(fd);
        }
        current = current->next;
    }
    return 0; 
}

static void restore_fds(int saved_fds[2])
{
    if (saved_fds[0] != -1) 
    {
        if (dup2(saved_fds[0], STDIN_FILENO) == -1) perror("restore_fds: dup2 stdin");
        close(saved_fds[0]);
    }
    if (saved_fds[1] != -1) 
    {
        if (dup2(saved_fds[1], STDOUT_FILENO) == -1) perror("restore_fds: dup2 stdout");
        close(saved_fds[1]);
    }
}

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
        if (pid_left == -1) { perror("minishell: fork (left)"); close(pipe_fd[0]); close(pipe_fd[1]); return (1); }
        if (pid_left == 0) { 
            close(pipe_fd[0]); 
            if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) { perror("minishell: dup2 stdout to pipe"); exit(1); }
            close(pipe_fd[1]); 
            exit(executor(node->left, new_envp)); 
        }

        pid_right = fork();
        if (pid_right == -1) { perror("minishell: fork (right)"); close(pipe_fd[0]); close(pipe_fd[1]); waitpid(pid_left, NULL, 0); return (1); }
        if (pid_right == 0) { 
            close(pipe_fd[1]); 
            if (dup2(pipe_fd[0], STDIN_FILENO) == -1) { perror("minishell: dup2 stdin from pipe"); exit(1); }
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
            return 0; 
        }
        
        char *command_name = cmd->args[0];

        if (handle_redirections(cmd->redirs, saved_fds) == -1) {
            restore_fds(saved_fds); 
            return 1;
        }

        if (ft_strcmp(command_name, "echo") == 0)
            ft_echo(cmd);
        else if (ft_strcmp(command_name, "cd") == 0)
            status = ft_cd(cmd, new_envp);
        else if (ft_strcmp(command_name, "pwd") == 0)
            ft_pwd(cmd);
        else if (ft_strcmp(command_name, "export") == 0)
            ft_export(cmd, new_envp);
        else if (ft_strcmp(command_name, "unset") == 0)
            ft_unset(cmd, new_envp);
        else if (ft_strcmp(command_name, "env") == 0)
            ft_env(cmd, *new_envp);
        else if (ft_strcmp(command_name, "exit") == 0)
            ft_exit(cmd);
        else if(ft_strcmp(command_name, "."))
        {
            int i = 0;
            while((*new_envp)[i] != NULL && ft_strncmp((*new_envp)[i], "PWD=",4)!= 0)
            { 
                i++;
            }
            char *test = ft_strdup(ft_strchr((*new_envp)[i], '=')+1);
            char *full_path = ft_strjoin(test, "/minishell");
            
            execve(full_path, cmd->args, *new_envp);

            // printf("%s", test);
        }
        else
        {
            pid_t pid = fork();
            if (pid == -1) { 
                perror("minishell: fork (external cmd)"); 
                status = 1;
            }
            else if (pid == 0) { 
                char *cmd_path = get_cmd_path(command_name, *new_envp); 
                if (cmd_path) {
                    execve(cmd_path, cmd->args, *new_envp);
                    fprintf(stderr, "minishell: %s: %s\n", command_name, strerror(errno));
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
    return status;
}