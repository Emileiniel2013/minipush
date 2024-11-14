/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 17:50:07 by temil-da          #+#    #+#             */
/*   Updated: 2024/11/14 16:01:04 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	t_mini	*minish;
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
