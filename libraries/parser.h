/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:36:36 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/15 18:07:56 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    TOKEN_INPUT,    //2 <
    TOKEN_OUTPUT,   //3 >
    TOKEN_APPEND,   //4 >>
    TOKEN_HEREDOC,  //5 <<
} t_token_enum;

typedef struct s_redirector
{
    enum s_token_enum token_enum;
    char *file;
    int herodoc_fd;
    int				hd_no_expand; 
    struct s_redirector *next;
} t_redirector;

typedef struct s_lexer
{
    char *word;
    int	 heredoc_quoted;
    struct s_lexer *next;
    enum s_token_enum token_enum;
} t_lexer;


typedef enum s_built_in_type
{
    ECHO,   // 0
	CD,     // 1
	PWD,    // 2
	EXPORT, // 3
	UNSET,  // 4
	ENV,    // 5
	EXIT,   // 6
} t_built_in_type;

//şimdilik burda kalsın yeri değişcek
typedef struct s_parser
{
    char **args;
    t_redirector *redirector;
    t_built_in_type built_type;
    struct s_parser *next;
} t_parser;

//şimdilik burda kalsın yeri değişcek
typedef struct s_main_struct
{
	t_parser *parser_struct;
	t_enviroment **env_struct;
	char **env;
    int last_status;
    pid_t last_child_pid;
} t_main_struct;

//lexer.c
t_lexer *create_new_lexer_node(char *str, t_token_enum token);
void add_new_node_to_list(t_lexer *node, t_lexer **head);
void add_word_to_list(int start, int *i, t_lexer **head, char *line);
t_lexer **lexer(char *line);

//tokenize.c
void tokenize_heredoc_and_append(t_lexer **head, char *line, int *i);
void tokenize_input_output_and_pipe(t_lexer **head, char *line, int *i);


//expander.c
void tokenize_expender(t_lexer **head, t_enviroment *env, t_main_struct *main_struct);
void control_expender(int start, int end, t_enviroment *env, t_lexer *tmp);
void combine_expender(t_lexer *tmp, int start, int end, char *control_value);

//parser_utils.c
int ft_isspace(char line);
char *ft_strndup(const char *src, int len);
int	ft_strcmp(const char *s1, const char *s2);
int count_args(t_lexer *lexer);

//remove_quıtes.c
void remove_quotes_all(t_lexer **head);
char *remove_quotes(char *word);

//parser.c
void	add_redirector(t_parser *cmd, t_token_enum type, char *file, int hd_no_expand);
t_parser *create_new_parser_node(t_lexer **lexer);
void	fill_args_to_parser(t_parser *cmd, t_lexer **lexer);
t_parser *main_parser_func(t_lexer *lexer);
t_parser	*parser_funct(t_lexer **head, t_enviroment **env_struct, t_main_struct *main_struct);

//debug.c
void debug_print_parser(t_parser *head);
void lexer_print(t_lexer *head);
void print_redirects(t_redirector *redir);
void    print_env_list(t_enviroment **env_list);

//error.c
int	check_redirector_error(t_lexer **lexer);
int check_pipe_error(t_lexer *lexer);

//heredoc_parser.c
int	is_has_quote(const char *s);
void	decide_heredoc_quoted(t_lexer *head);
char *heredoc_combine_expender(char *line, int start, int end, char *control_value);
char *heredoc_control_expender(int start, int end, t_enviroment *env, char *line);
int	heredoc_fail_clear(t_parser *parser, t_main_struct *main_struct);

#endif