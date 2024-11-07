/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_sh.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 10:48:37 by tndreka           #+#    #+#             */
/*   Updated: 2024/10/11 17:37:34 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_SHELL_H
# define MINI_SHELL_H

//============ HEADER FILES =============
# include "libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>

# define EXIT_SUCCESS 	0
# define EXIT_FAILURE 	1

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 10000
#endif

// STRUCT's
/* === comand === */
/* contains info of the comand
*/

//=========== STRUCTS =====================

typedef enum e_token {
	TOKEN_COMMAND,
	TOKEN_ARGUMENT,
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_RIDIRECTION_LESS,
	TOKEN_RIDIRECTION_LESS_LESS, 	
	TOKEN_RIDIRECTION_GREAT, 	
	TOKEN_RIDIRECTION_GREAT_GREAT,
	TOKEN_SINGLE_QUOTE,
	TOKEN_DOUBLE_QUOTE,
	TOKEN_UNKNOWN
}	t_token;


/*
	LEXING STRUCT
*/

/*
	EXECUTION 
typedef struct s_redirect {
	char *data;
	e_token type;
}

typedef struct s_runconfig {
	char *cmd;
	int argc;
	char **args;
	t_list *redirect;
	struct s_runconfig* pipe;
}

*/


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

/* === MINISHELL STRUCT ===*/
/*
*** This struct is going to contain every information if minishell data
*** comand info , comand table
*** input information.
*/
//typedef struct s_msh
//{
//	char **env;
//	char *input;
//	t_comands *comands;
//	t_lexer	  *list;
//	t_token type;
//}	t_msh;

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
    //bool	success;
   // bool	append_mode;
}		t_msh;


t_msh *init_minishell(int ac, char *av[], char *envp[]);
t_msh *set_parameters(t_msh *msh);
void minishell_parser(char **prompt, t_msh **msh);


//loop
void prompt(t_msh *msh);

//enviroment
char **create_env(char **envp);
void free_env(t_msh *msh);

// LEXING
int ft_isspace(char c); // chec if it is a white space
 
int is_this(char c);  // checks the delimeter

t_token get_token_type(char c); // check what type of token is

t_lexer *create_tok(char *data, t_token type); // creates a token 

void add_token(t_lexer **tokens, t_lexer *new_token); // ad the token to the list of tokens

char *create_redir_arr(char c);

t_lexer *lexer(char *prompt);

void free_token(t_lexer *head);

void print_token(t_lexer *tokens);

char *handle_quote(char *start);
#endif