#include "../libraries/built_in.h"
#include "../libraries/enviroment.h"

//0 başarılı - 1 hata
void print_export_error(char *str)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(str, 2);
	ft_putendl_fd("': not a valid identifier", 2);
}

int is_valid_env_name(char *str)
{
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
	{
		print_export_error(str);
		return (0);
	}
	return (1);
}

int print_export(t_enviroment *env) //formatlı yazdırmak için
{
	while (env)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(env->key, 1);
		if (env->value)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(env->value, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putchar_fd('\n', 1);
		env = env->next;
	}
	return (0);
}

void add_or_update_env(char *str, t_enviroment **env)
{
	char *equal_sign;
	char *key;
	char *value;

	equal_sign = ft_strchr(str, '=');
	if (equal_sign)
	{
		key = extract_key(str);
		value = extract_value(str);
		update_env_value(key, value, env);
		free(key);
		free(value);
	}
	else
	{
		if (!get_env_value(str, *env))
			update_env_value(str, "", env);
	}
}


int built_in_export(t_parser *parser, t_enviroment **env)
{
	int i;
	char *orj_key;
	char *orj_value;

	i = 1;
	if (!parser->args[1])
		return (print_export(*env));
	while (parser->args[i])
	{
		if (!is_valid_env_name(parser->args[i]))
			return (1);
		add_or_update_env(parser->args[i], env);
		i++;
	}
	return (0);
}