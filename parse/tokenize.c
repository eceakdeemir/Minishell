#include "../libraries/minishell.h"

void	tokenize_heredoc_and_append(t_lexer **head, char *line, int *i)
{
	if (line[(*i)] == '<' && line[(*i) + 1] == '<')
	{
		add_new_node_to_list(create_new_lexer_node("<<", TOKEN_HEREDOC), head);
		(*i) += 2;
	}
	else if (line[(*i)] == '>' && line[(*i) + 1] == '>')
	{
		add_new_node_to_list(create_new_lexer_node(">>", TOKEN_APPEND), head);
		(*i) += 2;
	}
}

void	tokenize_input_output_and_pipe(t_lexer **head, char *line, int *i)
{
	if (line[(*i)] == '|')
	{
		add_new_node_to_list(create_new_lexer_node("|", TOKEN_PIPE), head);
		(*i) += 1;
	}
	else if (line[(*i)] == '>')
	{
		add_new_node_to_list(create_new_lexer_node(">", TOKEN_OUTPUT), head);
		(*i) += 1;
	}
	else if (line[(*i)] == '<')
	{
		add_new_node_to_list(create_new_lexer_node("<", TOKEN_INPUT), head);
		(*i) += 1;
	}
}