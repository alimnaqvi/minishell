/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 16:39:09 by rreimann          #+#    #+#             */
/*   Updated: 2025/02/13 15:46:16 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	count_word_length(char *input, size_t index)
{
	int	start_index;

	start_index = index;
	while (input[index] != '\0' && \
			input[index] != ' ' && \
			input[index] != '\t' && \
			input[index] != '<' && \
			input[index] != '>' && \
			input[index] != '|' && \
			input[index] != '"' && \
			input[index] != '\'')
		index++;
	return (index - start_index);
}

//! When tokenizing a word, we must also tokenize a $HOME for example
//! With the current system, this would not get tokenized, because...
//! ...we end the word at the $ sign
//! ...but what if it begins with the $ sign? Huh? ;) 
// On a second thought... WHY would we have to end the word at $ anyways? 
// Like sure we would end it at a space
// But we will later on feed the entire string to the variable machine anyways
// Let's think about this the next time! 
t_token	tokenize_word(t_minishell *minishell, size_t index)
{
	t_token	token;
	char	*replaced_str;

	token.read_length = count_word_length(minishell->input, index);
	token.parsed_length = token.read_length;
	token.type = TOKEN_WORD;
	token.string = ft_substr(minishell->input, index, token.read_length);
	if (token.string == NULL)
		gc_exit(minishell, EXIT_FAILURE);
	gc_add_to_allocs(token.string, minishell);
	replaced_str = replace_env(minishell, token.string);
	gc_free(token.string, minishell);
	token.string = replaced_str;
	return (token);
}
