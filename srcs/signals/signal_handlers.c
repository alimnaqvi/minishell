/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 18:37:56 by anaqvi            #+#    #+#             */
/*   Updated: 2025/01/15 19:44:29 by anaqvi           ###   ########.fr       */
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
	}
	else if (signum == SIGQUIT)
		g_signal_received = SIGQUIT;
}

static void	signal_handler_heredoc(int signum)
{
	g_signal_received = signum;
	if (signum == SIGINT)
	{
		rl_done = 1;
		exit(128 + SIGINT);
	}
	else if (signum == SIGQUIT)
		g_signal_received = SIGQUIT;
}

static void	signal_handler_non_interactive(int signum)
{
	g_signal_received = signum;
	if (signum == SIGINT)
	{
		g_signal_received = SIGINT;
		write(STDOUT_FILENO, "\n", 1);
	}
	else if (signum == SIGQUIT)
		g_signal_received = SIGQUIT;
}

int	set_signal_handler(t_mode mode)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	if (mode == CHILD)
	{
		sa.sa_flags = 0;
		sa.sa_handler = SIG_DFL;
		if (sigaction(SIGINT, &sa, NULL) == -1)
			return (perror("sigaction"), -1);
		if (sigaction(SIGQUIT, &sa, NULL) == -1)
			return (perror("sigaction"), -1);
		return (0);
	}
	sa.sa_flags = SA_RESTART;
	if (mode == INTERACTIVE)
		sa.sa_handler = signal_handler_interactive;
	else if (mode == NON_INTERACTIVE)
		sa.sa_handler = signal_handler_non_interactive;
	else if (mode == HEREDOC)
		sa.sa_handler = signal_handler_heredoc;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		return (perror("sigaction"), -1);
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		return (perror("sigaction"), -1);
	return (0);
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