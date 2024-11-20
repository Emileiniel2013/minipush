/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_functions_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:00:28 by tndreka           #+#    #+#             */
/*   Updated: 2024/11/20 16:04:57 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"

bool	handle_heredoc(t_lexer **token, t_mini *msh, t_table **table)
{
	char	*separator;
	char	*line;
	int		fd;

	line = NULL;
	fd = -1;
	(void)table;
	check_valid_heredoc(token, msh, &separator);
	open_the_fd(&fd, msh, separator);
	while (true)
	{
		line = readline("heredoc> ");
		if (ft_strcmp(line, separator) == 0)
		{
			free(line);
			break ;
		}
		write_line(fd, line);
		free(line);
	}
	msh->in_redir = ft_strdup(".temporary_heredoc");
	free(separator);
	close(fd);
	return (true);
}

void	write_line(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

bool	check_valid_heredoc(t_lexer **token, t_mini *msh, char **separator)
{
	if ((*token)->next == NULL)
		write_err(msh, 13, NULL);
	else if ((*token)->next->type != STRING)
	{
		write_err(msh, 14, ((*token)->next->data));
		return (false);
	}
	else
		(*token)->next->type = DELIMITER;
	(*separator) = ft_strdup((*token)->next->data);
	return (true);
}

bool	open_the_fd(int *fd, t_mini *msh, char *separator)
{
	(*fd) = open(".temporary_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if ((*fd) < 0)
	{
		write_err(msh, 15, NULL);
		free(separator);
		return (false);
	}
	else
		return (true);
}
