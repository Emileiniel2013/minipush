/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 16:16:12 by temil-da          #+#    #+#             */
/*   Updated: 2024/12/04 21:46:53 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"

char	*ft_get_env_value(t_mini *msh, const char *var_name)
{
	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(msh->exit_code));
	else
		return (ft_getenv(msh, var_name));
}

int	ft_strcmp(const char *s1, char *s2)
{
	while (*s1 == *s2 && *s1 && *s2)
	{
		++s1;
		++s2;
	}
	return (*s1 - *s2);
}

char	*ft_getenv(t_mini *mini, const char *env)
{
	size_t	i;
	int		len;
	char	*var;

	len = ft_strlen(env);
	i = -1;
	var = NULL;
	while (mini->env[++i])
	{
		if (ft_strncmp(mini->env[i], env, len) == 0 && mini->env[i][len] == '=')
		{
			var = ft_strdup(mini->env[i] + (len + 1));
			return (var);
		}
	}
	return (var);
}

char	**copy_env(char **envp)
{
	char	**cpy;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (envp[i] != NULL)
		i++;
	cpy = malloc(sizeof(char *) * (i + 1));
	cpy[i] = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		cpy[j] = ft_strdup(envp[i]);
		i++;
		j++;
	}
	return (cpy);
}

bool	process_heredoc_input(int fd, char *separator)
{
	char	*line;

	while (true)
	{
		line = readline("heredoc> ");
		if (!line)
			return (false);
		if (ft_strcmp(line, separator) == 0)
		{
			free(line);
			break ;
		}
		write_line(fd, line);
		free(line);
	}
	return (true);
}
