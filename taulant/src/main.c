/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 10:48:48 by tndreka           #+#    #+#             */
/*   Updated: 2024/11/14 15:05:21 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/mini_sh.h"

void	free_minish(t_msh *minish, bool keep_env);
void	exit_minish(t_msh *minish);


// int	main(int ac, char *av[], char *envp[])
// {
// 	t_msh	*minish;

// 	minish = init_minishell(ac, av, envp);
// 	prompt(minish);
// 	free_env(minish);
// 	atexit(leaks);
// }
void	free_minish(t_msh *minish, bool keep_env)
{
	if (minish->table_head)
		free_table(minish);
	if (minish->env && !keep_env)
		free_arr(minish->env);
	if (minish->var_lst)
		free_arr(minish->var_lst);
	if (minish->in_redir)
	{
		free(minish->in_redir);
		minish->in_redir = NULL;
	}
	if (minish->out_redir)
	{
		free(minish->out_redir);
		minish->out_redir = NULL;
	}
	free(minish);
	minish = NULL;
}

void	exit_minish(t_msh *minish)
{
	int	status;

	status = minish->exit_code;
	free_minish(minish, false);
	exit(status);
}

// void	free_arr(char **arr)
// {
// 	int	i;

// 	i = -1;
// 	if (arr)
// 	{
// 		while (arr[++i])
// 			free(arr[i]);
// 		free(arr);
// 	}
// 	arr = NULL;
// }
int	main(int argc, char *argv[], char *envp[])
{
	t_msh	*minish;
	char	*line;

	handle_signals();
	minish = init_mini_vars(argc, argv, envp);
	while (1)
	{
		line = readline("Minishell $ ");
		if (!line)
			break ;
		minishell_parser(line, minish);
		free(line);
		if (minish->table && (handle_redirections(minish) != -1))
		{
			if (!minish->table->rightpipe && check_builtin(minish))
				executor(minish);
			else
				mini_main(minish);
		}
		restore_redirections(minish);
		if (minish->table_head)
			free_table(minish);
	}
	exit_minish(minish);
}

