/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_aux.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 21:38:09 by uviana-b          #+#    #+#             */
/*   Updated: 2025/06/14 23:08:59 by uviana-b         ###   ########.fr       */
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

int	handle_aux(t_redir *current, int *last_input_fd, int *last_output_fd)
{
	while (current)
	{
		if (is_input_redir(current->type))
		{
			if (process_input_redirection(current, last_input_fd) == -1)
				return (-1);
		}
		else if (is_output_redir(current->type))
		{
			if (process_output_redirection(current, last_output_fd) == -1)
			{
				if (*last_input_fd != -1)
					close(*last_input_fd);
				return (-1);
			}
		}
		current = current->next;
	}
	return (0);
}
