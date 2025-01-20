/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:50:55 by rreimann          #+#    #+#             */
/*   Updated: 2025/01/20 18:01:43 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char c)
{
	return (c == '\t' || c == '\n' ||  c == '\v' ||\
		c == '\f' || c == '\r' || c == ' ');
}

// EXAMPLE: echo "asd" > something| $USER$USER<<'asd' |
// This is the function that will be tokenizing everything
// Essentially just splits everything into strings
// By the rules that we have set in the Minishell
// 1. Skip all whitespaces that are outside of quotes
// 2. 
int tokenizer(t_minishell *minishell)
{
	int index;
	int read_length;

	if (!minishell)
		return (-1);
	index = 0;
	while (index < 10)
	{
		printf("Index: %d, char: %c\n", index, minishell->input[index]);
		index++;
	}
	index = 0;
	while (minishell->input[index] != 0)
	{
		// 1. Skip all whitespaces that are outside of quotes
		if (ft_isspace(minishell->input[index]))
			index++;
		// Double quotes stuff
		//! We could change this a bit so that it would start counting from the char after the first quote
		//! And then it would simply know that it will stop the reading once it has reached the next quote
		else if (minishell->input[index] == '"')
		{
			read_from_quote(&index, minishell);
			index++;
			continue;
		}
		// Essentially if it is anything else, simply treat it as a single word
		else
		{
			read_length = read_single_word(index, minishell);
			index += read_length;
		}
	}
	print_tokenized(minishell->tokenized);

	return (1);
}
