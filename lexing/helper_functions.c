/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:13:00 by temil-da          #+#    #+#             */
/*   Updated: 2024/11/14 16:22:53 by tndreka          ###   ########.fr       */
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

void	print_token(t_lexer *tokens)
{
	char *str;
	// printf("ERROR HERE\n");
	while (tokens)
	{
		// printf("ERROR HERE1\n");
		if (tokens->type == COMMAND)
		{
			// printf("ERROR HERE2\n");
		str = "COMMAND";
		}
		else if (tokens->type == PIPE)
		{
			// printf("ERROR HERE3\n");
			str = "PIPE";
		}
		else if (tokens->type == REDIROUT)
		{
			// printf("ERROR HERE4\n");
			str = "RIDIRECTION_OUT";
		}
		else if (tokens->type == REDIRIN)
		{
			// printf("ERROR HERE4\n");
			str = "RIDIRECTION_IN";
		}
		else if (tokens->type == HEREDOC)
		{
			// printf("ERROR HERE4\n");
			str = "RIDIRECTION_LESS_LESS";
		}
		else if (tokens->type == APPEND)
		{
			// printf("ERROR HERE4\n");
			str = "RIDIRECTION_GREAT_GREAT";
		}
		else
		{
			// printf("ERROR HERE5\n");
			str = "UNKNOWN";
		}
		// printf("ERROR HERE666\n");
		printf("Token : %s  Type: %s\n", tokens->data, str);
		tokens = tokens->next;
	}
}

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
