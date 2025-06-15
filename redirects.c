/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 18:07:56 by lalves-d          #+#    #+#             */
/*   Updated: 2025/06/14 23:06:05 by uviana-b         ###   ########.fr       */
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

int	process_input_redirection(t_redir *redir, int *last_input_fd)
{
	if (*last_input_fd != -1)
		close(*last_input_fd);
	if (redir->type == TOKEN_HEREDOC)
		*last_input_fd = my_heredoc(redir->file);
	else
		*last_input_fd = open(redir->file, O_RDONLY);
	if (*last_input_fd == -1)
	{
		fprintf(stderr, "minishell: %s: %s\n", redir->file, strerror(errno));
		return (-1);
	}
	return (0);
}

int	process_output_redirection(t_redir *redir, int *last_output_fd)
{
	int	flags;

	if (*last_output_fd != -1)
		close(*last_output_fd);
	if (redir->type == TOKEN_REDIR_OUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	*last_output_fd = open(redir->file, flags, 0644);
	if (*last_output_fd == -1)
	{
		fprintf(stderr, "minishell: %s: %s\n", redir->file, strerror(errno));
		return (-1);
	}
	return (0);
}

int	handle_redirections(t_redir *redir, int saved_fds[2])
{
	t_redir	*current;
	int		last_input_fd;
	int		last_output_fd;

	current = redir;
	last_input_fd = -1;
	last_output_fd = -1;
	saved_fds[0] = -1;
	saved_fds[1] = -1;
	if (handle_aux(current, &last_input_fd, &last_output_fd) == -1)
		return (-1);
	if (last_input_fd != -1)
	{
		saved_fds[0] = dup(STDIN_FILENO);
		dup2(last_input_fd, STDIN_FILENO);
		close(last_input_fd);
	}
	if (last_output_fd != -1)
	{
		saved_fds[1] = dup(STDOUT_FILENO);
		dup2(last_output_fd, STDOUT_FILENO);
		close(last_output_fd);
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
