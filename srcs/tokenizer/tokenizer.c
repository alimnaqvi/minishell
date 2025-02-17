/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:50:55 by rreimann          #+#    #+#             */
/*   Updated: 2025/02/17 02:14:01 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	has_unclosed_quote(char *str)
{
	int	in_sing_quot;
	int	in_doub_quot;

	in_sing_quot = 0;
	in_doub_quot = 0;
	if (!str)
		return (0);
	while (*str)
	{
		if (*str == '"' && !in_sing_quot)
			in_doub_quot = !in_doub_quot;
		else if (*str == '\'' && !in_doub_quot)
			in_sing_quot = !in_sing_quot;
		str++;
	}
	if (in_doub_quot || in_sing_quot)
		return (ft_putendl_fd(QUOTE_ERR, STDERR_FILENO), 1);
	return (0);
}

// EXAMPLE: echo "asd" > something| $USER$USER<<'asd' |
// This is the function that will be tokenizing everything
// Essentially just splits everything into strings
// By the rules that we have set in the Minishell
// 1. Skip all whitespaces that are outside of quotes
int	tokenizer(t_minishell *minishell)
{
	size_t	index;
	t_vec	tokens_vec;
	t_token	token;

	if (!minishell)
		return (gc_exit(minishell, EXIT_FAILURE), -1);
	if (has_unclosed_quote(minishell->input))
		return (-1);
	tokens_vec = vec_init(sizeof(t_token));
	index = 0;
	while (minishell->input[index])
	{
		token = get_next_token(minishell, index);
		if (token.read_length == 0)
		{
			printf("Read length was ZERO. Oh nooo!!!\n");
			break ;
		}
		index += token.read_length;
		vec_push_copy(minishell, &tokens_vec, &token);
	}
	tokens_to_array(minishell, &tokens_vec);
	return (1);
}
