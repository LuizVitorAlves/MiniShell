/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d@student.42.rio <lalves-d>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:17:43 by lalves-d          #+#    #+#             */
/*   Updated: 2025/05/16 15:50:04 by lalves-d@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*char    **put_args_array(char *input)
{
    char    **args;
   
    printf("%s \n", input);

    args = ft_split(input, ' ');
    return(args);
}*/

t_node	*parse_command(t_token **tokens)
{
	t_node		*node;
	t_command	*cmd;
	char		**args;
	int			argc;
	t_token		*curr;

	if (!init_command_node(&node, &cmd, &args))
		return (free(args), free(cmd), free(node), NULL);
	curr = *tokens;
	argc = 0;
	while (curr && curr->type != TOKEN_PIPE)
	{
		if (!attach_redirection_or_arg(cmd, &curr, args, &argc))
			return (free(args), free_command(cmd), free(node), NULL);
		curr = curr->next;
	}
	fill_command_args(cmd, args, argc);
	node->command = cmd;
	*tokens = curr;
	return (node);
}

t_node	*parse_pipeline(t_token **tokens)
{
	t_node	*left;

	left = parse_command(tokens);
	if (!left)
		return (NULL);
	if (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		return (create_pipe_node(left, tokens));
	}
	return (left);
}

t_node	*parse_line(t_token **tokens)
{
	t_node	*node;

	if (!tokens || !*tokens)
		return (NULL);
	node = NULL;
	node = parse_pipeline(tokens);
	if (!node)
	{
		ft_putstr_fd("syntax error\n", STDERR_FILENO);
		return (NULL);
	}
	return (node);
}
