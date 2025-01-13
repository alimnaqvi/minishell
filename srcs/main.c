/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 15:54:26 by anaqvi            #+#    #+#             */
/*   Updated: 2025/01/13 18:40:19 by anaqvi           ###   ########.fr       */
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
}

static void	signal_handler(int signum)
{
	g_signal_received = signum;
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		rl_done = 1;
	}
	else if (signum == SIGQUIT)
		g_signal_received = SIGQUIT;
}

static void	set_signal_handler()
{
	struct sigaction	sa;

	g_signal_received = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = signal_handler;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		perror("sigaction");
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		perror("sigaction");
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;
	
	minishell = init_main_var(argc, argv, envp);
	while (1)
	{
		set_signal_handler();
		// minishell.input = ft_readline(&minishell); /*readline, check NULL, gc_add_to_allocs, add_history*/
		minishell.input = readline("minishell$ ");
		add_history(minishell.input);
		gc_add_to_allocs(minishell.input, &minishell);
		if (!minishell.input)
			break ;
		       minishell.tokenized = ft_split(minishell.input, ' ');
              int k = 0;
             while (minishell.tokenized[k])
             {
                      gc_add_to_allocs(minishell.tokenized[k], &minishell);
                      // printf("minishell.tokenized[%i] contains: %s\n", k, minishell.tokenized[k]);
                     k++;
               }
               gc_add_to_allocs(minishell.tokenized, &minishell);
		// printf("You typed \"%s\"!\n", minishell.input);
		/*lexer here*/
		if (parser(&minishell) != -1 && g_signal_received != SIGINT)
			execution(&minishell);
		cleanup_before_loop(&minishell);
	}
	gc_exit(&minishell, EXIT_SUCCESS);
}
