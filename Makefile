NAME = minishell
LIBFT_DIR = libft

SRCS = \
    main.c \
    builtins.c \
    cd.c \
    executor.c \
    executor_aux.c \
    exec_utils.c \
    export.c \
    export_aux.c \
    redirects.c \
    signals.c \
    unset.c \
    lexer/environment.c \
    lexer/get_word.c \
    lexer/lexer_builder.c \
    lexer/lexer_parts.c \
    lexer/lexer_utils.c \
    lexer/lexer.c \
    parser/free_parser.c \
    parser/parser_redirections.c \
    parser/parser_utils.c \
    parser/parser.c
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -g -Wall -Werror -Wextra
LIBS = -lreadline

all: $(NAME)

$(NAME): $(LIBFT_DIR)/libft.a $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS) -L$(LIBFT_DIR) -lft

$(LIBFT_DIR)/libft.a:
	make -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
