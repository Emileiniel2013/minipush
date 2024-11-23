/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:13:00 by temil-da          #+#    #+#             */
/*   Updated: 2024/11/23 15:06:03 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"

// char	*check_and_expand_env(char **content, int *i, t_mini *minish)
// {
// 	int		j;
// 	char	*temp;
// 	char	*temp2;

// 	j = 0;
// 	temp = NULL;
// 	while ((*content)[*i])
// 	{
// 		if (ft_isalnum((*content)[*i]) == 1 || (*content)[*i] == '_')
// 		{
// 			(*i)++;
// 			j++;
// 		}
// 		else
// 			break ;
// 	}
// 	temp2 = ft_strndup((*content) + ((*i) - j), j);
// 	temp = ft_getenv(minish, temp2);
// 	free(temp2);
// 	temp2 = NULL;
// 	return (temp);
// }

// char	*check_string(char **content, t_mini *minish)
// {
// 	int		i;
// 	char	*expanded_string;
// 	char	*env;

// 	i = 0;
// 	env = NULL;
// 	expanded_string = NULL;
// 	while ((*content)[i] && (*content)[i] != '$')
// 		i++;
// 	if (!(*content)[i])
// 		return (NULL);
// 	expanded_string = ft_strndup(*content, i);
// 	i++;
// 	if ((*content)[i] == '?')
// 	{
// 		env = ft_itoa(minish->exit_code);
// 		i++;
// 	}
// 	else
// 		env = check_and_expand_env(content, &i, minish);
// 	if (env)
// 		replace_varname_wtih_var(&expanded_string, &env);
// 	if ((*content)[i])
// 		append_remainder(&expanded_string, content, i);
// 	return (expanded_string);
// }

// void	expand_env_vars(char **content, t_mini *minish)
// {
// 	char	*expanded_string;

// 	expanded_string = NULL;
// 	while (1)
// 	{
// 		expanded_string = check_string(content, minish);
// 		if (!expanded_string)
// 			break ;
// 		else
// 		{
// 			free(*content);
// 			(*content) = ft_strdup(expanded_string);
// 			free(expanded_string);
// 			expanded_string = NULL;
// 		}
// 	}
// }
// void	replace_varname_wtih_var(char **expanded_string, char **env)
// {
// 	char	*temp;

// 	temp = ft_strdup(*expanded_string);
// 	free(*expanded_string);
// 	*expanded_string = ft_strjoin(temp, *env);
// 	free(temp);
// 	free(*env);
// 	*env = NULL;
// 	temp = NULL;
// }
// void	append_remainder(char **expanded_string, char **content, int pos)
// {
// 	char	*temp;

// 	temp = ft_strdup(*expanded_string);
// 	free(*expanded_string);
// 	*expanded_string = ft_strjoin(temp, (*content) + pos);
// 	free(temp);
// 	temp = NULL;
// }

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
		{
			check_dollar(&param);
		}
		else
			i++;
	}
	handle_remaining_content(*content, &str, last_pos, i);
	return (str);
}

void	handle_remaining_content(char *content, char **str, int last_pos,
	int i)
{
	char	*temp;

	temp = NULL;
	if (last_pos < i)
	{
		temp = ft_strdup(content + last_pos);
		free_and_assign(str, ft_strjoin(*str, temp));
		free(temp);
	}
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
