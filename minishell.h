/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d@student.42.rio <lalves-d>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:17:29 by lalves-d          #+#    #+#             */
/*   Updated: 2025/05/30 01:00:40 by lalves-d@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

    #define MINISHELL_H
    #include <stdio.h>
    #include <stdlib.h>
	#include <unistd.h>
    #include <readline/history.h>
    # include <readline/readline.h>
	#include <signal.h>
    #include "libft/libft.h"
	#include <limits.h> //tirar
	typedef enum e_token_type
	{
		TOKEN_WORD,
		TOKEN_PIPE,
		TOKEN_REDIR_IN,
		TOKEN_REDIR_OUT,
		TOKEN_APPEND,
		TOKEN_HEREDOC,
		TOKEN_EOF
	}	t_token_type;

	typedef struct s_builder
	{
		char	*str;
		size_t	len;
		size_t	capacity;
	}	t_builder;

	typedef enum e_state
	{
    	STATE_GENERAL,
    	STATE_IN_DQUOTES,
    	STATE_IN_SQUOTES
	}   t_state;

	typedef struct s_token
	{
		t_token_type		type;
		char				*value;
		struct s_token		*next;
	}	t_token;

	typedef enum e_node_type
    {   
        NODE_COMMAND,
        NODE_PIPE,
        NODE_REDIR
    }   t_node_type;

    typedef struct s_redir
    {   
        int             type;
        char            *file;
        struct s_redir  *next;
    }   t_redir;

    typedef struct s_command
    {   
        char    **args;
        t_redir *redirs;
    }   t_command;

    typedef struct s_node
    {   
        t_node_type     type;
        struct s_node   *left;
        struct s_node   *right;
        t_command       *command;
    }   t_node;

	//exec_utils.c
	int ft_strcmp(const char *s1, const char *s2);
	long ft_atol(const char *str);
	char *ft_strndup(const char *s, size_t n);
	
	//Lexer.c
	/*t_token	*tokenize(const char *line);
	t_token_type	get_token_type(const char *line, int *i);
	char	*get_op_str(t_token_type type);
	char	*get_word(const char *line, int *i);
	char	*get_unquoted_word(const char *line, int *i);
	char	*get_quoted_word(const char *line, int *i, char quote);
	void	add_token(t_token **head, t_token *new_token);
	t_token	*create_token(t_token_type type, const char *value);*/
	// void	ft_echo(t_token *tokens, char **newenvp);
	void ft_echo(t_command *cmd_info);
	int ft_cd(t_command *cmd_info, char ***envp);

	void	print_tokens(t_token *tokens);
	// void ft_exit(t_token *tokens, char *input);
	void ft_exit(t_command *cmd_info);

	void ft_pwd();
	char *get_cmd_path(char  *tokens, char **new_envp);
	//int ft_cd(t_token *tokens, char *path_name);
	// int ft_cd(t_token *tokens, char *path_name, char ***envp);

	// void ft_export(t_token *token, char ***new_envp);
	void ft_export(t_command *cmd_info, char ***new_envp);

	// void ft_unset(t_token *token, char ***new_envp);
	void ft_unset(t_command *cmd_info, char ***new_envp);

	void create_env_arr(char ***env_vars);
	// void ft_env(char **new_envp);//remove after test
	void ft_env(t_command *cmd_info, char **envp_copy);

	char    **put_args_array(char *input);
	extern char **env_vars;
	int set_env_var(char ***env, const char *key, const char *value);
	// int executor(t_token *tokens, char *path_name, char *input, char ***new_envp);
	int executor(t_node *node, char ***new_envp);

	//Lexer.c
	t_token	*tokenize(const char *line);

	//Lexer_builder.c
	void	builder_init(t_builder *b);
	void	builder_destroy(t_builder *b);
	void	builder_append_char(t_builder *b, char c);
	void	builder_append_str(t_builder *b, char *str);
	char	*builder_finalize(t_builder *b);

	//Lexer_utils.c
	int	ft_isspace(int c);
	t_token	*create_token(t_token_type type, const char *value);
	void	add_token(t_token **head, t_token *new_token);
	void	free_tokens(t_token *head);

	//Lexer_parts.c
	void	handle_expansion(const char *line, int *i, t_builder *builder);
	void	get_unquoted_part(const char *line, int *i, t_builder *builder);
	int	get_quoted_part(const char *line, int *i, t_builder *builder);

	//Parser.c
	t_node  *parse_command(t_token **tokens);
	t_node *parse_pipeline(t_token **tokens);
	t_node *parse_line(t_token **tokens);

	//Free_Parser.c
	void free_redirs(t_redir *redir);
	void free_command(t_command *cmd);
	void free_node(t_node *node);
	void	free_string_array(char **arr);

	//Parser_utils.c
	int	handle_redirection(t_command *cmd, t_token **curr);
	int	init_command_node(t_node **node, t_command **cmd, char ***args);
	int	attach_redirection_or_arg(t_command *cmd, t_token **curr,
			char **args, int *argc);
	void	fill_command_args(t_command *cmd, char **args, int argc);
	t_node	*create_pipe_node(t_node *left, t_token **tokens);
	
#endif