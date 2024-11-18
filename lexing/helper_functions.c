/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:13:00 by temil-da          #+#    #+#             */
/*   Updated: 2024/11/18 19:57:28 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lexer.h"

// void	free_token(t_lexer *head)
// {
// 	t_lexer	*tmp;

// 	while (head)
// 	{
// 		tmp = head;
// 		head = head->next;
// 		free(tmp->data);
// 		free(tmp);
// 	}
// 	free(head);
// }
int	ft_isspace(char c)
{
	return (c == ' ');
}

void	free_tkn_lst(t_lexer *token_lst)
{
	t_lexer	*current;
	t_lexer	*next;

	current = token_lst;
	while (current)
	{
		next = current->next;
		if (current->data)
			free(current->data);
		free(current);
		current = next;
	}
}

// char	*handle_single_quote(char *prompt)
// {
// 	char	*end;
// 	char	*string;

// 	end = ft_strchr(prompt, 39);
// 	string = malloc(ft_strlen(prompt));
// 	if (!string)
// 	{
// 		printf("failed to allocate memory for the new string\n");
// 		return (NULL);
// 	}
// 	if (prompt && end)
// 	{
// 		ft_memmove(string, prompt, end - prompt);
// 		string[end - prompt] = '\0';
// 	}
// 	else
// 		return (NULL);
// 	return (string);
// }
	// else
	// {
	// 	(*i)++;
	// 	quote_end = ft_strchr(&prompt[(*i)], 39);
	// 	if(quote_end)
	// 	{
	// 		tmp = handle_single_quote(&prompt[(*i)]);
	// 		(*current) = create_tok(tmp, SINGLE_QUOTE);
	// 		add_token(head, (*current));
	// 		free(tmp);
	// 		(*i) = quote_end - prompt + 1;
	// 	}
	// }

// void	set_quote_type(int *quote_type, char quote)
// {
// 	if (quote == '"')
// 		*quote_type = 2;
// 	else if (quote == '\'')
// 		*quote_type = 1;
// }

// t_tkn	identify_token(char *token, int quote_type)
// {
// 	if (quote_type == 1)
// 		return (SINGLE_QUOTE);
// 	else if (quote_type == 2)
// 		return (DOUBLE_QUOTE);
// 	else if (ft_strncmp(token, "|", ft_strlen(token)) == 0)
// 		return (PIPE);
// 	else if (ft_strncmp(token, "<", ft_strlen(token)) == 0)
// 		return (REDIRIN);
// 	else if (ft_strncmp(token, ">", ft_strlen(token)) == 0)
// 		return (REDIROUT);
// 	else if (ft_strncmp(token, ">>", ft_strlen(token)) == 0)
// 		return (REDIROUTAPP);
// 	else if (ft_strncmp(token, "<<", ft_strlen(token)) == 0)
// 		return (HEREDOC);
// 	else
// 		return (STRING);
// }
