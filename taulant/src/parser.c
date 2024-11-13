/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 15:09:14 by tndreka           #+#    #+#             */
/*   Updated: 2024/11/13 17:32:08 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/mini_sh.h"

bool	token_to_table(t_lexer **token, t_table **table, t_msh *msh);
bool	trip_to_table_pipe(t_lexer *token, t_table *table, t_msh *msh);
bool	check_valid_pipe(t_lexer *token, t_table *table);

void	minishell_parser(char *prompt, t_msh *msh)
{
	t_lexer	*token;
	t_lexer	*head;
	t_table	*table;

	(void)msh;
	token = lexer(prompt);
	if (token)
	{
		print_token(token);
	}
	head = token;
	table = NULL;
	if (!token)
		return ;
	while (token)
	{
		token_to_table(&token, &table, msh);
		printf("here");
		token = token->next;
	}
	free_token(token);
}

bool	token_to_table(t_lexer **token, t_table **table, t_msh *msh)
{
	t_lexer	*temp;

	temp = *token;
	if (temp->type == PIPE)
	{
		//printf("here2");
		if (!trip_to_table_pipe(*token, *table, msh))
		{
			printf("false main table function");
			return (false);
		}
		//printf("here3");
	}
	else if (temp->type == COMMAND)
	{
		if (!trip_to_table_commad(*token, *table, msh))
		{
			printf("false main table function");
			return (false);
		}
	}
	msh->table = *table;
	msh->table_head = *table;
	printf("true !!");
	return (true);
}
