#include "libraries/parser.h"
#include <readline/readline.h>
#include <readline/history.h>

int main(int ag, char **av)
{
    char *line;
    while (1)
    {
        line = readline("minishell ");
        lexer(line);
    }
}