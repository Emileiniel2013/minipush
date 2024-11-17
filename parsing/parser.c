/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:43:06 by temil-da          #+#    #+#             */
/*   Updated: 2024/11/17 01:43:38 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"
bool	pass_token_to_table(t_lexer **token, t_mini *minish, t_table **table);
//** This function will call the lexer function and pass the tokens to the parser
// void	minishell_parser(char *line, t_mini *minish)
// {
// 	t_lexer		*token;
// 	t_table		*table;
	
// 	table = NULL;
// 	token = lexer(line);
// 	if (!token)
// 		return ;
// 	while (token)
// 	{
// 		if (!pass_token_to_table(&token, minish, &table))
// 		{
// 			free_parser(minish, token, table);
// 			return ;
// 		}
// 		token = token->next;
// 	}
// 	free_tkn_lst(token);
// 	minish->table = table;
// 	minish->table_head = table;
// }
void	minishell_parser(char *prompt, t_mini *msh)
{
	t_lexer	*tkn_lst;
	t_lexer	*lst_head;
	t_table		*table;
	//int			i;

	table = NULL;
	//i = 0;
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
		//i++;
		tkn_lst = tkn_lst->next;
	}
	msh->table = table;
	msh->table_head = table;
}
//======== pAss tO tAblE FuNctiOns =========
//** This function will check the type of the token and call the appropriate
bool	pass_token_to_table(t_lexer **token, t_mini *minish, t_table **table)
{
	bool	result;

	result = false;
	

    if ((*token)->data == NULL)
        return false;

    if ((*token)->type == STRING)
    {
        exp_env_vars(&(*token)->data, minish);
		
    }
	if ((*token)->type == PIPE)
	{
		handle_pipe(token, minish, table);
	}
	add_token_to_table(table, *token);
	return (true);
}


// int	handle_token(t_lexer **tkn_lst, t_mini *minish, t_table **table)
// {
// 	int		i;
// 	t_token	token;

// 	i = 0;
// 	token = (*tkn_lst)->type;
// 	if (token == REDIROUT || token == REDIROUTAPP || token == REDIRIN)
// 		i = check_valid_redir_input(tkn_lst, minish);
// 	else if (token == STRING || token == DOUBLE_QUOTE)
// 		exp_env_vars(&(*tkn_lst)->data, minish);
// 	else if (token == HEREDOC)
// 		i = handle_heredoc(tkn_lst, minish);
// 	else if (token == PIPE)
// 		i = check_valid_pipe(*tkn_lst, *table, minish);
// 	if (i == -1)
// 		return (i);
// 	add_token_to_table(table, *tkn_lst);
// 	return (i);
// }

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

// int	handle_heredoc(t_lexer **token_lst, t_mini *minish)
// {
// 	char	*delimiter;
// 	int		fd;

// 	fd = -1;
// 	if (!(*token_lst)->next)
// 		return (write_err(minish, 13, NULL), -1);
// 	else if ((*token_lst)->next->type != STRING)
// 		return (write_err(minish, 14, (*token_lst)->next->data), -1);
// 	else
// 		(*token_lst)->next->type = DELIMITER;
// 	delimiter = ft_strdup((*token_lst)->next->data);
// 	fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd < 0)
// 		return (write_err(minish, 15, NULL), -1);
// 	while (heredoc_loop(delimiter, fd))
// 		;
// 	minish->in_redir = ft_strdup(".heredoc_tmp");
// 	free(delimiter);
// 	delimiter = NULL;
// 	close(fd);
// 	return (0);
// }

bool put_to_table_pipe(t_table **table, t_lexer **token, t_mini *minish)
{
	t_table *node;
	t_table *current;

	(void)token;
	current = (*table);
	while(current->next)
		current = current->next;
	node = malloc(sizeof(t_table));
	if (!node)
		return (write_err(minish, 6, NULL), false);
	node->leftpipe = false;
	node->rightpipe = true;
	node->command = NULL;
	node->next = NULL;
	if (current)
		current->next = node;
	else
		(*table) = node;
	return (true);
}


bool check_valid_pipe(t_lexer **token, t_table **table, t_mini *minish)
{
	if((*token)->next == NULL)
		return (write_err(minish, 10, NULL), false);
	else if ((*token)->next->type != STRING)
		return (write_err(minish, 11, (*token)->next->data), false);
	else if ((*token)->type == PIPE && !(*table))
		return (write_err(minish, 12, NULL), false);
	return (true);
}

bool handle_pipe(t_lexer **token, t_mini *minish, t_table **table)
{	
	if (!check_valid_pipe(token, table, minish))
		return (false);
	if (!put_to_table_pipe(table, token, minish))
		return (false);
	return (true);
}