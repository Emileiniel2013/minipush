/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_sh.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 10:48:37 by tndreka           #+#    #+#             */
/*   Updated: 2024/11/12 14:47:58 by tndreka          ###   ########.fr       */
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

typedef enum e_token {
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
    //bool						leftpipe;
    //bool						rightpipe;
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
//    bool	success;
//    bool	append_mode;
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

void	create_comand_token(char *prompt, t_lexer **head, t_lexer **current,
		int *i);

//TOKEN CREATION
t_lexer	*create_tok(char *data, t_token type);
void	add_token(t_lexer **tokens, t_lexer *new_token);

//DOUBLE QUOTES 
char	*handle_quote(char *prompt);

//REDIRECTION
// char	*create_redir_arr(char c);

//TOKEN UTILS
//print token's
void	print_token(t_lexer *tokens);

int	ft_isspace(char c);

int	is_this(char c);

//FREE TOKEN 
void	free_token(t_lexer *head);
//========= PARSER ==========
void minishell_parser(char **prompt, t_msh **msh);






//
//
////loop
//void prompt(t_msh *msh);
//
////enviroment
//char **create_env(char **envp);
//void free_env(t_msh *msh);
//
//// LEXING
//int ft_isspace(char c); // chec if it is a white space
//
//int is_this(char c);  // checks the delimeter
//
//t_token get_token_type(char c); // check what type of token is
//
//t_lexer *create_tok(char *data, t_token type); // creates a token
//
//void add_token(t_lexer **tokens, t_lexer *new_token); // ad the token to the list of tokens
//
//char *create_redir_arr(char c);
//
//t_lexer *lexer(char *prompt);
//
//void free_token(t_lexer *head);
//
//void print_token(t_lexer *tokens);
//
//char *handle_quote(char *start);
#endif