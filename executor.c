/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d@student.42.rio <lalves-d>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:44:36 by lalves-d          #+#    #+#             */
/*   Updated: 2025/06/06 15:25:47 by lalves-d@st      ###   ########.fr       */
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

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
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

int	executor(t_node *node, char ***new_envp)
{
	int			status;
	int			saved_fds[2];
	t_command	*cmd;

	saved_fds[0] = -1;
	saved_fds[1] = -1;
	status = 0;
	if (!node)
		return (0);
	if (node->type == NODE_PIPE)
		status = execute_pipe(node, new_envp);
	else if (node->type == NODE_COMMAND)
	{
		cmd = node->command;
		//printf("%s \n", cmd->args[0]);
		//if (!cmd || !cmd->args || !cmd->args[0])
		//	return (handle_redirections(cmd->redirs, saved_fds) == -1 ? 1 : 0);
		if (handle_redirections(cmd->redirs, saved_fds) == -1)
			return (1);
		if(cmd->args[0] == NULL)
		{
			restore_fds(saved_fds);
			return (status);
		}
		status = execute_builtin(cmd, new_envp);
		if (status == -1)
			status = execute_external(cmd, new_envp);
		restore_fds(saved_fds);
	}
	exit_status(status);
	return (status);
		// =============================================================
}
