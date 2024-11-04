/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pompt.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:16:58 by tndreka           #+#    #+#             */
/*   Updated: 2024/10/05 15:36:03 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/mini_sh.h"

/*
	Here its where we start our shell loop//
	1->checking if the input comes wrom a interactiv terminal
	2-> the input its saved in prompt
*/
void prompt(t_msh *msh)
{
    char	*prompt;
    //size_t i = 0;
    if (!isatty(STDIN_FILENO))
        return ;
    while (1)
    {
        prompt = readline("Minishell~ ");
        if (prompt == NULL)
        {
            break ;
        }
        minishell_parser(prompt, msh);
        if(ft_strncmp(prompt, "exit", 4) == 0)
        {
            free(prompt); // free the exit part
            break;
        }
        free(prompt);
    }
}

t_lexer *check_prompt(char *prompt, t_msh *msh)
{
    t_lexer             *head;

    head = NULL;
    if(ft_strlen(prompt) > 0)
        add_history(prompt);
    head = tokenize_prompt(prompt, msh);
    return (head);
}