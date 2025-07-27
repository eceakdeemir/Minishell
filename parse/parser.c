#include "../libraries/parser.h"
#include "../libraries/enviroment.h"


void combine_expender(t_lexer *tmp, int start, int end, char *control_value)
{
    char *first_part;
    char *second_part;

    first_part = ft_substr(tmp->word, 0, start);
    second_part = ft_substr(tmp->word, end, ft_strlen(tmp->word) - end + 1);
    if (control_value)
    {
        tmp->word = ft_strjoin(first_part, control_value);
        tmp->word = ft_strjoin(tmp->word, second_part);//memory leak var.
    }
    else
        tmp->word = ft_strjoin(first_part, second_part);
    free(first_part);
    free(second_part);
}

void control_expender(int start, int end, t_enviroment *env, t_lexer *tmp)
{
    char *is_this_expender;
    char *control_value;

    is_this_expender = ft_substr(tmp->word, start + 1, end - start - 1);
    control_value = get_env_value(is_this_expender, env);
    free(is_this_expender);
    combine_expender(tmp, start, end, control_value);
    free(control_value);
}

void tokenize_expender(t_lexer **head, t_enviroment *env)
{
    t_lexer *tmp;
    int i;
    int start;
    
    tmp = *head;
    while (tmp)
    {
        i = 0;
        while (tmp->word[i])
        {
            if (tmp->word[i] == '\'') // tek tırnak varsa direk geçicek atlanıcak işlem yok. 
            {
               i++;
               while (tmp->word[i] && tmp->word[i] != '\'')
                    i++;
               if (tmp->word[i] == '\'')
                    i++;
            }
            else if (tmp->word[i] == '$') // bunun dışında her kelimede $ varsa expander etcek 
            {
                start = i;
                i++;
                while (tmp->word[i] && (ft_isalnum(tmp->word[i]) || tmp->word[i] == '_')) // değişkenin sonuna kadar gidilir. 
                    i++;
                control_expender(start, i, env, tmp);
                i = 0; // her bir kelime için sıfır yapılır ki baştan tekrar döngüye girsin diye. 
            }
            else
                i++;
        }
        tmp = tmp->next;
    }
}


t_parser **parser_funct(t_lexer **head, t_enviroment **env_struct)
{
    tokenize_expender(head, *env_struct);
    lexer_print(*head);
    return NULL;
}