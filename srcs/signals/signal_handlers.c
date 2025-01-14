/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 18:37:56 by anaqvi            #+#    #+#             */
/*   Updated: 2025/01/14 16:24:53 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	signal_handler_interactive(int signum)
{
	g_signal_received = signum;
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		// rl_done = 1;
	}
	else if (signum == SIGQUIT)
		g_signal_received = SIGQUIT;
}

static void	signal_handler_heredoc(int signum)
{
	g_signal_received = signum;
	if (signum == SIGINT)
	{
		// write(STDOUT_FILENO, "\n", 1);
		// rl_replace_line("", 0);
		// rl_on_new_line();
		// rl_redisplay();
		rl_done = 1;
		exit(128 + SIGINT);
	}
	else if (signum == SIGQUIT)
		g_signal_received = SIGQUIT;
}

// static void	signal_handler_child(int signum)
// {
// 	g_signal_received = signum;
// 	if (signum == SIGINT)
// 	{
// 		write(STDOUT_FILENO, "\n", 1);
// 		exit(128 + SIGINT);
// 	}
// 	else if (signum == SIGQUIT)
// 	{
// 		write(STDERR_FILENO, "Quit\n", 5);
// 		exit(128 + SIGQUIT);
// 	}
// }

static void	signal_handler_non_interactive(int signum)
{
	g_signal_received = signum;
	printf("signal received: %i\n", signum); // for testing
	if (signum == SIGINT)
	{
		g_signal_received = SIGINT;
		write(STDOUT_FILENO, "\n", 1);
		// rl_replace_line("", 0);
		// rl_on_new_line();
		// rl_redisplay();
		// rl_done = 1;
	}
	else if (signum == SIGQUIT)
		g_signal_received = SIGQUIT;
}

int	set_signal_handler(t_mode mode)
{
	struct sigaction	sa;

	if (mode == INTERACTIVE)
	{
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = SA_RESTART;
		sa.sa_handler = signal_handler_interactive;
		if (sigaction(SIGINT, &sa, NULL) == -1)
			return (perror("sigaction"), -1);
		if (sigaction(SIGQUIT, &sa, NULL) == -1)
			return (perror("sigaction"), -1);
	}
	if (mode == HEREDOC)
	{
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = SA_RESTART;
		sa.sa_handler = signal_handler_heredoc;
		if (sigaction(SIGINT, &sa, NULL) == -1)
			return (perror("sigaction"), -1);
		if (sigaction(SIGQUIT, &sa, NULL) == -1)
			return (perror("sigaction"), -1);
	}
	else if (mode == CHILD)
	{
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		sa.sa_handler = SIG_DFL;
		if (sigaction(SIGINT, &sa, NULL) == -1)
			return (perror("sigaction"), -1);
		if (sigaction(SIGQUIT, &sa, NULL) == -1)
			return (perror("sigaction"), -1);
	}
	else if (mode == NON_INTERACTIVE)
	{
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = SA_RESTART;
		sa.sa_handler = signal_handler_non_interactive;
		if (sigaction(SIGINT, &sa, NULL) == -1)
			return (perror("sigaction"), -1);
		if (sigaction(SIGQUIT, &sa, NULL) == -1)
			return (perror("sigaction"), -1);
	}
	return (0);
}
