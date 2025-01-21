/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_from_quote.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 16:01:57 by rreimann          #+#    #+#             */
/*   Updated: 2025/01/21 18:55:33 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct s_count_with_quote
{
	size_t	count;
	int		ends_with_quote;
};

static struct s_count_with_quote	count_until_quote(size_t index, char *input)
{
	size_t						old_index;
	struct s_count_with_quote	return_value;

	return_value.ends_with_quote = 0;
	old_index = index;
	while (input[index] && input[index] != '"')
		index++;
	if (input[index] == '"')
		return_value.ends_with_quote = 1;
	return_value.count = index - old_index;
	return (return_value);
}

// This function reads the string as the input
// Expecting the character at the given index to be '"'
// Start reading the input from the given index
// Once we have ended the reading, malloc as many characters as we need
// And the copy the characters over into the allocated string
// Save the new string into the `tokenized` array, somehow
// There is basically a pointer to the array
// And the array itself is also a pointer, to the very first element of the array
// The `read_from_quote` function takes also the pointer to the index...
// ...therefore, it can skip the idnex by as many characters forward as it needs
t_returned_word	read_from_quote(size_t index, t_minishell *minishell)
{
	t_returned_word				returned_word;
	struct s_count_with_quote	counted_value;

	index++;
	counted_value = count_until_quote(index, minishell->input);
	returned_word.length = counted_value.count;
	returned_word.word = ft_substr(minishell->input, index, returned_word.length);
	if (returned_word.word != NULL)
		gc_add_to_allocs(returned_word.word, minishell);
	returned_word.length++;
	if (counted_value.ends_with_quote)
		returned_word.length++;
	return (returned_word);
}
