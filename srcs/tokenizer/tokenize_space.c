/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_space.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 20:05:12 by rreimann          #+#    #+#             */
/*   Updated: 2025/01/22 20:33:53 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	count_space(char *input, size_t index)
{
	size_t	start;

	start = index;
	while (input[index] && ft_isspace(input[index]))
		index++;
	return (index - start);
}

t_token	tokenize_space(t_minishell *minishell, size_t index)
{
	t_token	token;

	token.read_length = count_space(minishell->input, index);
	token.parsed_length = token.read_length;
	token.type = TOKEN_SPACE;
	token.string = ft_substr(minishell->input, index, token.read_length);
	if (token.string == NULL)
		gc_exit(minishell, EXIT_FAILURE);
	gc_add_to_allocs(token.string, minishell);
	return (token); // Creating a copy of the token
}
