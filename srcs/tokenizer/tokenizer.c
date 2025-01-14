/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:50:55 by rreimann          #+#    #+#             */
/*   Updated: 2025/01/14 17:05:33 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_env_variable(t_minishell *minishell, char *key)
{
	
}

int	is_char_env_valid(char c)
{
	return (ft_isalnum(c) || c == '_');
}

// Replace any variables within the passed string
// Variables are anything that starts with the dollar sign $
// Create a new string with the replaced variables and return it
char	*replace_variables(t_minishell *minishell, char *str)
{
	int		index;
	int		env_index;
	int		allocation_length;
	char	*output_str;

	// Get the new required length for the string (together with variables replaced)
	allocation_length = 0;
	index = 0;
	// 123$NAME123
	// index = 3
	while (str[index] != 0)
	{
		ft_substr
		// The start of the env variable
		if (str[index] == '$')
		{
			env_index = 0;
			while (is_char_env_valid(str[index]))
			{
				
			}
		}
		index++;
	}
}

// The passed `str` must always be a valid C string, 
// and have at least one character
static void	add_to_tokenizer(t_minishell *minishell, char *str)
{
	int		old_length;
	char	**new_tokenized;

	old_length = 0;
	while (minishell->tokenized != NULL && minishell->tokenized[old_length] != NULL)
		old_length++;
	new_tokenized = gc_malloc(sizeof(char *) * (old_length + 2), minishell);
	old_length = 0;
	if (minishell->tokenized != NULL)
	{
		while (minishell->tokenized[old_length] != NULL)
		{
			new_tokenized[old_length] = minishell->tokenized[old_length];
			old_length++;
		}
		gc_free(minishell->tokenized, minishell);
	}
	new_tokenized[old_length++] = str;
	new_tokenized[old_length] = NULL;
	minishell->tokenized = new_tokenized;
}

// This function reads the string as the input
// Expecting the character at the given index to be '"'
// Start reading the input from the given index
// Once we have ended the reading, malloc as many characters as we need
// And the copy the characters over into the allocated string
// Save the new string into the `tokenized` array, somehow
// There is basically a pointer to the array
// And the array itself is also a pointer, to the very first element of the array
static void read_from_quote(int *index, t_minishell *minishell)
{
	int		length;
	char	*new_str;
	int		str_index;
	char	*input;
	char	*replaced_variables;

	input = minishell->input;
	(*index)++;
	length = 0;
	while (input[(*index) + length] != 0 && input[(*index) + length] != '"')
		length++;
	new_str = gc_malloc(sizeof(char) * (length + 1), minishell);
	str_index = 0;
	while (str_index < length)
	{
		new_str[str_index] = input[str_index + (*index)];
		str_index++;
	}
	new_str[str_index] = 0;
	(*index) += length + 1;
	replaced_variables = replace_variables(minishell, new_str);
	gc_free(new_str, minishell);
	add_to_tokenizer(minishell, replaced_variables);
}

// echo "asd" > something| $USER$USER<<'asd' |
int tokenizer(t_minishell *minishell)
{
	int index;

	if (!minishell)
		return (-1);
	index = 0;
	while (minishell->input[index] != 0)
	{
		if (minishell->input[index] == '"')
		{
			// Read starting from the quote
			// Until we reach the next quote
			read_from_quote(&index, minishell);
			index++;
			continue;
		}
		else if (minishell->input[index] == '\'')
		// This does the same as "
		// But without converting the $ variables
		{
			index++;
			continue;
		}
		// Do that
		else if (minishell->input[index] == '<')
		{
			index++;
			continue;
		}
		else if (minishell->input[index] == '>')
		{
			index++;
			continue;
		}
		else if (minishell->input[index] == '|')
		{
			index++;
			continue;
		}
		else if (minishell->input[index] == ' ')
		// If it has reache the space, it needs to know
		// Whether or not the quote is in progress
		// To know if we should inlude the space

		// In this case, the quote will probably not be in progress
		// Because we have an entirely different function just for reading in the quotes
		{
			index++;
			continue;
		}
		else
		{
			index++;
			continue;
		}
	}
	print_tokenized(minishell->tokenized);
	return (1);
}
