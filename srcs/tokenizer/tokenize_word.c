/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 16:39:09 by rreimann          #+#    #+#             */
/*   Updated: 2025/01/22 21:15:47 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	count_word_length(char *input, size_t index)
{
	int	start_index;

	start_index = index;
	while (input[index] != '\0' &&\
			input[index] != ' ' &&\
			input[index] != '\t' &&\
			input[index] != '<' &&\
			input[index] != '>' &&\
			input[index] != '|' &&\
			input[index] != '"' &&\
			input[index] != '\'' &&\
			input[index] != '$')
		index++;
	return (index - start_index);
}

t_token	tokenize_word(t_minishell *minishell, size_t index)
{
	t_token	token;

	token.read_length = count_word_length(minishell->input, index);
	token.parsed_length = token.read_length;
	token.type = TOKEN_WORD;
	token.string = ft_substr(minishell->input, index, token.read_length);
	if (token.string == NULL)
		gc_exit(minishell, EXIT_FAILURE);
	gc_add_to_allocs(token.string, minishell);
	return (token);
}
