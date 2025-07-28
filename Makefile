NAME = MINISHELL

SRC = main.c parse/lexer.c parse/parser_utils.c parse/parser.c parse/tokenize.c parse/remove_quotes.c env/env.c

CC = cc 

LIBFT = libft/libft.a

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(LIBFT)

$(NAME):
	$(CC) $(SRC) $(LIBFT) -lreadline -o minishell

$(LIBFT):
		make bonus -C libft

clean:
	make clean -C libft

fclean: clean
	rm -rf minishell

re: fclean $(NAME)