NAME = minishell

SRC = execute/execute_path.c parse/parser_utils.c parse/expander.c parse/tokenize.c parse/remove_quotes.c parse/parser.c env/env.c\
	parse/error.c execute/execute_functions.c main.c parse/lexer.c execute/open_pipe_fork.c built_in/built_in.c built_in/built_in_echo.c\
	built_in/built_in_env.c built_in/built_in_exit.c built_in/built_in_pwd.c built_in/built_in_unset.c built_in/built_in_cd.c built_in/built_in_export.c\
	redirector/redirector.c redirector/append.c redirector/output.c redirector/input.c redirector/herodoc.c signal/signal.c signal/signal_management.c signal/signal_handlers.c\
	execute/execute_utils.c utils/helper_utils.c utils/helper_for_execute.c parse/heredoc_parser.c env/added_env_node.c execute/execute_helper.c execute/execute_heredoc.c\
	execute/execute_wait.c execute/heredoc_fork.c redirector/heredoc_tokenize.c signal/heredoc_signal.c parse/expander_helper.c parse/expander_utils.c memory/memory_utils.c\
	memory/memory.c

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
	make fclean -C libft
	rm -rf minishell

re: fclean $(NAME)

