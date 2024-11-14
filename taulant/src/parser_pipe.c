/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:13:02 by tndreka           #+#    #+#             */
/*   Updated: 2024/11/14 15:04:16 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/mini_sh.h"



int checd_valid(t_lexer *token_lst, t_table *table, t_msh *msh)
{
	if (token_lst->type == PIPE && !table)
	{
		write_err(msh, 11, token_lst->data);
		return (-1);
	}
	else if (token_lst->type == PIPE && !token_lst->next)
	{
		write_err(msh, 10, NULL);
		return (-1);
	}
	else if (token_lst->type == PIPE && token_lst->next->type != COMMAND)
	{
		write_err(msh, 12, token_lst->next->data);
		return (-1);
	}
	return (0);
}

int	trip_to_table_pipe(t_lexer *token_lst, t_table *table, t_msh *msh)
{
	t_table	*new_node;
	t_table	*current_node;

	checd_valid(token_lst, table, msh);
	current_node = table;
	while (current_node && current_node->next)
		current_node = current_node->next;
	new_node = malloc(sizeof(t_table));
	if (!new_node)
	{
		perror("malloc");
		return -1;
	}
	new_node->leftpipe = true;
	new_node->rightpipe = false;
	new_node->next = NULL;
	new_node->command = NULL;
	if (current_node)
	{
		current_node->rightpipe = true;
		current_node->next = new_node;
	}
	else
	{
		table = new_node;
	}
	return (0);
}

// bool	trip_to_table_pipe(t_lexer *token, t_table *table, t_msh *msh)
// {
// 	t_table		*new_node;
// 	t_table		*current;

// 	(void)msh;
// 	check_valid_pipe(token, table);
// 	current = table;
// 	while (current && current->next)
// 		current = current->next;
// 	new_node = malloc(sizeof(t_table));
// 	if (new_node == NULL)
// 		return (false);
// 	new_node->leftpipe = true;
// 	new_node->rightpipe = false;
// 	new_node->next = NULL;
// 	new_node->command = NULL;
// 	if (current)
// 	{
// 		current->rightpipe = true;
// 		current->next = new_node;
// 	}
// 	else
// 		table = new_node;
// 	return (true);
// }

// bool	check_valid_pipe(t_lexer *token, t_table *table)
// {
// 	if (token->type == PIPE && token->next->type != COMMAND)
// 		return (false);
// 	else if (token->type == PIPE && !table)
// 		return (false);
// 	else if (token->type == PIPE && !token->next)
// 		return (false);
// 	return (true);
// }