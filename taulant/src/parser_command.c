/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:30:21 by tndreka           #+#    #+#             */
/*   Updated: 2024/11/13 20:02:29 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/mini_sh.h"

bool	trip_to_table_commad(t_lexer *token, t_table *table, t_msh *msh)
{
	t_lexer		*temp;
	t_table		*new_node;
	int			cmd_len;

	(void)msh;
	temp = token;
	if (!count_allocate_commands(&temp, &new_node, &cmd_len))
	{
		return(false);
	}
	if (!add_command_in_table(new_node, table, cmd_len, token))
	{
		return(false);
	}
	return (true);
}

bool	count_allocate_commands(t_lexer **temp, t_table **new_node,
					int *cmd_len)
{
	*cmd_len = 0;
	while ((*temp) && (*temp)->type == COMMAND)
	{
		(*cmd_len)++;
		(*temp) = (*temp)->next;
	}
	(*new_node) = malloc(sizeof(t_table));
	if (!(*new_node))
		return (false);
	(*new_node)->command = malloc(sizeof(char *) * ((*cmd_len) + 1));
	if (!(*new_node)->command)
	{
		free(new_node);
		return (false);
	}
	(*new_node)->leftpipe = false;
	(*new_node)->cmd_head = NULL;
	return (true);
}

bool	add_command_in_table(t_table *new_node, t_table *table, int cmd_len, t_lexer *token)
{
	t_lexer		*tempo;
	t_table		*current;
	int			i;

	i = 0;
	tempo = token;
	while (tempo->data && i < cmd_len)
	{
		new_node->command[i].content = ft_strdup(tempo->data);
		new_node->command[i].next = NULL;
		tempo = tempo->next;
		i++;
	}
	new_node->command[i].content = NULL;
	if (table)
	{
		current = table;
		while (current->next)
		{
			current = current->next;
		}
		current->next = new_node;
	}
	else
		table = new_node;
	return (true);
}
