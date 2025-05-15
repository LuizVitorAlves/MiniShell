/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d@student.42.rio <lalves-d>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:17:43 by lalves-d          #+#    #+#             */
/*   Updated: 2025/05/15 03:53:22 by lalves-d@st      ###   ########.fr       */
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

void free_redirs(t_redir *redir)
{
	t_redir *tmp;
	while (redir)
	{
		tmp = redir->next;
		free(redir->file);
		free(redir);
		redir = tmp;
	}
}

void free_command(t_command *cmd)
{
	int i = 0;
	if (!cmd)
		return;
	while (cmd->args && cmd->args[i])
		free(cmd->args[i++]);
	free(cmd->args);
	free_redirs(cmd->redirs);
	free(cmd);
}

void free_node(t_node *node)
{
	if (!node)
		return;
	if (node->type == NODE_COMMAND)
		free_command(node->command);
	else
	{
		free_node(node->left);
		free_node(node->right);
	}
	free(node);
}

int add_redir(t_command *command, t_token *redir_token)
{
	if (!redir_token || !redir_token->next || redir_token->next->type != TOKEN_WORD)
		return (0);
	t_redir *new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (0);
	new_redir->type = redir_token->type;
	new_redir->file = ft_strdup(redir_token->next->value);
	new_redir->next = NULL;
	if (!command->redirs)
		command->redirs = new_redir;
	else
	{
		t_redir *tmp = command->redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_redir;
	}
	return (1);
}

int handle_redir(t_command *cmd, t_token **curr)
{
	if (!(*curr)->next || (*curr)->next->type != TOKEN_WORD)
		return (0);
	t_redir *redir = malloc(sizeof(t_redir));
	if (!redir)
		return (0);
	redir->type = (*curr)->type;
	redir->file = ft_strdup((*curr)->next->value);
	redir->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = redir;
	else
	{
		t_redir *tmp = cmd->redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = redir;
	}
	*curr = (*curr)->next;
	return (1);
}

t_node *parse_command(t_token **tokens)
{
	t_node *node = malloc(sizeof(t_node));
	t_command *cmd = malloc(sizeof(t_command));
	char **args = malloc(sizeof(char *) * 1024);
	int argc = 0;
	t_token *curr = *tokens;

	if (!node || !cmd || !args)
		return (free(args), free(cmd), free(node), NULL);
	node->type = NODE_COMMAND;
	node->left = NULL;
	node->right = NULL;
	cmd->redirs = NULL;
	while (curr && curr->type != TOKEN_PIPE)
	{
		if (curr->type == TOKEN_WORD)
			args[argc++] = ft_strdup(curr->value);
		else if (!handle_redir(cmd, &curr))
			return (free(args), free_command(cmd), free(node), NULL);
		curr = curr->next;
	}
	args[argc] = NULL;
	cmd->args = args;
	node->command = cmd;
	*tokens = curr;
	return (node);
}

t_node *parse_pipeline(t_token **tokens)
{
	t_node *left = parse_command(tokens);
	if (!left)
		return (NULL);

	if (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		t_node *pipe_node = malloc(sizeof(t_node));
		if (!pipe_node)
			return (free_node(left), NULL);
		pipe_node->type = NODE_PIPE;
		pipe_node->left = left;
		pipe_node->right = parse_pipeline(tokens);
		if (!pipe_node->right)
			return (free_node(pipe_node), NULL);
		return pipe_node;
	}
	return left;
}

t_node *parse_line(t_token **tokens)
{
	if (!tokens || !*tokens)
		return (NULL);
	return parse_pipeline(tokens);
}
