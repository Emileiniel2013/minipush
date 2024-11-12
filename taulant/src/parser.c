/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 15:09:14 by tndreka           #+#    #+#             */
/*   Updated: 2024/11/12 17:51:56 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/mini_sh.h"

void	minishell_parser(char **prompt, t_msh **msh)
{
	t_lexer	*token;
	t_lexer	*head;
	t_table	*table;
	
	(void)msh;
	token = lexer(*prompt);
	head = token;
	table = NULL;
	if(!token)
		return ;
	while (token)
	{
		token_to_table(&token, &table, msh);
		token = token->next;
	}
	free_token(token);
}

