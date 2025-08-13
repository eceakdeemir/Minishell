/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herodoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:37:16 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/13 17:47:41 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libraries/minishell.h"

extern sig_atomic_t g_signal;

static char *heredoc_combine_expender(char *line, int start, int end, char *control_value)
{
    char *first_part;
    char *second_part;

    first_part = ft_substr(line, 0, start);
    second_part = ft_substr(line, end, ft_strlen(line) - end + 1);
    if (control_value)
    {
        line = ft_strjoin(first_part, control_value);
        line = ft_strjoin(line, second_part); // memory leak var.
    }
    else
        line = ft_strjoin(first_part, second_part);
    return (line);
}

static char *heredoc_control_expender(int start, int end, t_enviroment *env, char *line)
{
    char *is_this_expender;
    char *control_value;
    if (line[start] == '$' && end - 1 == start)
        line = heredoc_combine_expender(line, start, end - 1, NULL);
    else
    {
        is_this_expender = ft_substr(line, start + 1, end - start - 1);
        control_value = get_env_value(is_this_expender, env);
        line = heredoc_combine_expender(line, start, end, control_value);
    }
    return (line);
}

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

static int create_heredoc_file(char *limiter, t_enviroment *env, t_main_struct *main_struct)
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
        line = heredoc_tokenize_expender(line, env, main_struct);
        if (line && ft_strcmp(line, limiter) == 0)
            break;
        ft_putendl_fd(line, pipefd[1]);
    }
    close(pipefd[1]);
    return pipefd[0];
}

static int	heredoc_fail_clear(t_parser *parser, t_main_struct *main_struct)
{
	t_redirector	*redirector;

	if (g_signal == SIGINT)
		main_struct->last_status = 130;
	else
		main_struct->last_status = 1;
	g_signal = 0;
	redirector = parser->redirector;
	while (redirector)
	{
		if (redirector->token_enum == TOKEN_HEREDOC)
			redirector->herodoc_fd = -1;
		redirector = redirector->next;
	}
	return (-1);
}


int prepare_heredocs(t_parser *parser, t_enviroment *env, t_main_struct *main_struct)
{
    t_redirector *redir = parser->redirector;

    while (redir)
    {
        if (redir->token_enum == TOKEN_HEREDOC)
        {
            setup_signals(HEREDOC_MODE);
            redir->herodoc_fd = create_heredoc_file(redir->file, env, main_struct);
            setup_signals(INTERACTIVE_MODE);
            if (redir->herodoc_fd  == -1 || g_signal == SIGINT)
                return (heredoc_fail_clear(parser, main_struct)); 
        }
        redir = redir->next;
    }
    return (0);
}
