NAME = minishell
LIBFT_DIR = libft

SRCS = parser.c executor.c builtins.c signals.c main.c lexer.c
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror
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
