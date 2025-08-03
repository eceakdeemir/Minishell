#include "libraries/minishell.h"

int main(int ag, char **av, char **envp)
{
    t_enviroment **env_list = malloc(sizeof(t_enviroment *));
    t_main_struct *main_struct = malloc(sizeof(t_main_struct));
    t_parser **parser_head;
    init_env(envp, env_list);
	main_struct->env_struct = env_list;
    char *line;
    t_lexer **lexer_head;
    while (1)
    {
        line = readline("minishell ");
        lexer_head = lexer(line);
        parser_head = parser_funct(lexer_head, main_struct->env_struct);
        
    }
}