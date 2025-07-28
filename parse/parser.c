#include "../libraries/minishell.h"


t_parser **parser_funct(t_lexer **head, t_enviroment **env_struct)
{
    tokenize_expender(head, *env_struct);
    remove_quotes_all(head);
    lexer_print(*head);
    return NULL;
}