#include "../libraries/minishell.h"

int	check_redirector_error(t_lexer **lexer)
{
	t_lexer	*temp;
	int		exit_code;

	temp = *lexer;
	if (temp && (temp->token_enum == TOKEN_APPEND
			|| temp->token_enum == TOKEN_INPUT
			|| temp->token_enum == TOKEN_OUTPUT
			|| temp->token_enum == TOKEN_HEREDOC))
	{
		ft_putstr_fd("syntax error near unexpected token\n", 2);
		exit_code = 2;
		return (1);
	}
	while (temp)
	{
		if ((temp->token_enum == TOKEN_APPEND || temp->token_enum == TOKEN_INPUT
				|| temp->token_enum == TOKEN_OUTPUT
				|| temp->token_enum == TOKEN_HEREDOC) && (!temp->next
				|| temp->next->token_enum != TOKEN_WORD))
		{
			ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
			exit_code = 2;
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}

int	check_pipe_error(t_lexer *lexer)
{
	t_lexer	*temp;
	int		exit_code;

	temp = lexer;
	exit_code = 0;
	if (temp && (temp->token_enum == TOKEN_PIPE))
	{
		ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
		exit_code = 2;
		return (1);
	}
	while (temp)
	{
		if (temp->token_enum == TOKEN_PIPE)
		{
			if (!temp->next)
			{
				ft_putstr_fd("syntax error near unexpected token `newline'\n",
					2);
				exit_code = 2;
				return (1);
			}
			if (temp->next->token_enum == TOKEN_PIPE)
			{
				ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
				exit_code = 2;
				return (1);
			}
		}
		temp = temp->next;
	}
	return (0);
}
