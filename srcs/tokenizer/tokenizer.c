/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:50:55 by rreimann          #+#    #+#             */
/*   Updated: 2025/01/10 17:51:12 by rreimann         ###   ########.fr       */
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
void	read_from_quote(char *input, int *index, char **tokenized)
{
	
}

// echo "asd" > something| $USER$USER<<'asd' |
int	tokenizer(t_minishell *minishell)
{
	int		index;
	char	*tokenized;

	if (!minishell)
		return (-1);
	index = 0;
	while (minishell->input[index++] != 0)
	{
		if (minishell->input[index] == '"')
		{
			// Read starting from the quote
			// Until we reach the next quote
			read_from_quote(minishell->input, &index, &tokenized);
			continue ;
		}
		else if (minishell->input[index] == '\'')
			// This does the same as "
			// But without converting the $ variables
			continue ;
		else if (minishell->input[index] == '<')
			// Do that
			continue ;
		else if (minishell->input[index] == '>')
			continue ;
		else if (minishell->input[index] == '|')
			continue ;
		else if (minishell->input[index] == ' ')
			// If it has reache the space, it needs to know
			// Whether or not the quote is in progress
			// To know if we should inlude the space
			continue ;
		else
			continue ;
	}
}
