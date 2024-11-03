/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 17:41:30 by tndreka           #+#    #+#             */
/*   Updated: 2024/10/05 15:53:26 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/mini_sh.h"

void redirection(const char *prompt, t_lexer *current, t_lexer **head, int *i);

void redirection_less(const char *prompt, t_lexer **head, t_lexer **current, int *i);

void double_qoute(char *quote_end, t_lexer **head, t_lexer **current, int *i);

/*
	lexer()
	t
*/

t_lexer *lexer(char *prompt)
{
	char *buffer;
	t_lexer *head = NULL;
	t_lexer *current = NULL;
	size_t len = 0;

    char *tmp;
    char *quote_end;
    int i = 0;
    int quote_len = 0;

	while (prompt[i])
	{
		while (prompt[i] && ft_isspace(prompt[i]))
			i++;
		if(prompt[i] == '|')
		{
			current = create_tok("|", TOKEN_PIPE);
			add_token(&head, current);
			i++;
		}
        else if (prompt[i] =='\"')
        {
            double_qoute(quote_end, &head, &current, &i);
        }
		else if (prompt[i] == '>')
		{
            redirection(prompt, current, &head, &i);
        }
		else if (prompt[i] == '<')
		{
            redirection_less(prompt, &head, &current, &i);
        }
		else if (prompt[i])
		{
			len = 0;
			while (prompt[i] && !ft_isspace(prompt[i]) && prompt[i] != '|' && prompt[i] != '<' && prompt[i] != '<')
			{
				len++;
				i++;
			}
			buffer = malloc((len + 1) * sizeof(char));
			if(!buffer)
			{
				perror("malloc for buffer at lexer() FAILED !!");
				return NULL;
			
			}
			ft_strncpy(buffer, prompt + (i - len) , len);
			buffer[len] = '\0';
			current = create_tok(buffer, TOKEN_WORD);
			add_token(&head, current);
			free(buffer);
		}
	}
	return head;	
}

void double_qoute(char *quote_end, t_lexer **head, t_lexer **current, int *i) {
    int quote_len;
    quote_end = ft_strchr((&prompt[(*i)]) , '\"');
    if (quote_end)
    {
        quote_len = quote_end - (prompt + (*i) - 1);
        tmp = handle_quote(&prompt[(*i)]);
        (*current) = create_tok("\"", TOKEN_DOUBLE_QUOTE);
        add_token(head, (*current));
        free(tmp);
        (*i) = quote_end - prompt + 1;
    }
}

void redirection_less(const char *prompt, t_lexer **head, t_lexer **current, int *i) {
    if (prompt[(*i) + 1] == '<')
    {
        (*current) = create_tok(create_redir_arr(prompt[(*i)]), TOKEN_RIDIRECTION_LESS_LESS);
        add_token(head, (*current));
        (*i) += 2;
    }
    else
    {
        (*current) = create_tok(create_redir_arr(prompt[(*i)]), TOKEN_RIDIRECTION_LESS);
        add_token(head, (*current));
        (*i)++;
    }
}

void redirection(const char *prompt, t_lexer *current, t_lexer **head, int *i) {
    if (prompt[(*i) + 1] == '>')
    {
        current = create_tok(create_redir_arr(prompt[(*i)]), TOKEN_RIDIRECTION_GREAT_GREAT);
        add_token(head, current);
        (*i) += 2;
    }
    else
    {
        current = create_tok(create_redir_arr(prompt[(*i)]), TOKEN_RIDIRECTION_GREAT);
        add_token(head, current);
        (*i)++;
    }
}

void free_token(t_lexer *head)
{
	t_lexer *tmp;
	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->data);
		free(tmp);
	}
}