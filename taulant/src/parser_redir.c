/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:49:13 by tndreka           #+#    #+#             */
/*   Updated: 2024/11/14 15:01:27 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/mini_sh.h"



int	handle_type_of_redir_type2(t_lexer **token_lst, t_msh *msh)
{
	int		fd;
	char	*separator;

	fd = -1;
	if (!(*token_lst)->next)
		return -1;
	else if ((*token_lst)->next->type != COMMAND)
		return -1;
	else
		(*token_lst)->next->type = DELIMITER;
	separator = ft_strdup((*token_lst)->next->data);
	fd = open(".heredoc_tempo", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return -1;
	while (loop(separator, fd))
		;
	msh->in_redir = ft_strdup("heredoc_tempo");
	free(separator);
	separator = NULL;
	close(fd);
	return (0);
}
int	loop(char *separator, int fd)
{
	char	*here;

	here = NULL;
	here = readline(" >");
	if (ft_strcmp(here, separator) == 0)
	{
		free(here);
		here = NULL;
		return (-1);
	}
	else
	{
		write(fd, here, ft_strlen(here));
		write(fd, "\n", 1);
		free(here);
		here = NULL;
		return (0);
	}
}
int	handle_type_of_redir(t_lexer **token_lst, t_msh *msh)
{
	if ((*token_lst)->next == NULL)
		return (write_err(msh, 8, NULL), -1);
	else if ((*token_lst)->next->type != COMMAND)
		return (write_err(msh, 9, (*token_lst)->next->data), -1);
	if ((*token_lst)->type == REDIRIN)
	{
		if (msh->in_redir)
			free(msh->in_redir);
		msh->in_redir = ft_strdup((*token_lst)->next->data);
	}
	else if ((*token_lst)->type == REDIROUT || (*token_lst)->type == REDIROUTAPP)
	{
		if (msh->out_redir)
			free(msh->out_redir);
		msh->out_redir = ft_strdup((*token_lst)->next->data);
	}
	(*token_lst)->next->type = FILENAME;
	if ((*token_lst)->type == REDIROUTAPP)
		msh->append_mode = true;
	return (0);
}