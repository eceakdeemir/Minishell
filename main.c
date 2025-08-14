/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakdemi <ecakdemi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:39:27 by ecakdemi          #+#    #+#             */
/*   Updated: 2025/08/14 16:41:33 by ecakdemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libraries/minishell.h"

static t_main_struct *init_main_struct(char **envp)
{
    t_main_struct *main_struct;
    t_enviroment **env_list;

    env_list = mem_malloc(sizeof(t_enviroment *));
    main_struct = mem_malloc(sizeof(t_main_struct));

	*env_list = NULL;
    if (!env_list || !main_struct)
        ft_exit(1);
    main_struct->last_status = 0;
    init_env(envp, env_list);
    main_struct->env = envp;
    main_struct->env_struct = env_list;
    return (main_struct);
}

static void restore_stdio(int org_o, int org_i)
{
    dup2(org_o, STDOUT_FILENO);
    dup2(org_i, STDIN_FILENO);
    close(org_o);
    close(org_i);
}

static void handle_no_command(t_parser *parser, t_main_struct *main_struct)
{
    if (!parser || !parser->redirector)
        return;
    if (prepare_heredocs(parser, *(main_struct->env_struct), main_struct) == -1)
        return;
    if (main_redirector(parser, 0) == -1)
        main_struct->last_status = 1;
    else
        main_struct->last_status = 0;
}

static void	process_line(char *line, t_main_struct *main_struct)
{
    t_lexer		**lexer_head;
	t_parser	*parser_head;

	if (!line || line[0] == '\0')
		return ;
	add_history(line);
	lexer_head = lexer(line);
	if (!lexer_head || !*lexer_head)
		return ;
	parser_head = parser_funct(lexer_head, main_struct->env_struct, main_struct);
	compact_all_commands(parser_head);
	if (!parser_head)
		return ;
	if (!parser_head->args || !parser_head->args[0])
		handle_no_command(parser_head, main_struct);
	else
	{
		add_built_in_token(&parser_head);
		execute(parser_head->args, main_struct, parser_head);
	}
}

int main(int ac, char **av, char **envp)
{
	char			*line;
	t_main_struct	*main_struct;
	int				org_o;
	int				org_i;

	(void)ac;
	(void)av;
	main_struct = init_main_struct(envp);
	while (1)
	{
		setup_signals(INTERACTIVE_MODE);
		org_o = dup(STDOUT_FILENO);
		org_i = dup(STDIN_FILENO);
		line = readline("minishell ");
		if (g_signal == SIGINT)
		{
			main_struct->last_status = 130;
			g_signal = 0;
		}
		if (!line)
		{
			printf("exit\n");
			ft_exit(0);
		}
		process_line(line, main_struct);
		restore_stdio(org_o, org_i);
	}
	mem_free();
	main_struct = NULL;
}

