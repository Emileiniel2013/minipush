/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_creation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 17:16:25 by temil-da          #+#    #+#             */
/*   Updated: 2024/12/04 21:42:44 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"

bool	add_token_to_table(t_table **table, t_tkn_lst *token_lst)
{
	t_table	*current_node;

	current_node = NULL;
	if (token_lst->tkn == STRING || token_lst->tkn == DOUBLE_QUOTE
		|| token_lst->tkn == SINGLE_QUOTE)
	{
		if (!(*table))
		{
			*table = malloc(sizeof(t_table));
			if (!(*table))
			{
				write(STDERR_FILENO, "Memory allocation fail!\n", 25);
				return (false);
			}
			(*table)->command = NULL;
			(*table)->cmd_head = NULL;
			(*table)->next = NULL;
		}
		current_node = *table;
		while (current_node->next)
			current_node = current_node->next;
		if (!create_cmd_table(&current_node, token_lst->content))
			return (false);
	}
	return (true);
}

bool	create_cmd_table(t_table **table, char *content)
{
	t_cmd	*new_node;
	t_cmd	*current_node;

	new_node = NULL;
	current_node = NULL;
	new_node = malloc(sizeof(t_cmd));
	if (!new_node)
		return (write(STDERR_FILENO, "Memory allocation fail!\n", 25), false);
	new_node->content = ft_strdup(content);
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
	return (true);
}

void	add_redir_to_table(t_tkn_lst **token, t_table **table)
{
	t_table	*current_node;

	current_node = NULL;
	if ((*token)->tkn == STRING || (*token)->tkn == DOUBLE_QUOTE
		|| (*token)->tkn == SINGLE_QUOTE)
	{
		if (!(*table))
			*table = malloc(sizeof(t_table));
		current_node = *table;
		while (current_node->next)
			current_node = current_node->next;
		create_cmd_table(&current_node, (*token)->content);
	}
}

int	creat_close_file(t_mini *minish)
{
	int	fd;

	fd = -1;
	fd = open(minish->out_redir, O_CREAT, 0644);
	if (fd < 0)
		return (write_err(minish, 7, NULL), -1);
	close(fd);
	free(minish->out_redir);
	return (0);
}
