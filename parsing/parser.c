/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:43:06 by temil-da          #+#    #+#             */
/*   Updated: 2024/11/26 18:23:19 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"

void	minishell_parser(char *prompt, t_mini *msh)
{
	t_tkn_lst	*tkn_lst;
	t_tkn_lst	*lst_head;
	t_table		*table;

	table = NULL;
	tkn_lst = process_input(prompt, msh);
	lst_head = tkn_lst;
	if (!tkn_lst)
		return ;
	while (tkn_lst)
	{
		if (!pass_token_to_table(&tkn_lst, msh, &table))
		{
			free_parser(msh, lst_head, table);
			return ;
		}
		tkn_lst = tkn_lst->next;
	}
	msh->table = table;
	msh->table_head = table;
}

bool	pass_token_to_table(t_tkn_lst **token, t_mini *minish, t_table **table)
{
	if ((*token)->tkn == STRING || (*token)->tkn == DOUBLE_QUOTE)
	{
		if (!exp_env_vars(&(*token)->content, minish))
			return (false);
	}
	else if ((*token)->tkn == PIPE)
	{
		if (!handle_pipe(*token, minish, *table))
			return (false);
	}
	else if ((*token)->tkn == REDIRIN || (*token)->tkn == REDIROUT
		|| (*token)->tkn == REDIROUTAPP)
	{
		if (!handle_redir(token, minish, table))
			return (false);
	}
	else if ((*token)->tkn == HEREDOC)
	{
		if (!handle_heredoc(token, minish, table))
			return (false);
	}
	else if ((*token)->tkn == SINGLE_QUOTE)
		add_token_to_table(table, (*token));
	add_token_to_table(table, *token);
	return (true);
}

bool	check_valid_pipe(t_tkn_lst *token, t_table *table, t_mini *minish)
{
	if (token->next == NULL)
		return (write_err(minish, 10, NULL), false);
	else if (token->next->tkn != STRING)
		return (write_err(minish, 11, token->next->content), false);
	else if (token->tkn == PIPE && !table)
		return (write_err(minish, 12, NULL), false);
	return (true);
}

bool	handle_pipe(t_tkn_lst *token, t_mini *minish, t_table *table)
{
	t_table	*node;
	t_table	*current;

	if (!check_valid_pipe(token, table, minish))
		return (false);
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

bool	handle_redir(t_tkn_lst **token, t_mini *minish, t_table **table)
{
	if ((*token)->next == NULL)
		return (write_err(minish, 8, NULL), false);
	else if ((*token)->next->tkn != STRING)
		return (write_err(minish, 9, (*token)->next->content), false);
	if ((*token)->tkn == REDIRIN)
	{
		if (minish->in_redir)
			free(minish->in_redir);
		minish->in_redir = ft_strdup((*token)->next->content);
	}
	else if ((*token)->tkn == REDIROUT || (*token)->tkn == REDIROUTAPP)
	{
		if (minish->out_redir)
			if (creat_close_file(minish) == -1)
				return (false);
		minish->out_redir = ft_strdup((*token)->next->content);
	}
	(*token)->next->tkn = FILENAME;
	if ((*token)->tkn == REDIROUTAPP)
		minish->append_mode = true;
	add_redir_to_table(token, table);
	return (true);
}
