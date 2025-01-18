/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:50:55 by rreimann          #+#    #+#             */
/*   Updated: 2025/01/18 14:54:38 by rreimann         ###   ########.fr       */
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
	int		env_length;
	int		allocation_length;
	char	*output_string;
	char	*env_string;

	// Get the new required length for the string (together with variables replaced)
	allocation_length = 0;
	index = 0;
	// 123$NAME$123
	// index = 3
	/*
		Variable ends when there is anything other than an alphanumeric character or '_'

	*/
	// Loop through the entire string
	while (str[index] != 0)
	{
		// We found the start of an environment variable!
		if (str[index] == '$')
		{
			// Let's find the length of that environment variable! 
			env_length = 0;
			while (is_char_env_valid(str[index + 1 + env_length])) {
				env_length++;
			}
			// So here env_index will be 4
			// And index will be 3
			env_string = ft_substr(str, index + 1, env_length);

			// And now that we have the env string, we can essentially find it and get the value for it
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
		// If it has reach the space, it needs to know
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
