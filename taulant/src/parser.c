/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 15:09:14 by tndreka           #+#    #+#             */
/*   Updated: 2024/11/13 16:56:46 by tndreka          ###   ########.fr       */
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

bool	trip_to_table_command(t_lexer *token, t_table *table)
{
	t_lexer		*temp;
	t_table		*current;
	t_table		*new_node;
	int			cmd_len;

	temp = token;
	count_allocate_commands(&temp, &cmd_len, &new_node);
	add_commad_in_table(&new_node, &current, &table, &cmd_len);
}

bool	count_allocate_commands(t_lexer **temp, t_table **new_node, int *cmd_len)
{
	cmd_len = 0;
	while ((*temp) && (*temp)->type == COMMAND)
	{
	 	cmd_len++;
	 	(*temp) = (*temp)->next;
	}
	new_node = malloc(sizeof(t_table));
	if (!new_node)
		return (false);
	(*new_node)->command = malloc(sizeof(char *) * (cmd_len + 1));
	if (!(*new_node)->command)
	{
		free(new_node);
		return (false);
	}
}

bool	add_command_in_table(t_table **new_node, t_table **current, int *cmd_len,
						t_table **table)
{
	t_lexer		*tempo;
	int			i;

	i = 0;
	while(i < cmd_len)
	{
		(*new_node)->command[i] =	
	}
	
}