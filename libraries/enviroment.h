

#ifndef ENV_H
#define ENV_H



typedef struct s_enviroment
{
    char *key;
    char *value;
    struct s_enviroment *next;

} t_enviroment;


t_enviroment *create_new_node_for_env(char *key, char *value);
void add_new_node_for_env(t_enviroment **head, t_enviroment *env);
char *get_env_value(const char *key, t_enviroment *env);
void update_env_value(const char *key, const char *value, t_enviroment **env);
void init_env(char **env, t_enviroment **env_list);

#endif