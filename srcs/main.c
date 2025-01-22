/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 15:54:26 by anaqvi            #+#    #+#             */
/*   Updated: 2025/01/22 15:38:43 by rreimann         ###   ########.fr       */
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
	
	// char char_0 = 's';
	// char char_1 = 't';
	// char char_2 = 'a';
	// char char_3 = 'r';
	// char char_4 = 't';

	minishell = init_main_var(argc, argv, envp);
	while (1)
	{
		ft_readline(&minishell);
		if (!minishell.input)
			break ;
		// printf("You typed \"%s\"!\n", minishell.input);

		// t_vec vector = vec_init(sizeof(char *));
		// vec_push(&minishell, &vector, &char_0);
		// vec_push(&minishell, &vector, &char_1);
		// vec_push(&minishell, &vector, &char_2);
		// vec_push(&minishell, &vector, &char_3);
		// vec_push(&minishell, &vector, &char_4);
		// for (size_t i = 0; i < vector.length; i++)
		// {
		// 	char character = **(char **)vec_get(&vector, i);
		// 	printf("Vec: %c\n", character);
		// }

		// gc_exit(&minishell, EXIT_SUCCESS);

		/*lexer here*/
		if (tokenizer(&minishell) != -1 && parser(&minishell) != -1)
			// execution
		cleanup_before_loop(&minishell);
	}
	gc_exit(&minishell, EXIT_SUCCESS);
}
