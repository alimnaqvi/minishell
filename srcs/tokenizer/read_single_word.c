/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_single_word.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 16:39:09 by rreimann          #+#    #+#             */
/*   Updated: 2025/01/21 18:18:04 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Use this function to determine how long the word will be
// This is needed for us to be able to know how much memory to allocate
// ...for the word
// Those characters could be: ' ', '\t', '\0'
//! There will also be the $ thingy with the environment variables, but we can handle this later
static size_t	count_word_length(size_t index, char *input)
{
	int	start_index;

	start_index = index;
	while (input[index] != '\0' && input[index] != '\t' && input[index] != ' ')
		index++;
	return (index - start_index);
}

// Essentially in this function we keep on reading something as a word, until we reach one of the specified characters
// This function will return the length of the word that it found
// asdasd -> 5
t_returned_word	read_single_word(size_t index, t_minishell *minishell)
{
	t_returned_word	returned_word;

	returned_word.length = count_word_length(index, minishell->input);

	returned_word.word =\
		ft_substr(minishell->input, index, returned_word.length);
	if (returned_word.word != NULL)
		gc_add_to_allocs(returned_word.word, minishell);

	return (returned_word);
}
