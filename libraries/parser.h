#ifndef PARSER_H
#define PARSER_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../libft/libft.h"

//pipe tan sonra gelen komutu anlamlandırma kısmına dikkat et.


typedef enum s_token_enum
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_INPUT,
    TOKEN_OUTPUT,
    TOKEN_APPEND,
    TOKEN_HEREDOC,
} t_token_enum;

typedef struct s_lexer
{
    char *word;
    struct s_lexer *next;
    enum s_token_enum token_enum;
} t_lexer;


t_lexer *create_new_lexer_node(char *str, t_token_enum token);
void add_new_node_to_list(t_lexer *node, t_lexer **head);
void tokenize_herodoc_and_append(t_lexer **head, char *line, int *i);
void tokenize_input_output_and_pipe(t_lexer **head, char *line, int *i);
void add_word_to_list(int start, int *i, t_lexer **head, char *line);
t_lexer **lexer(char *line);






#endif