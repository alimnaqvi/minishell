/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 15:54:26 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/19 11:10:24 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_signal	g_signal_received;

static t_minishell	init_main_var(int argc, char **argv, char **envp)
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

static void	setup_terminal(t_minishell *minishell)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	minishell->original_term = term;
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;

	minishell = init_main_var(argc, argv, envp);
	while (1)
	{
		setup_terminal(&minishell);
		g_signal_received = 0;
		set_signal_handler(INTERACTIVE);
		if (ft_readline(&minishell) < 0 || tokenizer(&minishell) < 0)
		{
			gc_free(minishell.input, &minishell);
			continue ;
		}
		if (parser(&minishell) != -1 && g_signal_received != SIGINT)
			execution(&minishell);
		cleanup_before_loop(&minishell);
	}
	gc_exit(&minishell, EXIT_SUCCESS);
}
