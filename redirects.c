/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d@student.42.rio <lalves-d>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 03:49:54 by lalves-d          #+#    #+#             */
/*   Updated: 2025/06/05 04:32:21 by lalves-d@st      ###   ########.fr       */
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
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	while (1)
	{
		input = readline("> ");
		if (!input || ft_strncmp(input, my_eof, ft_strlen(my_eof) + 1) == 0)
		{
			free(input);
			break ;
		}
		ft_putendl_fd(input, pipe_fd[1]);
		free(input);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

static void	cleanup_current_redir_error(int original_stdin_local,
		int original_stdout_local)
{
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
}

static int	apply_one_input_redir(t_redir *redir_item, int *o_stdin_local_ptr,
		int *saved_fds_stdin_ptr)
{
	int	current_fd;

	if (*o_stdin_local_ptr == -1)
	{
		*o_stdin_local_ptr = dup(STDIN_FILENO);
		if (*o_stdin_local_ptr == -1)
			return (perror("dup stdin"), -1);
		if (*saved_fds_stdin_ptr == -1)
			*saved_fds_stdin_ptr = *o_stdin_local_ptr;
	}
	if (redir_item->type == TOKEN_REDIR_IN)
		current_fd = open(redir_item->file, O_RDONLY);
	else
		current_fd = my_heredoc(redir_item->file);
	if (current_fd == -1)
		return (fprintf(stderr, "minishell: %s: %s\n", redir_item->file,
				strerror(errno)), -1);
	if (dup2(current_fd, STDIN_FILENO) == -1)
		return (perror("dup2 stdin"), close(current_fd), -1);
	close(current_fd);
	return (0);
}

static int	apply_one_output_redir(t_redir *redir_item, int *o_stdout_local_ptr,
		int *saved_fds_stdout_ptr)
{
	int	current_fd;

	if (*o_stdout_local_ptr == -1)
	{
		*o_stdout_local_ptr = dup(STDOUT_FILENO);
		if (*o_stdout_local_ptr == -1)
			return (perror("dup stdout"), -1);
		if (*saved_fds_stdout_ptr == -1)
			*saved_fds_stdout_ptr = *o_stdout_local_ptr;
	}
	if (redir_item->type == TOKEN_REDIR_OUT)
		current_fd = open(redir_item->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		current_fd = open(redir_item->file, O_WRONLY | O_CREAT | O_APPEND,
				0644);
	if (current_fd == -1)
		return (fprintf(stderr, "minishell: %s: %s\n", redir_item->file,
				strerror(errno)), -1);
	if (dup2(current_fd, STDOUT_FILENO) == -1)
		return (perror("dup2 stdout"), close(current_fd), -1);
	close(current_fd);
	return (0);
}

static void	process_redirection_item(t_redir *item, t_redir_context *ctx)
{
	if (item->type == TOKEN_REDIR_IN
		|| item->type == TOKEN_HEREDOC)
		*(ctx->status_code_ptr) = apply_one_input_redir(item,
				ctx->o_stdin_local_ptr, ctx->saved_fds_stdin_ptr);
	else if (item->type == TOKEN_REDIR_OUT
		|| item->type == TOKEN_APPEND)
		*(ctx->status_code_ptr) = apply_one_output_redir(item,
				ctx->o_stdout_local_ptr, ctx->saved_fds_stdout_ptr);
}

int	handle_redirections(t_redir *redirs, int saved_fds[2])
{
	t_redir			*current;
	int				o_stdin_local;
	int				o_stdout_local;
	int				status_code;
	t_redir_context	ctx;

	current = redirs;
	o_stdin_local = -1;
	o_stdout_local = -1;
	status_code = 0;
	saved_fds[0] = -1;
	saved_fds[1] = -1;
	ctx.status_code_ptr = &status_code;
	ctx.o_stdin_local_ptr = &o_stdin_local;
	ctx.o_stdout_local_ptr = &o_stdout_local;
	ctx.saved_fds_stdin_ptr = &saved_fds[0];
	ctx.saved_fds_stdout_ptr = &saved_fds[1];
	while (current && status_code == 0)
	{
		process_redirection_item(current, &ctx);
		current = current->next;
	}
	if (status_code == -1)
		return (cleanup_current_redir_error(o_stdin_local, o_stdout_local), -1);
	return (0);
}

void	restore_fds(int saved_fds[2])
{
	if (saved_fds[0] != -1)
	{
		dup2(saved_fds[0], STDIN_FILENO);
		close(saved_fds[0]);
		saved_fds[0] = -1;
	}
	if (saved_fds[1] != -1)
	{
		dup2(saved_fds[1], STDOUT_FILENO);
		close(saved_fds[1]);
		saved_fds[1] = -1;
	}
}
