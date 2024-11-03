/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tndreka <tndreka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 19:47:06 by tndreka           #+#    #+#             */
/*   Updated: 2024/10/05 12:37:50 by tndreka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/mini_sh.h"

/* This function creates a copy of all enviroment
	variables. The function takes the original
	copy of the Enviroment Variables and return a copy
	of it. 
*/
t_msh *set_parameters(t_msh *msh)
{
    msh->var_lst = NULL;
    msh->table = NULL;
    msh->table_head = NULL;
    msh->in_redir = NULL;
    msh->out_redir = NULL;
    msh->exit_code = 0;
    msh->infd = STDIN_FILENO;
    msh->outfd = STDOUT_FILENO;
    return (msh);
}


t_msh *init_minishell(int ac, char *av[], char *envp[])
{
    t_msh       *msh;
    char        **copy_env;

    (void)ac;
    (void )av;
    copy_env = create_env(envp);
    msh = malloc(sizeof(t_msh));
    msh->env = copy_env;
    set_parameters(msh);
    return msh;
}

char **create_env(char **envp)
{
	char **dup;
	size_t i;
	size_t j;

	i = 0;
	while (envp[i] != NULL)
		i++;
	dup = malloc((i + 1) * sizeof(char *));
	if (!dup)
	{
		printf("failed stage 1 malloc for dup\n");
		return (NULL);
	}
	j = 0;
	while (j < i)
	{
		dup[j] = ft_strdup(envp[j]);
		if (!dup[j])
		{
		printf("failed stage 2 for duplication\n");
		return (free (dup), NULL);
		}
		j++;
	}
	dup[j] = NULL;
	return (dup);
}
	
void free_env(t_msh *msh)
{
	size_t i = 0;
	while (msh->env[i] != NULL)
	{
		free(msh->env[i]);
		i++;
	}
	free(msh->env);
}