/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d@student.42.rio <lalves-d>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:17:29 by lalves-d          #+#    #+#             */
/*   Updated: 2025/04/11 16:57:58 by lalves-d@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

    #define MINISHELL_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <readline/history.h>
    # include <readline/readline.h>
    #include "libft/libft.h"
    char    **put_args_array(char *input);

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

	typedef struct s_token
	{
		t_token_type		type;
		char				*value;
		struct s_token		*next;
	}	t_token;

	//Lexer.c
	t_token	*tokenize(const char *line);
	t_token_type	get_token_type(const char *line, int *i);
	char	*get_op_str(t_token_type type);
	char	*get_word(const char *line, int *i);
	char	*get_unquoted_word(const char *line, int *i);
	char	*get_quoted_word(const char *line, int *i, char quote);
	void	add_token(t_token **head, t_token *new_token);
	t_token	*create_token(t_token_type type, const char *value);



#endif