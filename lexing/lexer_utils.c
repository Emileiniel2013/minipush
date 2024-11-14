/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 19:45:00 by temil-da          #+#    #+#             */
/*   Updated: 2024/11/14 16:26:47 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lexer.h"


t_lexer	*create_tok(char *data, t_token type)
{
	t_lexer	*token;

	token = malloc(sizeof(t_lexer));
	if (token == NULL)
	{
		perror("Malloc for tokens failed--->create_tok()\n");
		return (NULL);
	}
	token->data = ft_strdup(data);
	token->type = type;
	token->next = NULL;
	return (token);
}

void	add_token(t_lexer **tokens, t_lexer *new_token)
{
	t_lexer	*temp;

	if (!*tokens)
		*tokens = new_token;
	else
	{
		temp = *tokens;
		while (temp->next)
		{
			temp = temp->next;
		}
		temp->next = new_token;
	}
}
char	*ft_strndup(char *s1, size_t len)
{
	size_t	j;
	char	*cpy;

	j = 0;
	cpy = malloc(sizeof(char) * (len + 1));
	if (!cpy)
		return (NULL);
	while (s1[j] && j < len)
	{
		cpy[j] = s1[j];
		j++;
	}
	cpy[j] = '\0';
	return (cpy);
}
char	*ft_strncpy(char *dst, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	if (dst == NULL || src == NULL || n == 0)
		return (dst);
	while (i < n && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dst[i] = '\0';
		i++;
	}
	return (dst);
}
char	*handle_quote(char *prompt)
{
	char	*end;
	char	*string;

	end = ft_strchr(prompt, 34);
	string = malloc(ft_strlen(prompt));
	if (!string)
	{
		perror("failed to allocate memory for the new string\n");
		return (NULL);
	}
	if (prompt && end)
	{
		ft_memmove(string, prompt, end - prompt);
		string[end - prompt] = '\0';
	}
	else
		return (NULL);
	return (string);
}

// bool	ft_isspace(char index)
// {
// 	if (index == ' ')
// 		return (true);
// 	else
// 		return (false);
// }

// void	free_tkn_lst(t_tkn_lst *token_lst)
// {
// 	t_tkn_lst	*current;
// 	t_tkn_lst	*next;

// 	current = token_lst;
// 	while (current)
// 	{
// 		next = current->next;
// 		if (current->content)
// 			free(current->content);
// 		free(current);
// 		current = next;
// 	}
// }

// char	*ft_strndup(char *s1, size_t len)
// {
// 	size_t	j;
// 	char	*cpy;

// 	j = 0;
// 	cpy = malloc(sizeof(char) * (len + 1));
// 	if (!cpy)
// 		return (NULL);
// 	while (s1[j] && j < len)
// 	{
// 		cpy[j] = s1[j];
// 		j++;
// 	}
// 	cpy[j] = '\0';
// 	return (cpy);
// }

// t_tkn_lst	*create_new_node(char *content, t_tkn token)
// {
// 	t_tkn_lst	*new_token;

// 	new_token = malloc(sizeof(t_tkn_lst));
// 	if (!new_token)
// 		return (NULL);
// 	new_token->content = ft_strdup(content);
// 	new_token->tkn = token;
// 	new_token->next = NULL;
// 	return (new_token);
// }
