/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:43:06 by temil-da          #+#    #+#             */
/*   Updated: 2024/11/18 15:17:47 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"
bool	pass_token_to_table(t_lexer **token, t_mini *minish, t_table **table);
bool	handle_redir(t_lexer **token, t_mini *minish, t_table **table);
void add_redir_to_table(t_lexer **token, t_table **table);
//** This function will call the lexer function and pass the tokens to the parser
void	minishell_parser(char *prompt, t_mini *msh)
{
	t_lexer		*tkn_lst;
	t_lexer		*lst_head;
	t_table		*table;

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
//======== pAss tO tAblE FuNctiOns =========
//** This function will check the type of the token and call the appropriate
bool	pass_token_to_table(t_lexer **token, t_mini *minish, t_table **table)
{	
	bool res;

	res = true;
    if ((*token)->data == NULL)
        return false;
    if ((*token)->type == STRING || (*token)->type == DOUBLE_QUOTE || (*token)->type == SINGLE_QUOTE)
    {
        res = exp_env_vars(&(*token)->data, minish);
		add_token_to_table(table, *token);	
    }
	else if ((*token)->type == PIPE)
		res = handle_pipe(*token, minish, *table);
	else if ((*token)->type == REDIRIN || (*token)->type == REDIROUT || (*token)->type == REDIROUTAPP)
    {
        res = handle_redir(token, minish, table);
    }
	return (true);
}

//==============================================================================

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
//============================================================================


//================== PIPE =================================================
bool check_valid_pipe(t_lexer *token, t_table *table, t_mini *minish)
{
	if(token->next == NULL)
		return (write_err(minish, 10, NULL), false);
	else if (token->next->type != STRING)
		return (write_err(minish, 11, token->next->data), false);
	else if (token->type == PIPE && !table)
		return (write_err(minish, 12, NULL), false);
	return (true);
}

bool handle_pipe(t_lexer *token, t_mini *minish, t_table *table)
{	
	t_table *node;
	t_table *current;
	
	check_valid_pipe(token, table, minish);
	current = table;
	while(current && current->next)
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
//===================================================================================

//=============================== COMMANF & ENV_VAR =================================

bool exp_env_vars(char **content, t_mini *msh)
{
	char		*exp_string;
	while (1)
	{
		exp_string = handle_content(content, msh);
		if(ft_strcmp(*content, exp_string) == 0)
		{
			free(exp_string);
			break;
		}
		free_and_assign(content, exp_string);
	}
	return(true);
}

char *handle_content(char **content, t_mini *msh)
{
	char	*str;
	int		i;
	int		last_pos;
	char 	*tmp;
	t_dollar_param param;
	
	i = 0;
	last_pos = 0;
	str = ft_strdup("");
	param.content = content;
	param.content = &i;
	param.last_pos = &last_pos;
	param.str = &str;
	param.msh = msh;
	while ((*content)[i])
	{
		if ((*content)[i] == '$')
			check_dollar(&param);
		else
			i++;
	}
	if(last_pos < i)
	{
		tmp = ft_strdup((*content) + last_pos);
		free_and_assign(&str, ft_strjoin(str, tmp));
		free(tmp);
	}
	return (str);
}

//===================================================================================

//========================== REDIRECTIONS ===========================================
bool handle_redir(t_lexer **token, t_mini *minish, t_table **table)
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

void add_redir_to_table(t_lexer **token, t_table **table)
{
	t_table	*new_node;
	t_table	*current_node;
	
	new_node = NULL;
	current_node = NULL;
	if ((*token)->type == STRING || (*token)->type == DOUBLE_QUOTE || (*token)->type == SINGLE_QUOTE)
	{
		if (!(*table))
			create_table(table, false);
		current_node = *table;
		while (current_node->next)
			current_node = current_node->next;
		create_cmd_table(&current_node, (*token)->data);
	}
}
// ==================================================================================

//======================== HEREDOC ==================================================
