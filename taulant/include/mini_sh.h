/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_sh.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 10:48:37 by tndreka           #+#    #+#             */
/*   Updated: 2024/11/13 20:19:33 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_SH_H
# define MINI_SH_H

//============ HEADER FILES =============
# include "libft/libft.h"
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <curses.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <fcntl.h>

# define EXIT_SUCCESS 	0
# define EXIT_FAILURE 	1

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10000
# endif

//=========== STRUCTS =====================

typedef enum e_token
{
	COMMAND,
	FILENAME,
	ARGUMENT,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	PIPE,
	REDIRIN,
	REDIROUT,
	REDIROUTAPP,
	HEREDOC,
	DELIMITER,
	STRING,
	APPEND,
	UNKNOWN
}	t_token;

typedef struct s_lexer
{
	char				*data;
	t_token				type;
	struct s_lexer		*next;
}	t_lexer;

typedef struct t_simple_command
{
	char						*content;
	struct t_simple_command		*next;
}	t_cmd;

typedef struct t_command_table
{
	bool						leftpipe;
	bool						rightpipe;
	t_cmd						*command;
	t_cmd						*cmd_head;
	struct t_command_table		*next;
}		t_table;

typedef struct t_minishell
{
	t_table	*table;
	t_table	*table_head;
	char	**env;
	char	**var_lst;
	char	*out_redir;
	int		outfd;
	char	*in_redir;
	int		infd;
	int		exit_code;
    bool	success;
    bool	append_mode;
}		t_msh;

//====== INITIALIZE MINISHELL	=========
t_msh		*init_minishell(int ac, char *av[], char *envp[]);

t_msh		*set_parameters(t_msh *msh);

char		**create_env(char **envp);

void		free_env(t_msh *msh);

//====== SHELL LOOP ==========
void		prompt(t_msh *msh);

//======= TOKENIZER ==========
t_lexer		*lexer(char *prompt);

void		redirection(const char *prompt, t_lexer *current,
				t_lexer **head, int *i);

void		redirection_less(const char *prompt, t_lexer **head,	
				t_lexer **current, int *i);

void		double_qoute(char *prompt,
				t_lexer **head, t_lexer **current, int *i);

void		create_comand_token(char *prompt, t_lexer **head, t_lexer **current,
				int *i);

//TOKEN CREATION
t_lexer		*create_tok(char *data, t_token type);
void		add_token(t_lexer **tokens, t_lexer *new_token);

//DOUBLE QUOTES 
char		*handle_quote(char *prompt);
//TOKEN UTILS
//print token's
void		print_token(t_lexer *tokens);

int			ft_isspace(char c);

int			is_this(char c);

//FREE TOKEN 
void		free_token(t_lexer *head);
//========= PARSER ==========
void		minishell_parser(char *prompt, t_msh *msh);

bool		token_to_table(t_lexer **token, t_table **table, t_msh *msh);

//========= PARSER PIPE ==========
bool		trip_to_table_pipe(t_lexer *token, t_table *table, t_msh *msh);
bool		check_valid_pipe(t_lexer *token, t_table *table);
//========= PARSER COMMAND ==========
bool		trip_to_table_commad(t_lexer *token, t_table *table, t_msh *msh);

bool		add_command_in_table(t_table *new_node, t_table *table, int cmd_len, t_lexer *token);

bool		count_allocate_commands(t_lexer **temp, t_table **new_node,
				int *cmd_len);



//========= EXECUTION =========
void	executor(t_msh *minish);
void	mini_main(t_msh *minish);

// //========= BUILTINS ==========
void	check_path(t_msh *minish);
void	execute_file(t_msh *minish);
void    handle_exit(t_msh *minish);
void	handle_unset(t_msh *minish);
void	handle_export(t_msh *minish);
void	handle_env(t_msh *minish);
void	handle_cd(t_msh *minish);
void	handle_pwd(t_msh *minish);
void	handle_echo(t_msh *minish);

// //========= UTILS ==========


void	replace_env(t_msh *mini, char *path, char *env);
void	free_arr(char **arr);
void	swap_vars(char **newenv);
char	**list2array(t_msh *mini);
char	*ft_getcwd(t_msh *mini);
char	**create_arg_lst(t_msh *mini);
char	*ft_check_var_lst(t_msh *mini, char *var);
void	add_var_to_list(t_msh *mini);
int	handle_redirections(t_msh *mini);
void	restore_redirections(t_msh *mini);
t_msh	*init_mini_vars(int argc, char **argv, char **envp);
bool	ft_isnumber(char *content);
void	free_mini(t_msh *mini, bool keep_env);
void	sigint_handler(int sig);
void	handle_signals(void);
char	*ft_strndup(char *s1, size_t len);
int check_existing_var(char *newvar, t_msh *mini);
void handle_shlvl(t_msh *mini, char sign);
bool check_nl(char *content);
bool check_builtin(t_msh *minish);
void free_table(t_msh *minish);
void free_cmd(t_cmd *cmd);
void free_table(t_msh *minish);
int ft_strcmp(char *s1, char *s2);
char	*ft_getenv(t_msh *mini, char *env);
char	**copy_env(char **envp);

// void	free_cmd(t_cmd *cmd);
// void	free_table(t_msh *minish);
// bool    check_builtin(t_msh *minish);
// bool	check_nl(char *content);
// void	handle_shlvl(t_msh *mini, char sign);
// int	check_existing_var(char *newvar, t_msh *mini);
// char	*ft_strndup(char *s1, size_t len);
// void	free_arr(char **arr); 
// void	swap_vars(char **newenv);
// char	**list2array(t_msh *mini);
// char	*ft_getcwd(t_msh *mini);
// char	**create_arg_lst(t_msh *mini);
// char	*ft_check_var_lst(t_msh *mini, char *var);
// void	add_var_to_list(t_msh *mini);
// int	handle_redirections(t_msh *mini);
// void	restore_redirections(t_msh *mini);
// t_msh	*init_mini_vars(int argc, char **argv, char **envp);
// bool	ft_isnumber(char *content);
// void	sigint_handler(int sig);
// void	handle_signals(void);
// int	ft_strcmp(char *s1, char *s2);
// void	free_mini(t_msh *mini, bool keep_env);
#endif