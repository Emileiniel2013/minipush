/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_token_to_table.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:50:26 by tndreka           #+#    #+#             */
/*   Updated: 2024/11/14 15:02:21 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/mini_sh.h"

void	add_tokens_to_table(t_table **table, t_lexer *token_lst);
void	create_table(t_table **table, bool leftpipe);
void	create_cmd_table(t_table **table, char *data);

void	add_tokens_to_table(t_table **table, t_lexer *token_lst)
{
	t_table	*new_node;
	t_table	*current_node;
	t_token	token;

	new_node = NULL;
	current_node = NULL;
	token = token_lst->type;
	if (token == COMMAND || token == DOUBLE_QUOTE || token == SINGLE_QUOTE)
	{
		if (!(*table))
			create_table(table, false);
		current_node = *table;
		while (current_node->next)
			current_node = current_node->next;
		create_cmd_table(&current_node, token_lst->data);
	}
	// else if (token_lst->type == PIPE)
	// {
	// 	while (current_node->next)
	// 		current_node = current_node->next;
	// 	current_node->rightpipe = true;
	// 	create_table(&new_node, true);
	// 	current_node->next = new_node;
}

void	create_table(t_table **table, bool leftpipe)
{
	*table = malloc(sizeof(t_table));
	if (!(*table))
		return ;
	if (leftpipe == true)
		(*table)->leftpipe = true;
	else
		(*table)->leftpipe = false;
	(*table)->rightpipe = false;
	(*table)->next = NULL;
	(*table)->command = NULL;
}


void	create_cmd_table(t_table **table, char *data)
{
	t_cmd	*new_node;
	t_cmd	*current_node;

	new_node = NULL;
	current_node = NULL;
	new_node = malloc(sizeof(t_cmd));
	if (!new_node)
		return ;
	new_node->content = ft_strdup(data);
	new_node->next = NULL;
	if (!(*table)->command)
	{
		(*table)->command = new_node;
		(*table)->cmd_head = new_node;
	}
	else
	{
		current_node = (*table)->command;
		while (current_node->next != NULL)
			current_node = current_node->next;
		current_node->next = new_node;
	}
}
