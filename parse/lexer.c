#include "../libraries/parser.h"


int ft_isspace(char line)
{
    if (line == ' ' || line == '\t' || line == '\n' || line == '\v' || line == '\r' || line == '\f')
        return (1);
    return (0);
}

char *ft_strndup(const char *src, int len)
{
    char *dest = malloc(len + 1);
    int i;

    if (!dest)
        return NULL;
    i = 0;
    while (i < len)
    {
        dest[i] = src[i];
        i++;
    }
    dest[len] = '\0';
    return dest;
}

t_lexer *create_new_lexer_node(char *str, t_token_enum token)
{
    t_lexer *new_node;
    new_node = malloc(sizeof(t_lexer));
    if (!new_node)
        return (NULL);
    new_node->word = str;
    new_node->token_enum = token;
    new_node->next = NULL;
    return (new_node);
}

void add_new_node_to_list(t_lexer *node, t_lexer **head)
{
    t_lexer *tmp;

    tmp = *head;
    if (!tmp)
    {
        *head = node;
        return ;
    }
    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = node;
}


void tokenize_herodoc_and_append(t_lexer **head, char *line, int *i)
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

void tokenize_input_output_and_pipe(t_lexer **head, char *line, int *i)
{
    if (line[(*i)] == '|')
    {
        add_new_node_to_list(create_new_lexer_node("|", TOKEN_PIPE), head);
        (*i) += 1;
    }
    else if (line[(*i)] == '>')
    {
        add_new_node_to_list(create_new_lexer_node(">", TOKEN_INPUT), head);
        (*i) += 1;
    }
    else if (line[(*i)] == '<')
    {
        add_new_node_to_list(create_new_lexer_node("<", TOKEN_OUTPUT), head);
        (*i) += 1;
    }
}
void lexer_print(t_lexer *list)
{
    while (list)
    {
        printf("%d %s \n", list->token_enum, list->word);
        list = list->next;
    }
}

void add_word_to_list(int start, int *i, t_lexer **head, char *line)
{
    if (line[*i] == '\"' || line[*i] == '\'')
    {
        char quote = line[(*i)];
        (*i)++;
        while (line[(*i)] && line[(*i)] != quote)
            (*i)++;
        if (line[(*i)] == quote)
            (*i)++;
    }
    while ((line[*i] && !ft_isspace(line[*i])) && !ft_strchr("|<>\"\'", line[*i]))
        (*i)++;
    add_new_node_to_list(create_new_lexer_node(ft_strndup(&line[start], (*i) - start), TOKEN_WORD), head);
}

t_lexer **lexer(char *line)
{
    t_lexer **head;
    int i;

    i = 0;
    head = malloc(sizeof(t_lexer *));
    *head = NULL;
    while (line[i])
    {
        while (ft_isspace(line[i]) == 1)
            i++;
        if (((line[i] == '<' && line[i + 1] == '<') || (line[i] == '>' && line[i + 1] == '>')) && line[i + 1])
            tokenize_herodoc_and_append(head, line, &i);
        else if ((line[i] == '<') || (line[i] == '>') || (line[i] == '|'))
            tokenize_input_output_and_pipe(head, line, &i);
        else
            add_word_to_list(i, &i, head, line);
    }
    lexer_print(*head);
    return (head);
}