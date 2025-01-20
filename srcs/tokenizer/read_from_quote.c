/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_from_quote.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 16:01:57 by rreimann          #+#    #+#             */
/*   Updated: 2025/01/20 16:37:01 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
void read_from_quote(int *index, t_minishell *minishell)
{
	int		length;
	char	*new_str;
	int		str_index;
	char	*input;

	input = minishell->input;
	// We increase the index to make sure we start from the next character instead of the first '"'
	(*index)++;
	// Here we simply count all the characters we have until either the end of the string, or the end of the quote
	length = 0;
	while (input[(*index) + length] != 0 && input[(*index) + length] != '"')
		length++;
	// Here we create a new string and we copy the characters over into it from input
	new_str = gc_malloc(sizeof(char) * (length + 1), minishell);
	str_index = 0;
	while (str_index < length)
	{
		new_str[str_index] = input[str_index + (*index)];
		str_index++;
	}
	// Don't forget the null terminator
	new_str[str_index] = 0;
	// Visualization:
	/*
		-> this "is a" string!
		the first " is at index 5
		the length will be found as 4
		we malloc for 4 + 1 = 5 (for the null terminator as well)
		then we set all characters for the new string from input

	*/
	(*index) += length;
	// Oh yeah we also need to replace the variables that exist in the string we got
	// replaced_variables = replace_variables(minishell, new_str);
	add_to_tokenizer(minishell, new_str);
}
