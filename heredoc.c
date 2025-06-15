/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 21:59:36 by uviana-b          #+#    #+#             */
/*   Updated: 2025/06/14 22:02:09 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	aux_perror(void)
{
	perror("minishell: pipe");
	return (-1);
}

int	my_heredoc(char *eof)
{
	char	*input;
	int		pid_fd[2];

	if (pipe(pid_fd) == -1)
		return (aux_perror());
	while (1)
	{
		input = readline("> ");
		if (!input)
		{
			fprintf(stderr,
				"minishell: warning: missing end delimiter for '%s'\n", eof);
			break ;
		}
		if (ft_strncmp(input, eof, ft_strlen(eof) + 1) == 0)
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
