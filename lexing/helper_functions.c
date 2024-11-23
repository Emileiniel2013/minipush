/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:13:00 by temil-da          #+#    #+#             */
/*   Updated: 2024/11/23 15:53:15 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lexer.h"

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

char	*ft_strndup(const char *s1, size_t len)
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

void	single_qoute(t_lexer_params *param)
{
	char	*quote_start;
	char	*quote_end;
	char	*tmp;

	quote_start = &param->prompt[*(param->i)];
	quote_end = ft_strchr(quote_start + 1, 39);
	if (quote_end)
	{
		tmp = handle_single_quote(quote_start, param);
		if (!tmp)
		{
			write_err(param->msh, 16, NULL);
			return ;
		}
		*(param->current) = create_tok(tmp, SINGLE_QUOTE);
		add_token(param->head, *(param->current));
		free(tmp);
		//*(param->i) = ft_strlen(param->prompt);
		*(param->i) += (quote_end - quote_start + 1);
	}
	else
	{
		write_err(param->msh, 16, NULL);
		*(param->i) = ft_strlen(param->prompt);
	}
}
