/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:10:49 by temil-da          #+#    #+#             */
/*   Updated: 2024/12/04 21:53:49 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/executor.h"

void	handle_signals(void)
{
	rl_catch_signals = 0;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, sigint_handler);
}

void	sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	create_pipes(int *pipefd, int num_pipes)
{
	int	i;

	i = 0;
	while (i < num_pipes)
	{
		pipe(pipefd + i * 2);
		i++;
	}
}

void	fork_processes(t_mini *minish, int *pipefd, int *pid)
{
	int	i;

	i = 0;
	while (i < minish->pipes + 1)
	{
		handle_shlvl(minish, '+');
		pid[i] = fork();
		if (pid[i] == 0)
			child_execution(minish, pipefd, i);
		minish->table = minish->table->next;
		i++;
	}
}

void	close_pipes(int *pipefd, int num_pipes)
{
	int	i;

	i = 0;
	while (i < 2 * num_pipes)
	{
		close(pipefd[i]);
		i++;
	}
}
