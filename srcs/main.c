/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 15:54:26 by anaqvi            #+#    #+#             */
/*   Updated: 2024/12/25 13:42:33 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_minishell init_main_var(int argc, char **argv, char **envp)
{
	t_minishell	minishell;

	(void)argc;
	(void)argv;
	minishell.garbage.allocs = NULL;
	minishell.garbage.open_fds = NULL;
	minishell.input = NULL;
	minishell.mini_env = copy_2d_char_arr(envp, &minishell);
	return (minishell);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;
	
	minishell = init_main_var(argc, argv, envp);
	while (1)
	{
		// minishell.input = ft_readline(&minishell); /*readline, check NULL, gc_add_to_allocs, add_history*/
		minishell.input = readline("minishell$ ");
		gc_add_to_allocs(minishell.input, &minishell);
		if (!minishell.input)
			break ;
		printf("You typed \"%s\"!\n", minishell.input);
		/*main logic of minishell comes here*/
		gc_free(minishell.input, &minishell);
	}
	gc_exit(&minishell, EXIT_SUCCESS);
}
