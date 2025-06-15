/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:17:29 by lalves-d          #+#    #+#             */
/*   Updated: 2025/06/14 23:09:31 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H
# include "libft/libft.h"
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}					t_token_type;

typedef struct s_builder
{
	char			*str;
	size_t			len;
	size_t			capacity;
}					t_builder;

typedef enum e_state
{
	STATE_GENERAL,
	STATE_IN_DQUOTES,
	STATE_IN_SQUOTES
}					t_state;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}					t_token;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIR
}					t_node_type;

typedef struct s_redir
{
	int				type;
	char			*file;
	struct s_redir	*next;
}					t_redir;

typedef struct s_command
{
	char			**args;
	t_redir			*redirs;
}					t_command;

typedef struct s_node
{
	t_node_type		type;
	struct s_node	*left;
	struct s_node	*right;
	t_command		*command;
}					t_node;

// get_word
char				*get_word(const char *line, int *i);

// lexer_builder
void				builder_init(t_builder *b);
void				builder_destroy(t_builder *b);
void				builder_append_char(t_builder *b, char c);
void				builder_append_str(t_builder *b, char *str);
char				*builder_finalize(t_builder *b);

// lexer_parts
int					exit_status(int status);
void				handle_expansion(const char *line, int *i,
						t_builder *builder);
void				get_unquoted_part(const char *line, int *i,
						t_builder *builder);
int					get_quoted_part(const char *line, int *i,
						t_builder *builder);

// lexer_utils
int					ft_isspace(int c);
t_token				*create_token(t_token_type type, const char *value);
void				add_token(t_token **head, t_token *new_token);
void				free_tokens(t_token *head);

// lexer
t_token				*tokenize(const char *line);

// free_parser
void				free_redirs(t_redir *redir);
void				free_command(t_command *cmd);
void				free_node(t_node *node);
void				free_string_array(char **arr);

// parser_redirect
int					is_input_redir(int type);
int					is_output_redir(int type);
int					handle_redirection(t_command *cmd, t_token **curr);

// parser_utils
int					init_command_node(t_node **node, t_command **cmd,
						char ***args);
int					attach_redirection_or_arg(t_command *cmd, t_token **curr,
						char **args, int *argc);
void				fill_command_args(t_command *cmd, char **args, int argc);
t_node				*create_pipe_node(t_node *left, t_token **tokens);

// parser
t_node				*parse_command(t_token **tokens);
t_node				*parse_pipeline(t_token **tokens);
t_node				*parse_line(t_token **tokens);

// atol
long				ft_atol(const char *str);

// builtins
int					ft_echo(t_command *cmd_info);
int					ft_pwd(t_command *cmd_info);
int					ft_env(t_command *cmd_info, char **envp_copy);

// cd
int					ft_cd(t_command *cmd_info, char ***envp);

// env_utils
char				***get_main_envp_addr(void);
int					set_env_var(char ***env, const char *key,
						const char *value);
char				**dup_env(char **envp);
int					append_env_var(char ***env, char *new_entry, int i);

// enviroments
char				**get_all_env(char **envp);
char				*my_get_env(char *name);

// exec_utils
int					ft_strcmp(const char *s1, const char *s2);
char				*ft_strndup(const char *s, size_t n);

// executor_aux
void				handle_pipe_left(t_node *node, char ***new_envp,
						int *pipe_fd);
void				handle_pipe_right(t_node *node, char ***new_envp,
						int *pipe_fd);
int					execute_pipe(t_node *node, char ***new_envp);
int					execute_builtin(t_command *cmd, char ***new_envp);

// executor
int					executor(t_node *node, char ***new_envp);

// exit
void				exit_aux(t_command *cmd_info, int i);
int					ft_exit(t_command *cmd_info);

// export_aux
char				**bublesort(char **new_envp);
int					is_valid_var_name(const char *name, int len);
void				print_export_env(char **envp);

// export
int					ft_export(t_command *cmd_info, char ***new_envp);

// heredoc
int					aux_perror(void);
int					my_heredoc(char *eof);

// path_utils
char				*get_cmd_path(char *cmd, char **envp);
char				*get_cmd_path_aux(char **paths, char *cmd);

// redirects
int					handle_redirections(t_redir *redir, int saved_fds[2]);
void				restore_fds(int saved_fds[2]);

// shell_control
void				shell_exit(int exit_code);
void				handle_empty_or_invalid_input(char *input, t_token *tokens);
void				execute_valid_command(char *input, t_token *tokens,
						t_node *ast, char ***envp_copy);

// unset
void				unset_aux(char *arg_to_unset, char ***new_envp);
int					ft_unset(t_command *cmd_info, char ***new_envp);

// redirect_aux
int					process_input_redirection(t_redir *redir,
						int *last_input_fd);
int					process_output_redirection(t_redir *redir,
						int *last_output_fd);
int					handle_aux(t_redir *current, int *last_input_fd,
						int *last_output_fd);
#endif