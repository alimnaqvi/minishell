/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 15:54:26 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/12 16:28:52 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received;

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
	minishell.last_exit_status = 0;
	return (minishell);
}

static void	cleanup_before_loop(t_minishell *minishell)
{
	gc_free_2d_char_arr(minishell->tokenized, minishell);
	minishell->tokenized = NULL;
	gc_free_cmd_grps(minishell);
	gc_free(minishell->input, minishell);
	gc_close_all_open_fds(minishell);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;

	printf("minishell's pid is %i\n", getpid()); // for testing
	minishell = init_main_var(argc, argv, envp);
	while (1)
	{
		g_signal_received = 0;
		set_signal_handler(INTERACTIVE);
		// minishell.input = ft_readline(&minishell); /*readline, check NULL, gc_add_to_allocs, add_history*/
		minishell.input = readline("minishell$ ");
		add_history(minishell.input);
		gc_add_to_allocs(minishell.input, &minishell);
		// printf("You typed \"%s\"!\n", minishell.input);
		/*lexer here*/
		set_signal_handler(NON_INTERACTIVE);
		if (parser(&minishell) != -1 && g_signal_received != SIGINT)
			execution(&minishell);
		cleanup_before_loop(&minishell);
	}
	gc_exit(&minishell, EXIT_SUCCESS);
}
