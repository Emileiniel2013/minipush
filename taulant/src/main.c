/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 10:48:48 by tndreka           #+#    #+#             */
/*   Updated: 2024/11/13 15:34:11 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/mini_sh.h"

void	leaks(void)
{
	system("leaks Minishell");
}

int	main(int ac, char *av[], char *envp[])
{
	t_msh	*minish;

	minish = init_minishell(ac, av, envp);
	prompt(minish);
	free_env(minish);
	atexit(leaks);
}
