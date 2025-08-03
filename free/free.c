
#include "../libraries/minishell.h"

void free_argv(char **argv)
{
    int i;

    i = 0;
    while (argv[i])
    {
        free(argv[i]);
        i++;
    }
    free(argv);
}