/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 15:54:26 by anaqvi            #+#    #+#             */
/*   Updated: 2025/01/10 16:57:35 by rreimann         ###   ########.fr       */
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
		ft_readline(&minishell);
		if (!minishell.input)
			break ;
		printf("You typed \"%s\"!\n", minishell.input);
		/*lexer here*/
		if (tokenizer(&minishell) != -1 && parser(&minishell) != -1)
			// execution
		cleanup_before_loop(&minishell);
	}
	gc_exit(&minishell, EXIT_SUCCESS);
}
