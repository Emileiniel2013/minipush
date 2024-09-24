/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: temil-da <temil-da@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 10:56:58 by temil-da          #+#    #+#             */
/*   Updated: 2024/09/24 13:22:27 by temil-da         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H

# define EXECUTOR_H

# include "libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <curses.h>

# ifndef STD_OUT
#  define STD_OUT 1
# endif

typedef enum
{
	TOKEN_COMMAND,
	TOKEN_FILENAME,
	TOKEN_ARGUMENT,
	TOKEN_LITERAL,
	TOKEN_OPERATOR,
	TOKEN_SPECIAL,
	TOKEN_INVALID_ARG,
	TOKEN_UNKNOWN
}		token_type;

typedef struct	t_simple_command
{
	token_type					token;
	char						*content;
	struct	t_simple_command	*next;
}	t_command;

typedef	struct	t_test_struct
{
	bool						leftpipe;
	bool						rightpipe;
	t_command					*simple_command;
	struct t_test_struct		*next;
}		command_table;

typedef struct t_minishell
{
	command_table	*table;
	char			**env;
}		t_minishell;

void	executor(t_minishell *minishell);
void	handle_echo(t_command *command);
void	handle_pwd(t_minishell *minishell);
void	handle_cd(t_minishell *minishell);
void	handle_env(t_minishell *minishell);
void	handle_export(t_minishell *minishell);
void	handle_unset(t_minishell *minishell);
void	check_path(t_minishell *minishell);
char	**list2array(t_minishell *minishell);
char	**copy_env(char **envp);
void	free_env(t_minishell *minishell);
void	swap_vars(char **newenv);
void	replace_env(t_minishell *minishell, char *path);

#endif