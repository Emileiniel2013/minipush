/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:32:53 by temil-da          #+#    #+#             */
/*   Updated: 2024/11/20 16:05:30 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H

# define PARSER_H

# include "lexer.h"
# include <stdbool.h>
# include <fcntl.h>

typedef struct s_dollar_param
{
	char	**content;
	int		*i;
	int		*last_pos;
	char	**str;
	t_mini	*msh;
}	t_dollar_param;

// MAIN PARSING FUNCTIONS
void	minishell_parser(char *prompt, t_mini *msh);
bool	pass_token_to_table(t_lexer **token, t_mini *minish, t_table **table);
bool	exp_env_vars(char **content, t_mini *msh);
char	*handle_content(char **content, t_mini *msh);
void	check_dollar(t_dollar_param *param);
void	free_and_assign(char **dst, char *src);
char	*expand_var(t_mini *msh, const char *content, int *i);
char	*ft_get_env_value(t_mini *msh, const char *var_name);
bool	check_valid_pipe(t_lexer *token, t_table *table, t_mini *minish);
bool	handle_pipe(t_lexer *token, t_mini *minish, t_table *table);
bool	handle_redir(t_lexer **token, t_mini *minish, t_table **table);
void	add_redir_to_table(t_lexer **token, t_table **table);
bool	handle_heredoc(t_lexer **token, t_mini *msh, t_table **table);
void	write_line (int fd, char *line);
bool	check_valid_heredoc(t_lexer **token, t_mini *msh, char **separator);
bool	open_the_fd(int *fd, t_mini *msh, char *separator);

// PARSING UTILS
int		ft_strcmp(const char *s1, char *s2);
char	*ft_getenv(t_mini *minish, const char	*env);
char	**copy_env(char **envp);

// STRUCT CREATION
void	add_token_to_table(t_table **table, t_lexer *token_lst);
void	create_table(t_table **table, bool leftpipe);
void	create_cmd_table(t_table **table, char *content);

// FREEING 
void	free_table(t_mini *minish);
void	free_cmd(t_cmd *cmd);
void	free_parser(t_mini *minish, t_lexer *lst, t_table *table);
void	ft_free(char **content);

#endif