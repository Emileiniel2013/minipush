/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:13:00 by temil-da          #+#    #+#             */
/*   Updated: 2024/11/16 16:12:16 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"


// bool exp_env_vars(char **content, t_mini *msh)
// {
// 	char *expanded_string;
// 	char *prefix;
// 	char *env_exit_code;
// 	char *variable_name;
// 	char *get_value;
// 	int i = 0;
// 	int start = 0;	

// 	expanded_string = ft_strdup("");
// 	 if (*content == NULL || **content == '\0')  // Check if the content is empty
//         return true;
// 	 printf("Initial content: %s\n", *content);
// 	while((*content)[i])
// 	{
// 		while ((*content)[i] && (*content)[i] != '$')
// 			i++;
// 		printf("here\n");
// 		printf("skip the dollar sign\n");
// 		if (!(*content)[i])
// 			break;
// 		prefix = ft_strndup(*content, i);
// 		printf("Prefix: %s\n", prefix);
// 		expanded_string = ft_strjoin(expanded_string, prefix);
// 		printf("Exit code: %s\n", env_exit_code);
// 		free(prefix); // free prefix
// 		if ((*content)[i] == '?'){
// 			env_exit_code = ft_itoa(msh->exit_code);
// 		expanded_string = ft_strjoin(expanded_string, env_exit_code);
// 		i = i + 2;
// 		continue;
// 		}
// 		if ((*content)[i] == '$')
// 		{
// 			start = i + 1;
// 			i++;
// 			while ( i < ft_strlen(*content) && (ft_isalnum((*content)[i]) || (*content)[i] == '_'))
// 				i++;
// 			//while(isalnum(*content[i]) || *content[i] == '_')
// 			variable_name = ft_strndup(*content + start, i - start);
// 			printf("Variable name: %s\n", variable_name);
// 			get_value = ft_getenv(msh, variable_name);
// 			free(variable_name);
// 			if (get_value)
// 			{
// 				 printf("Variable value: %s\n", get_value);
// 				expanded_string = ft_strjoin(expanded_string, get_value);
// 				free(get_value);
// 			}
// 		}
// 		if ((*content)[i])
// 		{
// 			expanded_string = ft_strjoin(expanded_string, *content + i);
// 		}		
// 	}
// 	 printf("Expanded string: %s\n", expanded_string);
// 	free(*content);
// 	*content = expanded_string;
// 	return (true);
// }
char	*check_and_expand_env(char **content, int *i, t_mini *minish)
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

char	*check_string(char **content, t_mini *minish)
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

void	expand_env_vars(char **content, t_mini *minish)
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

bool	heredoc_loop(char *delimiter, int fd)
{
	char	*line;

	line = NULL;
	line = readline("heredoc> ");
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		line = NULL;
		return (false);
	}
	write(fd, line, strlen(line));
	write(fd, "\n", 1);
	free(line);
	line = NULL;
	return (true);
}
