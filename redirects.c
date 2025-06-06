/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 18:07:56 by lalves-d          #+#    #+#             */
/*   Updated: 2025/06/06 14:22:36 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static int	my_heredoc(char *my_eof)
{
	char	*input;
	int		pid_fd[2];

	if (pipe(pid_fd) == -1)
	{
		perror("minishell: pipe");
		return (1);
	}
	input = NULL;
	while (1)
	{
		input = readline("> ");
		if (input == NULL)
			break;

		if (ft_strncmp(input, my_eof, ft_strlen(my_eof) + 1) == 0)
		{
			free(input);
			break ;
		}
		ft_putendl_fd(input, pid_fd[1]);
		free(input);
	}
	close(pid_fd[1]);
	return (pid_fd[0]);
}

int	handle_redirections(t_redir *redirs, int saved_fds[2])
{
	t_redir	*current;
	int		original_stdin_local;
	int		original_stdout_local;
	int		fd;

	current = redirs;
	original_stdin_local = -1;
	original_stdout_local = -1;
	saved_fds[0] = -1;
	saved_fds[1] = -1;
	while (current)
	{
		if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_HEREDOC)
		{
			if (original_stdin_local == -1)
			{
				original_stdin_local = dup(STDIN_FILENO);
				if (original_stdin_local == -1)
				{
					perror("dup STDIN_FILENO");
					return (-1);
				}
				saved_fds[0] = original_stdin_local;
			}
			if (current->type == TOKEN_REDIR_IN)
			{
				fd = open(current->file, O_RDONLY);
			}
			else
			{
				fd = my_heredoc(current->file);
			}
			if (fd == -1)
			{
				fprintf(stderr, "minishell: %s: %s\n", current->file,
					strerror(errno));
				if (original_stdout_local != -1)
				{
					dup2(original_stdout_local, STDOUT_FILENO);
					close(original_stdout_local);
				}
				if (original_stdin_local != -1)
				{
					dup2(original_stdin_local, STDIN_FILENO);
					close(original_stdin_local);
				}
				return (-1);
			}
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				perror("dup2 STDIN_FILENO");
				close(fd);
				return (-1);
			}
			close(fd);
		}
		else if (current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_APPEND)
		{
			if (original_stdout_local == -1)
			{
				original_stdout_local = dup(STDOUT_FILENO);
				if (original_stdout_local == -1)
				{
					perror("dup STDOUT_FILENO");
					return (-1);
				}
				saved_fds[1] = original_stdout_local;
			}
			if (current->type == TOKEN_REDIR_OUT)
				fd = open(current->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else
				fd = open(current->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				fprintf(stderr, "minishell: %s: %s\n", current->file,
					strerror(errno));
				if (original_stdout_local != -1)
				{
					dup2(original_stdout_local, STDOUT_FILENO);
					close(original_stdout_local);
				}
				if (original_stdin_local != -1)
				{
					dup2(original_stdin_local, STDIN_FILENO);
					close(original_stdin_local);
				}
				return (-1);
			}
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("dup2 STDOUT_FILENO");
				close(fd);
				return (-1);
			}
			close(fd);
		}
		current = current->next;
	}
	return (0);
}

void	restore_fds(int saved_fds[2])
{
	if (saved_fds[0] != -1)
	{
		dup2(saved_fds[0], STDIN_FILENO);
		close(saved_fds[0]);
	}
	if (saved_fds[1] != -1)
	{
		dup2(saved_fds[1], STDOUT_FILENO);
		close(saved_fds[1]);
	}
}
