/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:44:36 by lalves-d          #+#    #+#             */
/*   Updated: 2025/06/14 20:21:33 by uviana-b         ###   ########.fr       */
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

static void	child_external_exec(t_command *cmd, char ***new_envp)
{
	char	*cmd_path;
	int		saved_fds[2];

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	saved_fds[0] = -1;
	saved_fds[1] = -1;
	if (handle_redirections(cmd->redirs, saved_fds) == -1)
		exit(1);
	cmd_path = get_cmd_path(cmd->args[0], *new_envp);
	if (cmd_path)
	{
		execve(cmd_path, cmd->args, *new_envp);
		fprintf(stderr, "minishell: %s: %s\n", cmd->args[0], strerror(errno));
		free(cmd_path);
		exit(126);
	}
	fprintf(stderr, "minishell: %s: command not found\n", cmd->args[0]);
	exit(127);
}

static int	execute_external(t_command *cmd, char ***new_envp)
{
	int		status;
	pid_t	pid;

	status = 0;
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork (external cmd)");
		return (1);
	}
	if (pid == 0)
		child_external_exec(cmd, new_envp);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static int	executor_aux(t_node *node, int *saved_fds,
	char ***new_envp, int *status)
{
	t_command	*cmd;

	cmd = node->command;
	if (!cmd || !cmd->args || !cmd->args[0])
	{
		if (handle_redirections(cmd->redirs, saved_fds) == -1)
			return (1);
		restore_fds(saved_fds);
		return (0);
	}
	*status = execute_builtin(cmd, new_envp);
	if (*status == -1)
		*status = execute_external(cmd, new_envp);
	return (0);
}

int	executor(t_node *node, char ***new_envp)
{
	int			status;
	int			saved_fds[2];

	saved_fds[0] = -1;
	saved_fds[1] = -1;
	status = 0;
	if (!node)
		return (0);
	if (node->type == NODE_PIPE)
		status = execute_pipe(node, new_envp);
	status = executor_aux(node, saved_fds, new_envp, &status);
	exit_status(status);
	return (status);
}
