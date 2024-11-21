/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:43:06 by temil-da          #+#    #+#             */
/*   Updated: 2024/11/21 18:41:18 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lexer.h"

t_lexer	*lexer(char *prompt, t_mini *msh)
{
	t_lexer			*head;
	t_lexer			*current;
	t_lexer_params	param;
	int				i;

	param.prompt = prompt;
	param.head = &head;
	param.current = &current;
	param.i = &i;
	param.msh = msh;
	head = NULL;
	current = NULL;
	i = 0;
	while (prompt[i])
	{
		while (prompt[i] && ft_isspace(prompt[i]))
			i++;
		handle_token(&param);
		if (prompt[i])
			i++;
	}
	return (head);
}

void	handle_token(t_lexer_params *param)
{
	char		*prompt;
	t_lexer		**head;
	t_lexer		**current;
	int			*i;

	prompt = param->prompt;
	head = param->head;
	current = param->current;
	i = param->i;
	if (prompt[*i] == '|')
		*current = create_tok("|", PIPE);
	else if (prompt[*i] == '\"')
		double_qoute(param);
	else if (prompt[*i] == '\'')
		single_qoute(param);
	else if (prompt[*i] == '>')
		redirection(param);
	else if (prompt[*i] == '<')
		redirection_less(param);
	else if (prompt[*i])
		create_comand_token(param);
	if (prompt[*i] == '|' || prompt[*i] == '\"' || prompt[*i] == '\''
		|| prompt[*i] == '>' || prompt[*i] == '<')
		add_token(head, *current);
}

void	create_comand_token(t_lexer_params *param)
{
	size_t	len;
	char	*buffer;

	len = 0;
	buffer = NULL;
	while (param->prompt[*(param->i)] && !ft_isspace(param->prompt[*(param->i)])
		&& param->prompt[*(param->i)] != '|'
		&& param->prompt[*(param->i)] != '<'
		&& param->prompt[*(param->i)] != '>')
	{
		len++;
		(*(param->i))++;
	}
	buffer = malloc((len + 1) * sizeof(char));
	if (!buffer)
		return ;
	ft_strncpy(buffer, param->prompt + (*(param->i) - len), len);
	buffer[len] = '\0';
	*(param->current) = create_tok(buffer, STRING);
	add_token(param->head, *(param->current));
	free(buffer);
}

void	double_qoute(t_lexer_params *param)
{
	char	*quote_start;
	char	*quote_end;
	char	*tmp;

	quote_start = &param->prompt[*(param->i)];
	quote_end = ft_strchr(quote_start + 1, 34);
	if (quote_end)
	{
		tmp = handle_quote(quote_start, param);
		if (!tmp)
		{
			write_err(param->msh, 16, NULL);
			return ;
		}
		*(param->current) = create_tok(tmp, DOUBLE_QUOTE);
		add_token(param->head, *(param->current));
		free(tmp);
		*(param->i) = ft_strlen(param->prompt);
	}
	else
	{
		write_err(param->msh, 16, NULL);
		*(param->i) = ft_strlen(param->prompt);
	}
}

void	redirection_less(t_lexer_params *param)
{
	if (param->prompt[*(param->i) + 1] == '<')
	{
		*(param->current) = create_tok("<<", HEREDOC);
		add_token(param->head, *(param->current));
		(*(param->i)) += 2;
	}
	else
	{
		*(param->current) = create_tok("<", REDIRIN);
		add_token(param->head, (*(param->current)));
		(*(param->i))++;
	}
}

// void	print_token(t_lexer *tokens)
// {
// 	char *str;
// 	// printf("ERROR HERE\n");
// 	while (tokens)
// 	{
// 		// printf("ERROR HERE1\n");
// 		if (tokens->type == STRING)
// 		{
// 			// printf("ERROR HERE2\n");
// 		str = "COMMAND";
// 		}
// 		else if (tokens->type == PIPE)
// 		{
// 			// printf("ERROR HERE3\n");
// 			str = "PIPE";
// 		}
// 		else if (tokens->type == REDIROUT)
// 		{
// 			// printf("ERROR HERE4\n");
// 			str = "RIDIRECTION_OUT";
// 		}
// 		else if (tokens->type == REDIRIN)
// 		{
// 			// printf("ERROR HERE4\n");
// 			str = "RIDIRECTION_IN";
// 		}
// 		else if (tokens->type == HEREDOC)
// 		{
// 			// printf("ERROR HERE4\n");
// 			str = "RIDIRECTION_LESS_LESS";
// 		}
// 		else if (tokens->type == APPEND)
// 		{
// 			// printf("ERROR HERE4\n");
// 			str = "RIDIRECTION_GREAT_GREAT";
// 		}
// 		else if (tokens->type == DOUBLE_QUOTE)
// 		{
// 			str = "DOUBLE_QUOTE";
// 		}
// 		else
// 		{
// 			// printf("ERROR HERE5\n");
// 			str = "UNKNOWN";
// 		}
// 		// printf("ERROR HERE666\n");
// 		printf("Token : %s  Type: %s\n", tokens->data, str);
// 		tokens = tokens->next;
// 	}
// }