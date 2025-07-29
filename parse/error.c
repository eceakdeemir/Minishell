#include "../libraries/minishell.h"



int    syntax_error(t_lexer **lexer)
{
	char	*mistake;

	if ((*lexer)->next)
		mistake = (*lexer)->next->word;
	else
		mistake = "newline";
	ft_putstr_fd("syntax error near unexpected token `", 2);
	ft_putstr_fd(mistake, 2);
	ft_putendl_fd("'", 2);
	return (2); // exit code 2  
}