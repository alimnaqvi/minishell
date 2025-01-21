/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:50:55 by rreimann          #+#    #+#             */
/*   Updated: 2025/01/21 20:36:56 by rreimann         ###   ########.fr       */
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
	size_t			index;
	t_returned_word	returned_word;
	t_vec			tokenizer_result;

	if (!minishell)
		return (-1);
	tokenizer_result = vec_init(sizeof(char *));
	index = 0;
	while (minishell->input[index])
	{
		// 1. Skip all whitespaces that are outside of quotes
		if (ft_isspace(minishell->input[index]))
			index++;
		// Double quotes stuff
		//! We could change this a bit so that it would start counting from the char after the first quote
		//! And then it would simply know that it will stop the reading once it has reached the next quote
		else if (minishell->input[index] == '"')
		{
			returned_word = read_from_quote(index, minishell);
			if (returned_word.word == NULL)
				break;
			printf(": Word in quotes: %s\n", returned_word.word);
			printf("Length of word: %zu\n", returned_word.length);
			vec_push(minishell, &tokenizer_result, returned_word.word);
			index += returned_word.length;
		}
		// Essentially if it is anything else, simply treat it as a single word
		else
		{
			returned_word = read_single_word(index, minishell);
			if (returned_word.word == NULL)
				break;
			printf(": Word without quotes: %s\n", returned_word.word);
			vec_push(minishell, &tokenizer_result, returned_word.word);
			index += returned_word.length;
		}
	}
	vec_print_as_strings(&tokenizer_result);

	return (1);
}
