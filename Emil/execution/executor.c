/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: temil-da <temil-da@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 11:41:14 by temil-da          #+#    #+#             */
/*   Updated: 2024/10/21 15:33:12 by temil-da         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/executor.h"

void	executor(t_minishell *minishell)
{
	int		i;
	char	*content;

	i = 0;
	content = minishell->table->simple_command->content;
	if (minishell->table != NULL)
	{
		if (ft_strncmp(content, "echo", ft_strlen(content)) == 0)
			handle_echo(minishell);
		else if (ft_strncmp(content, "pwd", ft_strlen(content)) == 0)
			handle_pwd(minishell);
		else if (ft_strncmp(content, "cd", ft_strlen(content)) == 0)
			handle_cd(minishell);
		else if (ft_strncmp(content, "env", ft_strlen(content)) == 0)
			handle_env(minishell);
		else if (ft_strncmp(content, "export", ft_strlen(content)) == 0)
			handle_export(minishell);
		else if (ft_strncmp(content, "unset", ft_strlen(content)) == 0)
			handle_unset(minishell);
		else if ((ft_strncmp (content, "./", 2)) == 0)
			execute_file(minishell);
		else if (ft_strchr(content + 1, '=') != NULL)
			add_var_to_list(minishell);
		else if (content[0] == '\0')
			printf("\n");
		else
			check_path(minishell);
	}
}

void	mini_main(t_minishell *minishell)
{
	int	pipefd[2];
	int	prevpipefd;
	int	pid;

	prevpipefd = -1;
	while(minishell->table != NULL)
	{
		if (minishell->table->rightpipe == true)
			pipe(pipefd);
		pid = fork();
		if (pid == 0)
		{
			if (minishell->table->leftpipe == true)
			{
				dup2(prevpipefd, STDIN_FILENO);
				close(prevpipefd);
			}
			else if (!minishell->table->leftpipe && minishell->in_redir)
				dup2(minishell->infd, STDIN_FILENO);
			if (minishell->table->rightpipe == true)
			{
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[0]);
				close(pipefd[1]);
			}
			else if (!minishell->table->rightpipe && minishell->out_redir)
				dup2(minishell->outfd, STDOUT_FILENO);
			executor(minishell);
			exit(EXIT_SUCCESS);
		}
		waitpid(pid, NULL, 0);
		if (minishell->table->leftpipe)
			close(prevpipefd);
		if (minishell->table->rightpipe)
		{
			prevpipefd = pipefd[0];
			close(pipefd[1]);
		}
		minishell->table = minishell->table->next;
	}
	close(pipefd[0]);
}