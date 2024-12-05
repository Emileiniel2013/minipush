/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 11:41:14 by temil-da          #+#    #+#             */
/*   Updated: 2024/12/05 19:08:26 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/executor.h"

void	mini_main(t_mini *minish)
{
	int	*pipefd;
	int	*pid;

	pipefd = (int *)malloc(2 * minish->pipes * sizeof(int));
	pid = (int *)malloc((minish->pipes + 1) * sizeof(int));
	if (!pipefd || !pid)
	{
		free(pipefd);
		free(pid);
		return ;
	}
	create_pipes(pipefd, minish->pipes);
	fork_processes(minish, pipefd, pid);
	close_pipes(pipefd, minish->pipes);
	parent_execution(minish, pid);
	free(pipefd);
	free(pid);
}

void	child_execution(t_mini *minish, int *pipefd, int i)
{
	int	j;

	j = -1;
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	if (i == 0 && minish->infd != STDIN_FILENO)
	{
		dup2(minish->infd, STDIN_FILENO);
		close(minish->infd);
	}
	else if (i > 0)
		dup2(pipefd[(i - 1) * 2], STDIN_FILENO);
	if (i == minish->pipes && minish->outfd != STDOUT_FILENO)
	{
		dup2(minish->outfd, STDOUT_FILENO);
		close(minish->outfd);
	}
	else if (i < minish->pipes)
		dup2(pipefd[i * 2 + 1], STDOUT_FILENO);
	while (++j < 2 * minish->pipes)
		close(pipefd[j]);
	executor(minish);
	exit_minish(minish);
}

void	executor(t_mini *minish)
{
	if (minish->table != NULL)
	{
		if (ft_strcmp(minish->table->command->content, "echo") == 0)
			handle_echo(minish);
		else if (ft_strcmp(minish->table->command->content, "pwd") == 0)
			handle_pwd(minish);
		else if (ft_strcmp(minish->table->command->content, "cd") == 0)
			handle_cd(minish);
		else if (ft_strcmp(minish->table->command->content, "env") == 0)
			handle_env(minish);
		else if (ft_strcmp(minish->table->command->content, "export") == 0)
			handle_export(minish);
		else if (ft_strcmp(minish->table->command->content, "unset") == 0)
			handle_unset(minish);
		else if (ft_strcmp(minish->table->command->content, "exit") == 0)
			handle_exit(minish);
		else if ((ft_strncmp(minish->table->command->content, "./", 2)) == 0)
			execute_file(minish);
		else if (ft_strchr(minish->table->command->content + 1, '=') != NULL)
			add_var_to_list(minish);
		else if (minish->table->command->content[0] == '\0')
			printf("\n");
		else
			check_path(minish);
	}
}

void	parent_execution(t_mini *minish, int *pid)
{
	int	status;
	int	i;

	i = 0;
	status = 0;
	signal(SIGINT, SIG_IGN);
	while (i < minish->pipes + 1)
	{
		waitpid(pid[i], &status, 0);
		handle_shlvl(minish, '-');
		i++;
	}
	signal(SIGINT, sigint_handler);
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) != 0)
		{
			minish->exit_code = WEXITSTATUS(status);
			minish->success = false;
		}
	}
}

void	execute_file(t_mini *minish)
{
	char	*path;
	char	*filename;

	if (ft_strcmp(minish->table->command->content, "./") == 0)
	{
		write_err(minish, 29, NULL);
		return ;
	}
	filename = ft_getcwd(minish);
	path = ft_strjoin(filename, minish->table->command->content + 1);
	ft_free(&filename);
	if (access(path, X_OK) == 0)
		execute_path(minish, &path);
	else
	{
		write_err(minish, 22, minish->table->command->content);
		ft_free(&path);
	}
}
