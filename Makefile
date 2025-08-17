NAME = minishell

SRC = execute/execute_path.c parse/parser_utils.c parse/expander.c parse/tokenize.c parse/remove_quotes.c parse/parser.c env/env.c\
	parse/error.c execute/execute_functions.c main.c parse/lexer.c execute/open_pipe_fork.c built_in/built_in.c built_in/built_in_echo.c\
	built_in/built_in_env.c built_in/built_in_exit.c built_in/built_in_pwd.c built_in/built_in_unset.c built_in/built_in_cd.c built_in/built_in_export.c\
	redirector/redirector.c redirector/append.c redirector/output.c redirector/input.c redirector/herodoc.c signal/signal.c signal/signal_management.c signal/signal_handlers.c\
	execute/execute_utils.c utils/helper_utils.c utils/helper_for_execute.c parse/heredoc_parser.c env/added_env_node.c parse/debug.c


CC = cc 

LIBFT = libft/libft.a
LIBMEM = libmem/build/libs/libmem.a

CFLAGS = 

all: $(NAME)

$(NAME): $(LIBFT) $(LIBMEM)

$(NAME):
	$(CC) $(SRC) $(LIBFT) $(LIBMEM) -lreadline -o minishell
	

$(LIBFT):
	make bonus -C libft

$(LIBMEM):
	make -C libmem

clean:
	make clean -C libft
	make clean -C libmem

fclean: clean
	make fclean -C libft
	make fclean -C libmem
	rm -rf minishell

re: fclean $(NAME)

