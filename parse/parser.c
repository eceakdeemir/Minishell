#include "../libraries/minishell.h"
#include <stdio.h>

void	add_redirector(t_parser *cmd, t_token_enum type, char *file)
{
	t_redirector	*new;
	t_redirector	*current;

	new = malloc(sizeof(t_redirector));
	new->token_enum = type;
	new->file = ft_strdup(file);
	new->next = NULL;
	if (!cmd->redirector)
		cmd->redirector = new;
	else
	{
		current = cmd->redirector;
		while (current->next)
			current = current->next;
		current->next = new;
	}
}

t_parser	*create_new_parser_node(t_lexer **lexer)// buraya geldikten sonra pipe kadar komut node'u açar.
{
	t_parser *cmd;
	int count;

	cmd = calloc(1, sizeof(t_parser));
	count = count_args(*lexer);
	cmd->args = malloc(sizeof(char *) * (count + 1));
	if (!cmd->args)
		return (NULL);
	cmd->redirector = NULL;
	if (fill_args_to_parser(cmd, lexer) == 1)// en son bu fonksiyona gider. int yaptım çünkü hata döndüremiyordum seg yiyordum.
	{
		free(cmd->args);
		free(cmd);
		return(NULL);
	}
	return (cmd);
}

int	fill_args_to_parser(t_parser *cmd, t_lexer **lexer)// burda kelime ise komut olarak alınır eğer redirector ise yeni bir redirector adında node açar.
{
	int i;

	i = 0;
	while (*lexer && (*lexer)->token_enum != TOKEN_PIPE)
	{
		if (*lexer && (*lexer)->token_enum == TOKEN_WORD)
		{
			cmd->args[i++] = ft_strdup((*lexer)->word);
			*lexer = (*lexer)->next;
		}
		else
		{
			if (!(*lexer)->next || (*lexer)->next->token_enum != TOKEN_WORD)// redirectordan sonra boş veya beklenemyen bir karakterse 
			{
				syntax_error(lexer);
				cmd->args[i] = NULL;
				while (*lexer)
					*lexer = (*lexer)->next;
				return (1);
			}
			add_redirector(cmd, (*lexer)->token_enum, (*lexer)->next->word);// burda node yolların ve yeni bir struct yapısında tutulur.
			*lexer = (*lexer)->next;
			*lexer = (*lexer)->next;
		}
	}
	cmd->args[i] = NULL;
	return (0);
}

t_parser	*main_parser_func(t_lexer *lexer)// lexerdan gelen kelimeleri pipe göre komutlara çeviriyorum.
{
	t_parser *head;
	t_parser *current;

	head = NULL;
	current = NULL;
	while (lexer)
	{
		t_parser *cmd;
		cmd = create_new_parser_node(&lexer);// yeni parser node oluştura gider
		if (!cmd)
			return(NULL);
		if (!head)
			head = cmd;
		else
			current->next = cmd;
		current = cmd;
		if (lexer && lexer->token_enum == TOKEN_PIPE) // pipesa geçer.
			lexer = lexer->next;
	}
	return (head);
}

t_parser	**parser_funct(t_lexer **head, t_enviroment **env_struct)
{
	tokenize_expender(head, *env_struct);
	remove_quotes_all(head);
	t_parser *parser = main_parser_func(*head);
	if (!parser)
    	return (NULL);
	debug_print_parser(parser);
	return (NULL);
}