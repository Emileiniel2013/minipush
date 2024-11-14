/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:30:21 by tndreka           #+#    #+#             */
/*   Updated: 2024/11/14 15:06:55 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

 #include "../include/mini_sh.h"


void	expand_env_vars(char **content, t_msh *minish)
{
	char	*expanded_string;

	expanded_string = NULL;
	while (1)
	{
		expanded_string = check_string(content, minish);
		if (!expanded_string)
			break ;
		else
		{
			free(*content);
			(*content) = ft_strdup(expanded_string);
			free(expanded_string);
			expanded_string = NULL;
		}
	}
}
void	append_remainder(char **expanded_string, char **content, int pos)
{
	char	*temp;

	temp = ft_strdup(*expanded_string);
	free(*expanded_string);
	*expanded_string = ft_strjoin(temp, (*content) + pos);
	free(temp);
	temp = NULL;
}

void	replace_varname_wtih_var(char **expanded_string, char **env)
{
	char	*temp;

	temp = ft_strdup(*expanded_string);
	free(*expanded_string);
	*expanded_string = ft_strjoin(temp, *env);
	free(temp);
	free(*env);
	*env = NULL;
	temp = NULL;
}

// int	ft_strcmp(char *s1, char *s2)
// {
// 	while (*s1 == *s2 && *s1 && *s2)
// 	{
// 		++s1;
// 		++s2;
// 	}
// 	return (*s1 - *s2);
// }

// char	*ft_getenv(t_msh *mini, char *env)
// {
// 	size_t	i;
// 	int		len;
// 	char	*var;

// 	len = ft_strlen(env);
// 	i = -1;
// 	var = NULL;
// 	while (mini->env[++i])
// 	{
// 		if (ft_strncmp(mini->env[i], env, len) == 0 && mini->env[i][len] == '=')
// 		{
// 			var = ft_strdup(mini->env[i] + (len + 1));
// 			return (var);
// 		}
// 	}
// 	return (var);
// }

// char	**copy_env(char **envp)
// {
// 	char	**cpy;
// 	int		i;
// 	int		j;

// 	i = 0;
// 	j = 0;
// 	while (envp[i] != NULL)
// 		i++;
// 	cpy = malloc(sizeof(char *) * (i + 1));
// 	cpy[i] = NULL;
// 	i = 0;
// 	while (envp[i] != NULL)
// 	{
// 		if (ft_strncmp(envp[i], "OLDPWD=", 7) != 0)
// 		{
// 			cpy[j] = ft_strdup(envp[i]);
// 			i++;
// 			j++;
// 		}
// 		else
// 			i++;
// 	}
// 	return (cpy);
// }

char	*check_and_expand_env(char **content, int *i, t_msh *minish)
{
	int		j;
	char	*temp;
	char	*temp2;

	j = 0;
	temp = NULL;
	while ((*content)[*i])
	{
		if (ft_isalnum((*content)[*i]) == 1 || (*content)[*i] == '_')
		{
			(*i)++;
			j++;
		}
		else
			break ;
	}
	temp2 = ft_strndup((*content) + ((*i) - j), j);
	temp = ft_getenv(minish, temp2);
	free(temp2);
	temp2 = NULL;
	return (temp);
}

char	*check_string(char **content, t_msh *minish)
{
	int		i;
	char	*expanded_string;
	char	*env;

	i = 0;
	env = NULL;
	expanded_string = NULL;
	while ((*content)[i] && (*content)[i] != '$')
		i++;
	if (!(*content)[i])
		return (NULL);
	expanded_string = ft_strndup(*content, i);
	i++;
	if ((*content)[i] == '?')
	{
		env = ft_itoa(minish->exit_code);
		i++;
	}
	else
		env = check_and_expand_env(content, &i, minish);
	if (env)
		replace_varname_wtih_var(&expanded_string, &env);
	if ((*content)[i])
		append_remainder(&expanded_string, content, i);
	return (expanded_string);
}
// bool	trip_to_table_commad(t_lexer *token, t_table *table, t_msh *msh)
// {
// 	t_lexer		*temp;
// 	t_table		*new_node;
// 	int			cmd_len;

// 	(void)msh;
// 	temp = token;
// 	if (!count_allocate_commands(&temp, &new_node, &cmd_len))
// 	{
// 		return(false);
// 	}
// 	if (!add_command_in_table(new_node, table, cmd_len, token))
// 	{
// 		return(false);
// 	}
// 	return (true);
// }

// bool	count_allocate_commands(t_lexer **temp, t_table **new_node,
// 					int *cmd_len)
// {
// 	*cmd_len = 0;
// 	while ((*temp) && (*temp)->type == COMMAND)
// 	{
// 		(*cmd_len)++;
// 		(*temp) = (*temp)->next;
// 	}
// 	(*new_node) = malloc(sizeof(t_table));
// 	if (!(*new_node))
// 		return (false);
// 	(*new_node)->command = malloc(sizeof(char *) * ((*cmd_len) + 1));
// 	if (!(*new_node)->command)
// 	{
// 		free(new_node);
// 		return (false);
// 	}
// 	(*new_node)->leftpipe = false;
// 	(*new_node)->cmd_head = NULL;
// 	return (true);
// }

// bool	add_command_in_table(t_table *new_node, t_table *table, int cmd_len, t_lexer *token)
// {
// 	t_lexer		*tempo = NULL;
// 	t_table		*current;
// 	int			i;

// 	i = 0;
// 	tempo = token;
// 	while (tempo && i < cmd_len)
// 	{
// 		new_node->command[i].content = ft_strdup(tempo->data);
// 		if (!new_node->command[i].content)
//         {
//             // Free previously allocated memory in case of failure
//             while (i > 0)
//             {
// 				free(new_node->command[--i].content);
//             }
//             free(new_node->command);
//             free(new_node);
//             return (false);
//         }
// 	//	new_node->command[i].next = NULL;
// 		tempo = tempo->next;
// 		i++;
// 	}
// 	new_node->command[i].content = NULL;
// 	if (table)
// 	{
// 		current = table;
// 		while (current->next)
// 		{
// 			current = current->next;
// 		}
// 		current->next = new_node;
// 	}
// 	else
// 		table = new_node;
// 	return (true);
// }
