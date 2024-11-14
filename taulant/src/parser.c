/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 15:09:14 by tndreka           #+#    #+#             */
/*   Updated: 2024/11/14 15:03:47 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/mini_sh.h"

// bool	token_to_table(t_lexer **token, t_table **table, t_msh *msh);
// bool	trip_to_table_pipe(t_lexer *token, t_table *table, t_msh *msh);
// bool	check_valid_pipe(t_lexer *token, t_table *table);


void	minishell_parser(char *prompt, t_msh *msh)
{
	t_lexer		*token;
	t_table		*table;
	int			i;

	table = NULL;
	i = 0;
	token = lexer(prompt);
	if (!token)
		return ;
	while (token)
	{
		i = pass_to_table(&token, msh, &table);
		if (i == -1)
		{
			printf("Error\n");
			//free_parser(msh, lst_head, table);
			return ;
		}
		token = token->next;
	}
	msh->table = table;
	msh->table_head = table;
}

int	pass_to_table(t_lexer **tkn_lst, t_msh *msh, t_table **table)
{
	int		i;
	t_token	token;

	i = 0;
	token = (*tkn_lst)->type;
	if (token == REDIROUT || token == REDIROUTAPP || token == REDIRIN)
		i = handle_type_of_redir(tkn_lst, msh);
	else if (token == COMMAND || token == DOUBLE_QUOTE)
		expand_env_vars(&(*tkn_lst)->data, msh);
	else if (token == HEREDOC)
		i = handle_type_of_redir_type2(tkn_lst, msh);
	else if (token == PIPE)
		i = trip_to_table_pipe(*tkn_lst, *table, msh);
	if (i == -1)
		return (i);
	add_tokens_to_table(table, *tkn_lst);
	return (i);
}
	// t_lexer	*token;
	// t_lexer	*head;
	// t_table	*table;

	// (void)msh;
	// token = lexer(prompt);
	// if (token)
	// {
	// 	print_token(token);
	// }
	// head = token;
	// table = NULL;
	// if (!token)
	// 	return ;
	// while (token)
	// {
	// 	token_to_table(&token, &table, msh);
	// 	printf("here\n");
	// 	token = token->next;
	// }
	// free_token(token);

// bool	token_to_table(t_lexer **token, t_table **table, t_msh *msh)
// {
// 	t_lexer	*temp;

// 	temp = *token;
// 	if (temp->type == PIPE)
// 	{
// 		printf("here2\n");
// 		if (!trip_to_table_pipe(*token, *table, msh))
// 		{
// 			printf("false main table function");
// 			return (false);
// 		}
// 		printf("here3\n");
// 	}
// 	else if (temp->type == COMMAND)
// 	{
// 		printf("here3333333\n");
// 		if (!trip_to_table_commad(*token, *table, msh))
// 		{
// 			printf("false main table function");
// 			return (false);
// 			printf("here444444444\n");
// 		}
// 	}
// 	msh->table = *table;
// 	msh->table_head = *table;
// 	printf("true 444555555555==========!!\n");
// 	return (true);
// }