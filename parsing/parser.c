/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:43:06 by temil-da          #+#    #+#             */
/*   Updated: 2024/11/20 16:39:04 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"

void	minishell_parser(char *prompt, t_mini *msh)
{
	t_lexer	*tkn_lst;
	t_lexer	*lst_head;
	t_table	*table;

	table = NULL;
	tkn_lst = lexer(prompt);
	lst_head = tkn_lst;
	if (!tkn_lst)
		return ;
	while (tkn_lst)
	{
		if (!pass_token_to_table(&tkn_lst, msh, &table))
		{
			printf("Error\n");
			free_parser(msh, lst_head, table);
			return ;
		}
		tkn_lst = tkn_lst->next;
	}
	msh->table = table;
	msh->table_head = table;
}

bool	pass_token_to_table(t_lexer **token, t_mini *minish, t_table **table)
{
	bool	res;

	res = true;
	if ((*token)->data == NULL)
		return (false);
	if ((*token)->type == STRING || (*token)->type == DOUBLE_QUOTE
		|| (*token)->type == SINGLE_QUOTE)
	{
		res = exp_env_vars(&(*token)->data, minish);
		add_token_to_table(table, *token);
	}
	else if ((*token)->type == PIPE)
		res = handle_pipe(*token, minish, *table);
	else if ((*token)->type == REDIRIN || (*token)->type == REDIROUT
		|| (*token)->type == REDIROUTAPP)
	{
		res = handle_redir(token, minish, table);
	}
	else if ((*token)->type == HEREDOC)
	{
		res = handle_heredoc(token, minish, table);
		add_token_to_table(table, *token);
	}
	return (true);
}

bool	check_valid_pipe(t_lexer *token, t_table *table, t_mini *minish)
{
	if (token->next == NULL)
		return (write_err(minish, 10, NULL), false);
	else if (token->next->type != STRING)
		return (write_err(minish, 11, token->next->data), false);
	else if (token->type == PIPE && !table)
		return (write_err(minish, 12, NULL), false);
	return (true);
}

bool	handle_pipe(t_lexer *token, t_mini *minish, t_table *table)
{
	t_table	*node;
	t_table	*current;

	check_valid_pipe(token, table, minish);
	current = table;
	while (current && current->next)
		current = current->next;
	node = malloc(sizeof(t_table));
	if (!node)
		return (write_err(minish, 6, NULL), false);
	node->leftpipe = true;
	node->rightpipe = false;
	node->command = NULL;
	node->next = NULL;
	if (current)
	{
		current->rightpipe = true;
		current->next = node;
	}
	else
		table = node;
	return (true);
}

bool	handle_redir(t_lexer **token, t_mini *minish, t_table **table)
{
	if ((*token)->next == NULL)
		return (write_err(minish, 8, NULL), false);
	else if ((*token)->next->type != STRING)
		return (write_err(minish, 9, (*token)->next->data), false);
	if ((*token)->type == REDIRIN)
	{
		if (minish->in_redir)
			free(minish->in_redir);
		minish->in_redir = ft_strdup((*token)->next->data);
	}
	else if ((*token)->type == REDIROUT || (*token)->type == REDIROUTAPP)
	{
		if (minish->out_redir)
			free(minish->out_redir);
		minish->out_redir = ft_strdup((*token)->next->data);
	}
	(*token)->next->type = FILENAME;
	if ((*token)->type == REDIROUTAPP)
		minish->append_mode = true;
	add_redir_to_table(token, table);
	return (true);
}
