/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:09:16 by temil-da          #+#    #+#             */
/*   Updated: 2024/12/04 21:41:26 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/executor.h"

t_mini	*init_mini_vars(int argc, char **argv, char **envp)
{
	t_mini	*mini;

	(void)argc;
	(void)argv;
	mini = malloc(sizeof(t_mini));
	mini->env = copy_env(envp);
	mini->var_lst = NULL;
	mini->table = NULL;
	mini->table_head = NULL;
	mini->out_redir = NULL;
	mini->in_redir = NULL;
	mini->append_mode = false;
	mini->infd = STDIN_FILENO;
	mini->outfd = STDOUT_FILENO;
	mini->redirfd = STDOUT_FILENO;
	mini->exit_code = 0;
	mini->success = true;
	mini->pipes = 0;
	return (mini);
}

int	handle_redirections(t_mini *minish)
{
	int	fd;

	fd = 0;
	if (minish->in_redir)
	{
		fd = open(minish->in_redir, O_RDONLY);
		if (fd < 0)
			return (write_err(minish, 6, minish->in_redir), -1);
		minish->infd = fd;
	}
	if (minish->out_redir)
	{
		if (minish->append_mode == true)
		{
			fd = open(minish->out_redir, O_WRONLY | O_CREAT | O_APPEND, 0644);
			minish->append_mode = false;
		}
		else
			fd = open(minish->out_redir, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			return (write_err(minish, 7, NULL), -1);
		minish->outfd = fd;
		minish->redirfd = fd;
	}
	return (0);
}

void	restore_redirections(t_mini *mini)
{
	if (mini->in_redir)
	{
		if (mini->infd != STDIN_FILENO)
			close(mini->infd);
		mini->infd = STDIN_FILENO;
		free(mini->in_redir);
		mini->in_redir = NULL;
	}
	if (mini->out_redir)
	{
		if (mini->outfd != STDOUT_FILENO)
			close(mini->outfd);
		mini->outfd = STDOUT_FILENO;
		free(mini->out_redir);
		mini->out_redir = NULL;
	}
	if (open(".heredoc_tmp", O_RDONLY, 0644) > 0)
		unlink(".heredoc_tmp");
	if (mini->success)
		mini->exit_code = EXIT_SUCCESS;
	else
		mini->success = true;
	mini->redirfd = STDOUT_FILENO;
	mini->pipes = 0;
}

bool	check_valid_varname(char *content, t_mini *minish)
{
	int		i;
	char	*varname;

	varname = NULL;
	i = 0;
	if (strcmp(content, "=") == 0)
		return (write_err(minish, 69, content), false);
	while (content[i] && content[i] != '=')
		i++;
	varname = ft_strndup(content, i);
	if (ft_isalpha(varname[0]) != 1 && varname[0] != '_')
		return (write_err(minish, 69, varname), ft_free(&varname), false);
	i = 0;
	while (varname[++i])
	{
		if (ft_isalnum(varname[i]) != 1 && varname[i] != '_')
			return (write_err(minish, 69, varname), ft_free(&varname), false);
	}
	return (ft_free(&varname), true);
}

void	handle_env_export(t_mini *minish)
{
	int	i;

	i = 0;
	while (minish->env && minish->env[i])
	{
		write(minish->redirfd, "declare -x ", 11);
		write(minish->redirfd, minish->env[i], ft_strlen(minish->env[i]));
		write(minish->redirfd, "\n", 1);
		i++;
	}
	i = 0;
	while (minish->var_lst && minish->var_lst[i])
	{
		write(minish->redirfd, "declare -x ", 11);
		write(minish->redirfd, minish->var_lst[i],
			ft_strlen(minish->var_lst[i]));
		write(minish->redirfd, "\n", 1);
		i++;
	}
}
