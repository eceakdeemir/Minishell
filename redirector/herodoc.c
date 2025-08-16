/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herodoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:37:16 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/16 14:29:30 by ecakdemi         ###   ########.fr       */
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
                i = 0;
        }
        else
            i++;
    }
    return (line);
}

static void heredoc_child_process(char *limiter, t_enviroment *env, t_main_struct *main_struct, int hd_no_quoted, int write_fd)
{
    char *line;
    
    setup_signals(HEREDOC_MODE);
    while (1)
    {
        line = mem_absorb(readline("> "));
        if (!line)
            break;
        if (!hd_no_quoted)
            line = heredoc_tokenize_expender(line, env, main_struct);
        if (line && ft_strcmp(line, limiter) == 0)
        {
            break;
        }
        ft_putendl_fd(line, write_fd);
    }
    close(write_fd);
    ft_exit(0);
}

static int wait_child_for_heredoc(pid_t pid, int pipefd[2], t_main_struct *main_struct)
{
    int status;
    
    close(pipefd[1]);
    waitpid(pid, &status, 0);
    
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
    {
        close(pipefd[0]);
        main_struct->last_status = 130;
        return(-1);
    }
    else if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
    {
        close(pipefd[0]);
        return(-1);
    }
    return(0);
}

static int create_heredoc_file(char *limiter, t_enviroment *env, t_main_struct *main_struct, int hd_no_quoted)
{
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return (-1);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return (-1);
    }
    if (pid == 0)
    {
        close(pipefd[0]);
        heredoc_child_process(limiter, env, main_struct, hd_no_quoted, pipefd[1]);
    }
    if (wait_child_for_heredoc(pid, pipefd, main_struct) == -1)
    {
        printf("en son burdan çıktım\n");
        return (-1);   
    }
    return pipefd[0];
}


int prepare_heredocs(t_parser *parser, t_enviroment *env, t_main_struct *main_struct)
{
    t_redirector *redir = parser->redirector;

    while (redir)
    {
        if (redir->token_enum == TOKEN_HEREDOC)
        {
            signal(SIGINT, SIG_IGN);
            signal(SIGQUIT, SIG_IGN);
            redir->herodoc_fd = create_heredoc_file(redir->file, env, main_struct, redir->hd_no_expand);
            setup_signals(INTERACTIVE_MODE);
            if (redir->herodoc_fd  == -1 || g_signal == SIGINT)
                return (heredoc_fail_clear(parser, main_struct)); 
        }
        redir = redir->next;
    }
    return (0);
}
