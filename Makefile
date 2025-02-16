CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS_DIR = srcs
SRCS = $(addprefix $(SRCS_DIR)/, builtins/cd.c \
		builtins/echo.c \
		builtins/env.c \
		builtins/exit.c \
		builtins/export.c \
		builtins/pwd.c \
		builtins/unset.c \
		execution/children.c \
		execution/execution.c \
		garbage_collector/fds.c \
		garbage_collector/free.c \
		garbage_collector/malloc.c \
		parser/cmd_grp_cmds.c \
		parser/parser_utils.c \
		parser/cmd_grp_fds.c \
		parser/cmd_path.c \
		parser/heredoc.c \
		parser/parser.c \
		signals/signal_handlers.c \
		tokenizer/env_var/env.c \
		tokenizer/env_var/env_var_utils.c \
		tokenizer/add_to_tokenizer.c \
		tokenizer/get_next_token.c \
		tokenizer/replace_environment_variables.c \
		tokenizer/tokenize_operator.c \
		tokenizer/tokenize_quote.c \
		tokenizer/tokenize_space.c \
		tokenizer/tokenize_word.c \
		tokenizer/tokenizer.c \
		tokenizer/token_utils.c \
		utils/vector.c \
		utils/error_utils.c \
		utils/ft_readline.c \
		utils/safe_libft.c \
		utils/utils.c \
		main.c \
		)

OBJS_DIR = objs
OBJS = $(patsubst $(SRCS_DIR)/%.c,$(OBJS_DIR)/%.o,$(SRCS))

HEADERS = -I ./includes -I $(LIBFT_DIR)/includes

LIBS = $(LIBFT) -lreadline

LIBFT_DIR = lib/libft
LIBFT = $(LIBFT_DIR)/libft.a

NAME = minishell

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c includes/minishell.h
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
