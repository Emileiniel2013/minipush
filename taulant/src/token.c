/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 17:41:30 by tndreka           #+#    #+#             */
/*   Updated: 2024/11/12 17:30:09 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/mini_sh.h"

t_lexer	*lexer(char *prompt)
{
	t_lexer	*head;
	t_lexer	*current;
	int		i;

	head = NULL;
	current = NULL;
	i = 0;
	while (prompt[i])
	{
		while (prompt[i] && ft_isspace(prompt[i]))
			i++;
		if (prompt[i] == '|')
		{
			current = create_tok("|", PIPE);
			add_token(&head, current);
			i++;
		}
		else if (prompt[i] == '\"')
			double_qoute(prompt, &head, &current, &i);
		else if (prompt[i] == '>')
			redirection(prompt, current, &head, &i);
		else if (prompt[i] == '<')
			redirection_less(prompt, &head, &current, &i);
		else if (prompt[i])
			create_comand_token(prompt, &head, &current, &i);
	}
	return (head);
}

void	create_comand_token(char *prompt, t_lexer **head, t_lexer **current,
		int *i)
{
	size_t	len;
	char	*buffer;

	len = 0;
	buffer = NULL;
	while (prompt[(*i)] && !ft_isspace(prompt[(*i)]) && prompt[(*i)] != '|'
		&& prompt[(*i)] != '<' && prompt[(*i)] != '>')
	{
		len++;
		(*i)++;
	}
	buffer = malloc((len + 1) * sizeof(char));
	if (!buffer)
		return ;
	ft_strncpy(buffer, prompt + ((*i) - len), len);
	buffer[len] = '\0';
	(*current) = create_tok(buffer, COMMAND);
	add_token(head, (*current));
	// free(current);
	free(buffer);
}

void	double_qoute(char *prompt, t_lexer **head, t_lexer **current, int *i)
{
	i++;
	char	*quote_end;
	char	*tmp;

	quote_end = ft_strchr((&prompt[(*i)]), '\"');
	if (quote_end)
	{
		tmp = handle_quote(&prompt[(*i)]);
		(*current) = create_tok(tmp, DOUBLE_QUOTE);
		add_token(head, (*current));
		free(tmp);
		(*i) = quote_end - prompt + 1;
	}
}

void	redirection_less(const char *prompt, t_lexer **head, t_lexer **current,
		int *i)
{
	//char *redir_arr;
	if (prompt[(*i) + 1] == '<')
	{
		//redir_arr = create_redir_arr(prompt[(*i)]);
		(*current) = create_tok("<<", HEREDOC);
		add_token(head, (*current));
		//free(redir_arr);
		(*i) += 2;
	}
	else
	{
		//redir_arr = create_redir_arr(prompt[(*i)]);
		(*current) = create_tok("<", REDIRIN);
		add_token(head, (*current));
		//free(redir_arr);
		(*i)++;
	}
}

void	redirection(const char *prompt, t_lexer *current, t_lexer **head,
		int *i)
{
	//char *redir_arr;
	if (prompt[(*i) + 1] == '>')
	{
		//redir_arr = create_redir_arr(prompt[(*i)]);
		current = create_tok(">>", APPEND);
		add_token(head, current);
		//free(redir_arr);
		(*i) += 2;
	}
	else
	{
		//redir_arr = create_redir_arr(prompt[(*i)]);
		current = create_tok(">", REDIROUT);
		add_token(head, current);
		//free(redir_arr);
		(*i)++;
	}
}

void	free_token(t_lexer *head)
{
	t_lexer	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->data);
		free(tmp);
	}
	free(head);
}
