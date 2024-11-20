/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 17:50:07 by temil-da          #+#    #+#             */
/*   Updated: 2024/11/20 16:14:02 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	leaks(void)
{
	system("minishell");
}

int	main(int argc, char *argv[], char *envp[])
{
	t_mini	*minish;
	char	*line;

	atexit(leaks);
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
