#include "../libraries/minishell.h"

void	print_redirects(t_redirector *redir)
{
	while (redir)
	{
		printf("    redirect type (%d): ", redir->token_enum);
		if (redir->token_enum == TOKEN_INPUT)
			printf("< %s\n", redir->file);
		else if (redir->token_enum == TOKEN_OUTPUT)
			printf("> %s\n", redir->file);
		else if (redir->token_enum == TOKEN_APPEND)
			printf(">> %s\n", redir->file);
		else if (redir->token_enum == TOKEN_HEREDOC)
			printf("<< %s\n", redir->file);
		else
			printf("? %s\n", redir->file);
		redir = redir->next;
	}
}
void	debug_print_parser(t_parser *head)
{
	int	cmd_num;

	cmd_num = 1;
	while (head)
	{
		printf("---- Command %d ----\n", cmd_num++);
		printf("args:");
		for (int i = 0; head->args && head->args[i]; i++)
			printf(" [%s]", head->args[i]);
		printf("\n");
		if (head->redirector)
			print_redirects(head->redirector);
		else
			printf("    no redirects\n");
		head = head->next;
	}
}

void	lexer_print(t_lexer *list)
{
	while (list)
	{
		printf("%d %s \n", list->token_enum, list->word);
		list = list->next;
	}
}

void	print_env_list(t_enviroment **env_list)
{
	t_enviroment *current;

	if (!env_list || !*env_list)
	{
		printf("env_list boş!\n");
		return ;
	}
	current = *env_list;
	while (current)
	{
		printf("KEY: %s | VALUE: %s\n", current->key ? current->key : "(null)",
			current->value ? current->value : "(null)");
		current = current->next;
	}
}