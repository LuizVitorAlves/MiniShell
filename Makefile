NAME = minishell
LIBFT_DIR = libft

SRCS = \
	parser/parser.c \
	executor.c \
	builtins.c \
	main.c \
	lexer/lexer.c \
	parser/free_parser.c \
	parser/parser_utils.c \
	lexer/lexer_builder.c \
	lexer/lexer_parts.c \
	lexer/lexer_utils.c \
	exec_utils.c \
	redirects.c \
	cd.c \
	export.c \
	export_aux.c \
	unset.c \
	executor_aux.c \
	parser/parser_redirect.c \
	lexer/get_word.c \
	enviroments.c \
	exit.c

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -g -Wall -Werror -Wextra
LIBS = -lreadline

# Colors
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
RESET = \033[0m

# ─────────────────────────────────────────────────────────────── #

all: $(NAME)

$(NAME): $(LIBFT_DIR)/libft.a
	@printf "$(YELLOW)Compiling files...$(RESET)\n"
	@OBJS=""; \
	i=0; total=$(words $(SRCS)); \
	for src in $(SRCS); do \
		i=$$((i + 1)); \
		obj=$${src%.c}.o; \
		printf "\r[$(GREEN)%3d%%$(RESET)] Compiling %-30s" $$((i * 100 / total)) $$obj; \
		$(CC) $(CFLAGS) -c $$src -o $$obj || exit 1; \
		OBJS="$$OBJS $$obj"; \
	done; \
	printf "\n"; \
	$(CC) $(CFLAGS) $$OBJS -o $(NAME) $(LIBS) -L$(LIBFT_DIR) -lft
	@printf "$(GREEN)Build complete! 🚀$(RESET)\n"

$(LIBFT_DIR)/libft.a:
	@make -s -C $(LIBFT_DIR)

clean:
	@rm -f $(OBJS)
	@make -s -C $(LIBFT_DIR) clean

fclean: clean
	@printf "$(BLUE)Cleaning files...$(RESET)"
	@for i in 1 2 3; do printf "."; sleep 0.2; done
	@printf "\n"
	@rm -f $(NAME)
	@make -s -C $(LIBFT_DIR) fclean
	@printf "$(GREEN)Clean complete! ✔️$(RESET)\n"

re: fclean all

.PHONY: all clean fclean re
