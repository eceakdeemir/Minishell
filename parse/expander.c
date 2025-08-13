/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:36:50 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/13 17:47:19 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

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
}

void control_expender(int start, int end, t_enviroment *env, t_lexer *tmp)
{
    char *is_this_expender;
    char *control_value;
    if (tmp->word[start] == '$' && end - 1 == start)
        combine_expender(tmp, start, end - 1, NULL);
    else
    {
        is_this_expender = ft_substr(tmp->word, start + 1, end - start - 1);
        control_value = get_env_value(is_this_expender, env);
        combine_expender(tmp, start, end, control_value);
    }
}

void tokenize_expender(t_lexer **head, t_enviroment *env, t_main_struct *main_struct) //25'den fazla satır uzunluğı  
{
    t_lexer *tmp;
    int i;
    int start;
    char *return_val;
    
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
                if (tmp->word[i + 1] == 0)
                    return;
                if (tmp->word[i + 1] == '?')
                {
                    return_val = ft_itoa(main_struct->last_status);
                    combine_expender(tmp, i, i + 2, return_val);
                    i = 0;
                    continue;
                }
                start = i;
                i++;
                while (tmp->word[i] && (ft_isalnum(tmp->word[i]) || tmp->word[i] == '_')) // değişkenin sonuna kadar gidilir. 
                    i++;
                control_expender(start, i, env, tmp);
                if (i - 1 != start)
                    i = 0; // her bir kelime için sıfır yapılır ki baştan tekrar döngüye girsin diye. 
            }
            else
                i++;
        }
        tmp = tmp->next;
    }
}
