/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 15:54:26 by anaqvi            #+#    #+#             */
/*   Updated: 2025/01/10 18:13:01 by anaqvi           ###   ########.fr       */
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
	minishell.tokenized = NULL;
	return (minishell);
}

static void	cleanup_before_loop(t_minishell *minishell)
{
	gc_free_2d_char_arr(minishell->tokenized, minishell);
	minishell->tokenized = NULL;
	gc_free_cmd_grps(minishell);
	gc_free(minishell->input, minishell);
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
		/*lexer here*/
		if (parser(&minishell) != -1)
			execution(&minishell);
		cleanup_before_loop(&minishell);
	}
	gc_exit(&minishell, EXIT_SUCCESS);
}
