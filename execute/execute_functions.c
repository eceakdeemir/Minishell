#include "../libraries/minishell.h"
#include "../libraries/execute.h"

int count_cmd(t_parser *parser)
{
    int i;

    i = 0;
    while (parser)
    {
        if (parser->args != NULL)
            i++;
        parser = parser->next;
    }
    return (i);
}

void this_is_not_built_in(char **cmd, t_main_struct *main_struct, t_parser *parser)
{
    char *path;
    // -1 yaptım yani bu builtin değil; ama 0 ve 6 arasında ise builtin fonksiyonudur
        // redirector(parser);   //şu fonksiyon yazılıcak   
        path = find_path(*cmd, main_struct->env);  
        if (!path)
        {
            ft_putstr_fd("Komut bulunamadı: ", 2);
            ft_putendl_fd(cmd[0], 2);
            exit(127); // bunlar değişmeli mi?
        }
        execve(path, cmd, main_struct->env);
        perror("execve");
        exit(127); // bunlar değişmeli mi??
    
}

void execute(char **cmd, t_main_struct *main_struct, t_parser *parser)
{

    int cmd_count;
    pid_t pid;

    cmd_count = count_cmd(parser);
    if (cmd_count == 1)
    {
        if (parser->built_type >= 0 && parser->built_type <= 6 )// ⬅️ cd, export, unset, exit gibi komutlar shell durumunu değiştirdiği için ana processte çalışmalı bu yüzden burada yaptık
		{
            run_built_in(parser, main_struct); // fork açmadan çalıştır.
            return ;
        }
        pid = fork();
        if (pid == 0)
			this_is_not_built_in(cmd, main_struct, parser);
        else
            waitpid(pid, NULL, 0);
    }
    else
        execute_main(parser, main_struct->env);
}
