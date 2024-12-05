/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_functions_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:00:28 by tndreka           #+#    #+#             */
/*   Updated: 2024/12/05 19:10:26 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"

bool	handle_heredoc(t_tkn_lst **token, t_mini *msh, t_table **table)
{
	char	*separator;
	int		fd;

	fd = -1;
	(void)table;
	if (!check_valid_heredoc(token, msh, &separator))
		return (false);
	open_the_fd(&fd, msh, separator);
	if (!process_heredoc_input(fd, separator))
	{
		ft_free(&separator);
		unlink(".heredoc_tmp");
		return (false);
	}
	msh->in_redir = ft_strdup(".heredoc_tmp");
	ft_free(&separator);
	close(fd);
	return (true);
}

void	write_line(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

bool	check_valid_heredoc(t_tkn_lst **token, t_mini *msh, char **separator)
{
	if ((*token)->next == NULL)
		return (write_err(msh, 13, NULL), false);
	else if ((*token)->next->tkn != STRING)
	{
		write_err(msh, 14, ((*token)->next->content));
		return (false);
	}
	else
		(*token)->next->tkn = DELIMITER;
	(*separator) = ft_strdup((*token)->next->content);
	return (true);
}

bool	open_the_fd(int *fd, t_mini *msh, char *separator)
{
	(*fd) = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if ((*fd) < 0)
	{
		write_err(msh, 15, NULL);
		free(separator);
		return (false);
	}
	else
		return (true);
}

bool	is_in_single_quote(char *content)
{
	int		i;
	bool	in_single_quote;

	i = 0;
	in_single_quote = false;
	while (content[i])
	{
		if (content[i] == '\'')
			in_single_quote = !in_single_quote;
		i++;
	}
	return (in_single_quote);
}
