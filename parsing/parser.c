/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:43:06 by temil-da          #+#    #+#             */
/*   Updated: 2024/11/14 16:41:32 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"


void	minishell_parser(char *line, t_mini *minish)
{
	t_lexer	*tkn_lst;
	t_lexer	*lst_head;
	t_table		*table;
	int			i;

	table = NULL;
	i = 0;
	tkn_lst = lexer(line);
	lst_head = tkn_lst;
	if (!tkn_lst)
		return ;
	while (tkn_lst)
	{
		i = handle_token(&tkn_lst, minish, &table);
		if (i == -1)
		{
			free_parser(minish, lst_head, table);
			return ;
		}
		tkn_lst = tkn_lst->next;
	}
	free_tkn_lst(lst_head);
	minish->table = table;
	minish->table_head = table;
}

int	handle_token(t_lexer **tkn_lst, t_mini *minish, t_table **table)
{
	int		i;
	t_token	token;

	i = 0;
	token = (*tkn_lst)->type;
	if (token == REDIROUT || token == REDIROUTAPP || token == REDIRIN)
		i = check_valid_redir_input(tkn_lst, minish);
	else if (token == STRING || token == DOUBLE_QUOTE)
		expand_env_vars(&(*tkn_lst)->data, minish);
	else if (token == HEREDOC)
		i = handle_heredoc(tkn_lst, minish);
	else if (token == PIPE)
		i = check_valid_pipe(*tkn_lst, *table, minish);
	if (i == -1)
		return (i);
	add_token_to_table(table, *tkn_lst);
	return (i);
}

int	check_valid_redir_input(t_lexer **token_lst, t_mini *minish)
{
	int	fd;

	fd = 0;
	if ((*token_lst)->next == NULL)
		return (write_err(minish, 8, NULL), -1);
	else if ((*token_lst)->next->type != STRING)
		return (write_err(minish, 9, (*token_lst)->next->data), -1);
	if ((*token_lst)->type == REDIRIN)
	{
		if (minish->in_redir)
			free(minish->in_redir);
		minish->in_redir = ft_strdup((*token_lst)->next->data);
	}
	else if ((*token_lst)->type == REDIROUT || (*token_lst)->type == REDIROUTAPP)
	{
		if (minish->out_redir)
		{
			fd = open(minish->out_redir, O_CREAT, 0644);
			if (fd < 0)
				return (write_err(minish, 7, NULL), -1);
			close(fd);
			free(minish->out_redir);
		}
		minish->out_redir = ft_strdup((*token_lst)->next->data);
	}
	(*token_lst)->next->type = FILENAME;
	if ((*token_lst)->type == REDIROUTAPP)
		minish->append_mode = true;
	return (0);
}

int	handle_heredoc(t_lexer **token_lst, t_mini *minish)
{
	char	*delimiter;
	int		fd;

	fd = -1;
	if (!(*token_lst)->next)
		return (write_err(minish, 13, NULL), -1);
	else if ((*token_lst)->next->type != STRING)
		return (write_err(minish, 14, (*token_lst)->next->data), -1);
	else
		(*token_lst)->next->type = DELIMITER;
	delimiter = ft_strdup((*token_lst)->next->data);
	fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (write_err(minish, 15, NULL), -1);
	while (heredoc_loop(delimiter, fd))
		;
	minish->in_redir = ft_strdup(".heredoc_tmp");
	free(delimiter);
	delimiter = NULL;
	close(fd);
	return (0);
}

int	check_valid_pipe(t_lexer *token_lst, t_table *table, t_mini *minish)
{
	if (token_lst->type == PIPE && !table)
	{
		write_err(minish, 11, token_lst->data);
		return (-1);
	}
	else if (token_lst->type == PIPE && !token_lst->next)
	{
		write_err(minish, 10, NULL);
		return (-1);
	}
	else if (token_lst->type == PIPE && token_lst->next->type != STRING)
	{
		write_err(minish, 12, token_lst->next->data);
		return (-1);
	}
	return (0);
}
