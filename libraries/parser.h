#ifndef PARSER_H
#define PARSER_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "enviroment.h"
#include "../libft/libft.h"

//pipe tan sonra gelen komutu anlamlandırma kısmına dikkat et.

typedef enum s_token_enum
{
    TOKEN_WORD,     //0
    TOKEN_PIPE,     //1
    TOKEN_INPUT,    //2
    TOKEN_OUTPUT,   //3
    TOKEN_APPEND,   //4
    TOKEN_HEREDOC,  //5
} t_token_enum;

typedef struct s_lexer
{
    char *word;
    struct s_lexer *next;
    enum s_token_enum token_enum;
} t_lexer;


//şimdilik burda kalsın yeri değişcek
typedef struct s_parser
{
    t_lexer *lexer;
    char **args;
    char *infile;
    char *outfile;
    char *append;
    char *heredoc;
    struct s_parser *next;
    struct s_parser *prev;
} t_parser;

//şimdilik burda kalsın yeri değişcek
typedef struct s_main_struct
{
	t_parser *parser_struct;
	t_enviroment **env_struct;
	char **env;

} t_main_struct;


//lexer.c
t_lexer *create_new_lexer_node(char *str, t_token_enum token);
void add_new_node_to_list(t_lexer *node, t_lexer **head);
void add_word_to_list(int start, int *i, t_lexer **head, char *line);
t_lexer **lexer(char *line);

//tokenize.c
void tokenize_heredoc_and_append(t_lexer **head, char *line, int *i);
void tokenize_input_output_and_pipe(t_lexer **head, char *line, int *i);



//parser.c
void tokenize_expender(t_lexer **head, t_enviroment *env);
t_parser **parser_funct(t_lexer **head, t_enviroment **env_struct);
void control_expender(int start, int end, t_enviroment *env, t_lexer *tmp);
void combine_expender(t_lexer *tmp, int start, int end, char *control_value);



//parser_utils.c
int ft_isspace(char line);
char *ft_strndup(const char *src, int len);
int	ft_strcmp(const char *s1, const char *s2);
void lexer_print(t_lexer *head);

//remove_quıtes.c
void remove_quotes_all(t_lexer **head);
char *remove_quotes(char *word);







#endif