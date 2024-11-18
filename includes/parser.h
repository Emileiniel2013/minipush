/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:32:53 by temil-da          #+#    #+#             */
/*   Updated: 2024/11/18 15:09:14 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H

# define PARSER_H

# include "lexer.h"
# include <stdbool.h>
# include <fcntl.h>

typedef struct s_dollar_param
{
	char **content;
	int *i;
	int *last_pos;
	char **str;
	t_msh *msh;
}	t_dollar_param;
// MAIN PARSING FUNCTIONS
void	minishell_parser(char *prompt, t_mini *msh);
bool	pass_token_to_table(t_lexer **token, t_mini *minish, t_table **table);
bool 	exp_env_vars(char **content, t_mini *msh);
//int		handle_token(t_lexer **tkn_lst, t_mini *minish, t_table **table);
//int		check_valid_redir_input(t_lexer **token_lst, t_mini *minish);
int		handle_heredoc(t_lexer **token_lst, t_mini *minish);
bool	heredoc_loop(char *delimiter, int fd);
//int		check_valid_pipe(t_lexer *tkn_lst, t_table *table, t_mini *minish);
//bool 	exp_env_vars(char **content, t_mini *msh);
// char	*check_string(char **content, t_mini *minish);
// void	exp_env_vars(char **content, t_mini *minish);
// char	*check_and_expand_env(char **content, int *i, t_mini *minish);

void	create_table(t_table **table, bool leftpipe);
void	create_cmd_table(t_table **table, char *content);


bool handle_pipe(t_lexer *token, t_mini *minish, t_table *table);
bool put_to_table_pipe(t_table **table, t_lexer **token, t_mini *minish);
bool check_valid_pipe(t_lexer *token, t_table *table, t_mini *minish);
// PARSING UTILS
int		ft_strcmp(char *s1, char *s2);
char	*ft_getenv(t_mini *minish, char	*env);
char	**copy_env(char **envp);
void	append_remainder(char **expanded_string, char **content, int pos);
void	replace_varname_wtih_var(char **expanded_string, char **temp);

// STRUCT CREATION
 void	add_token_to_table(t_table **table, t_lexer *token_lst);
// void	add_cmd_node(t_table **table, char *content);
// void	allocate_table(t_table **table, bool leftpipe);

// FREEING 
void	free_table(t_mini *minish);
void	free_cmd(t_cmd *cmd);
void	free_parser(t_mini *minish, t_lexer *lst, t_table *table);
void	ft_free(char **content);

#endif


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
