NAME = libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
       ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
       ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
       ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
       ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c \
       ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
       ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c

OBJS = $(SRCS:.c=.o)

BONUS_SRCS = ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c \
			 ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c

BONUS_OBJS = $(BONUS_SRCS:.c=.o)

FT_PRINTF_SRCS = $(addprefix ft_printf/, ft_printf.c handle_specifiers1.c handle_specifiers2.c \
				 utils_ptr.c utils_integer.c utils_hex.c)

FT_PRINTF_OBJS = $(FT_PRINTF_SRCS:.c=.o)

GNL_SRCS = $(addprefix get_next_line/, get_next_line.c)

GNL_OBJS = $(GNL_SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(BONUS_OBJS) $(FT_PRINTF_OBJS) $(GNL_OBJS)
	ar -rcs $(NAME) $(OBJS) $(BONUS_OBJS) $(FT_PRINTF_OBJS) $(GNL_OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $< -I ./includes -o $(<:.c=.o)

clean:
	rm -f $(OBJS) $(BONUS_OBJS) $(FT_PRINTF_OBJS) $(GNL_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
