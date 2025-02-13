/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:50:55 by rreimann          #+#    #+#             */
/*   Updated: 2025/02/13 20:56:54 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		return (-1);
	tokens_vec = vec_init(sizeof(t_token));
	index = 0;
	while (minishell->input[index])
	{
		// get_next_token allocates memory for the token
		token = get_next_token(minishell, index);
		if (token.read_length == 0)
		{
			printf("Read length was ZERO. Oh nooo!!!\n");
			break ;
		}
		index += token.read_length;
		// printf("Skipping by %zu chars\n", token.read_length);
		vec_push_copy(minishell, &tokens_vec, &token);
	}
	tokens_to_array(minishell, &tokens_vec);
	//! For debugging
	// vec_print_as_tokens(&tokens_vec);
	// print_tokenized(minishell->tokenized);
	return (1);
}
