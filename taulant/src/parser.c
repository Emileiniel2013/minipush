/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 15:09:14 by tndreka           #+#    #+#             */
/*   Updated: 2024/11/12 19:30:04 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/mini_sh.h"

bool	token_to_table(t_lexer **token, t_table **table, t_msh *msh);
bool	trip_to_table_pipe(t_lexer *token, t_table *table, t_msh *msh);

void	minishell_parser(char *prompt, t_msh *msh)
{
	t_lexer	*token;
	t_lexer	*head;
	t_table	*table;

	(void)msh;
	token = lexer(prompt);
	head = token;
	table = NULL;
	if (!token)
		return ;
	while (token)
	{
		token_to_table(&token, &table, msh);
		if (table)
		{
			printf("Table data: %s\n", table->command->content);
		}
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
		trip_to_table_pipe(*token, *table, msh);
		if (!*token || !*table)
			return (false);
	}
	else if (temp->type == COMMAND)
	{
		
	}
	return (true);
}

bool	trip_to_table_pipe(t_lexer *token, t_table *table, t_msh *msh)
{
	t_table		*new_node;
	t_table		*current;

	(void)msh;
	check_valid_pipe(token, table);
	current = table;
	while (current && current->next)
		current = current->next;
	new_node = malloc(sizeof(t_table));
	if (new_node == NULL)
		return (false);
	new_node->leftpipe = true;
	new_node->rightpipe = false;
	new_node->next = NULL;
	new_node->command = NULL;
	if (current)
	{
		current->rightpipe = true;
		current->next = new_node;
	}
	else
		table = new_node;
	return (true);
}

bool	check_valid_pipe(t_lexer *token, t_table *table)
{
	if (token->type == PIPE && token->next->type != COMMAND)
		return (false);
	else if (token->type == PIPE && !table)
		return (false);
	else if (token->type == PIPE && !token->next)
		return (false);
	return (true);
}
