/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herodoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibrahimberatgurses <ibrahimberatgurses@    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:37:16 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/14 14:55:23 by ibrahimbera      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

extern sig_atomic_t g_signal;

static char *heredoc_tokenize_expender(char *line, t_enviroment *env, t_main_struct *main_struct) // 25'den fazla satır uzunluğı
{
    int i;
    int start;
    char *return_val;

    i = 0;
    if (!line)
        return (NULL);
    while (line[i])
    {
        if (line[i] == '$') // bunun dışında her kelimede $ varsa expander etcek
        {
            if (line[i + 1] == 0)
                return NULL;
            if (line[i + 1] == '?')
            {
                return_val = ft_itoa(main_struct->last_status);
                line = heredoc_combine_expender(line, i, i + 2, return_val);
                i = 0;
                continue;
            }
            start = i;
            i++;
            while (line[i] && (ft_isalnum(line[i]) || line[i] == '_')) // değişkenin sonuna kadar gidilir.
                i++;
            line = heredoc_control_expender(start, i, env, line);
            if (i - 1 != start)
                i = 0; // her bir kelime için sıfır yapılır ki baştan tekrar döngüye girsin diye.
        }
        else
            i++;
    }
    return (line);
}

static int create_heredoc_file(char *limiter, t_enviroment *env, t_main_struct *main_struct, int hd_no_quoted)
{
    char *line;
    int pipefd[2];

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return -1;
    }

    while (!g_signal) // g_signal kontrol et
    {
        line = readline("> ");
        if (!line) // EOF veya readline hatası
        {
            write(STDOUT_FILENO, "\n", 1);
            close(pipefd[0]);
            close(pipefd[1]);
            return (-1);
        }
        if (g_signal == SIGINT)
        {
            close(pipefd[0]);
            close(pipefd[1]);
            return (-1);
        }
         if (!hd_no_quoted)
            line = heredoc_tokenize_expender(line, env, main_struct);
        if (line && ft_strcmp(line, limiter) == 0)
        {
            free(line);
            break;
        }
        ft_putendl_fd(line, pipefd[1]);
        free(line);
    }
    close(pipefd[1]);
    return pipefd[0];
}


int prepare_heredocs(t_parser *parser, t_enviroment *env, t_main_struct *main_struct)
{
    t_redirector *redir = parser->redirector;

    while (redir)
    {
        if (redir->token_enum == TOKEN_HEREDOC)
        {
            setup_signals(HEREDOC_MODE);
            redir->herodoc_fd = create_heredoc_file(redir->file, env, main_struct, redir->hd_no_expand);
            setup_signals(INTERACTIVE_MODE);
            if (redir->herodoc_fd  == -1 || g_signal == SIGINT)
                return (heredoc_fail_clear(parser, main_struct)); 
        }
        redir = redir->next;
    }
    return (0);
}
