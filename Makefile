CC = cc
# CFLAGS = -g# -fsanitize=address
CFLAGS = -Wall -Wextra -Werror

SRCS_DIR = srcs
SRCS = $(addprefix $(SRCS_DIR)/, main.c \
		garbage_collector/malloc.c garbage_collector/free.c \
		garbage_collector/fds.c \
		parser/parser.c parser/cmd_grp_fds.c parser/cmd_grp_cmds.c \
		parser/cmd_path.c\
		utils/utils.c \
		utils/ft_readline.c \
		) # to be updated

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
