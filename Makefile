NAME = MINISHELL

SRC = execute/execute_path.c parse/parser_utils.c parse/expander.c parse/tokenize.c parse/remove_quotes.c parse/parser.c env/env.c parse/debug.c\
	parse/error.c execute/execute_functions.c main.c parse/lexer.c execute/open_pipe_fork.c free/free.c built_in/built_in.c built_in/built_in_echo.c\
	built_in/built_in_env.c built_in/built_in_exit.c built_in/built_in_pwd.c built_in/built_in_unset.c built_in/built_in_cd.c built_in/built_in_export.c

CC = cc 

LIBFT = libft/libft.a

CFLAGS = -Wall -Wextra -Werror -g

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