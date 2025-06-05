NAME = minishell
LIBFT_DIR = libft

SRCS = \
    parser/parser.c \
    executor.c \
    builtins.c \
    signals.c \
    main.c \
    lexer/lexer.c \
    parser/free_parser.c \
    parser/parser_utils.c \
    lexer/lexer_builder.c \
    lexer/lexer_parts.c \
    lexer/lexer_utils.c \
    exec_utils.c \
    redirects.c \
    lexer/environment.c \
    lexer/get_word.c \
    parser/parser_redirections.c
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
