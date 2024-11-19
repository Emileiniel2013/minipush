/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:13:00 by temil-da          #+#    #+#             */
/*   Updated: 2024/11/19 14:32:10 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"

bool	exp_env_vars(char **content, t_mini *msh)
{
	char	*exp_string;

	while (1)
	{
		exp_string = handle_content(content, msh);
		if (ft_strcmp(*content, exp_string) == 0)
		{
			free(exp_string);
			break ;
		}
		free_and_assign(content, exp_string);
	}
	return (true);
}

char	*handle_content(char **content, t_mini *msh)
{
	char			*str;
	int				i;
	int				last_pos;
	char			*tmp;
	t_dollar_param	param;

	i = 0;
	last_pos = 0;
	str = ft_strdup("");
	param.content = content;
	param.i = &i;
	param.last_pos = &last_pos;
	param.str = &str;
	param.msh = msh;
	while ((*content)[i])
	{
		if ((*content)[i] == '$')
			check_dollar(&param);
		else
			i++;
	}
	if (last_pos < i)
	{
		tmp = ft_strdup((*content) + last_pos);
		free_and_assign(&str, ft_strjoin(str, tmp));
		free(tmp);
	}
	return (str);
}

void	check_dollar(t_dollar_param *param)
{
	char	*temp;
	char	*env;

	temp = ft_strndup((*(param->content) + *(param->last_pos)), (*(param->i)
				- *(param->last_pos)));
	free_and_assign(param->str, ft_strjoin(*(param->str), temp));
	free(temp);
	(*(param->i))++;
	if ((*(param->content))[*(param->i)] == '?')
	{
		env = ft_itoa(param->msh->exit_code);
		(*(param->i))++;
	}
	else
		env = expand_var(param->msh, *(param->content), param->i);
	if (env)
	{
		free_and_assign(param->str, ft_strjoin(*(param->str), env));
		free(env);
	}
	*(param->last_pos) = *(param->i);
}

void	free_and_assign(char **dst, char *src)
{
	free(*dst);
	*dst = src;
}

char	*expand_var(t_mini *msh, const char *content, int *i)
{
	int		start;
	int		len;
	char	*var_name;
	char	*env_value;

	start = *i;
	len = 0;
	while (ft_isalnum(content[*i + len]) || content[*i + len] == '_')
		len++;
	var_name = ft_strndup(content + start, len);
	env_value = ft_get_env_value(msh, var_name);
	free(var_name);
	*i += len;
	return (env_value);
}

// bool	heredoc_loop(char *delimiter, int fd)
// {
// 	char	*line;

// 	line = NULL;
// 	line = readline("heredoc> ");
// 	if (ft_strcmp(line, delimiter) == 0)
// 	{
// 		free(line);
// 		line = NULL;
// 		return (false);
// 	}
// 	write(fd, line, strlen(line));
// 	write(fd, "\n", 1);
// 	free(line);
// 	line = NULL;
// 	return (true);
// }
