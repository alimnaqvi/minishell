/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_operator.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:23:08 by rreimann          #+#    #+#             */
/*   Updated: 2025/02/13 15:44:33 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_arrow(char c)
{
	return (c == '<' || c == '>');
}

static int	is_pipe(char c)
{
	return (c == '|');
}

int	ft_isoperator(char c)
{
	return (is_arrow(c) || is_pipe(c));
}

// We need this function only for the arrows, since pipes are only a single char
int	count_arrow_length(char *input, size_t index, char arrow)
{
	size_t	old_index;

	old_index = index;
	while ((index - old_index) < 2 && input[index] == arrow)
		index++;
	return (index - old_index);
}

t_token	tokenize_operator(t_minishell *minishell, size_t index)
{
	t_token	token;
	char	char_at_index;

	char_at_index = minishell->input[index];
	if (is_arrow(char_at_index))
		token.read_length = count_arrow_length(minishell->input, \
				index, char_at_index);
	else if (is_pipe(char_at_index))
		token.read_length = 1;
	token.parsed_length = token.read_length;
	token.string = ft_substr(minishell->input, index, token.read_length);
	if (token.string == NULL)
		gc_exit(minishell, EXIT_FAILURE);
	gc_add_to_allocs(token.string, minishell);
	token.type = TOKEN_OPERATOR;
	return (token);
}
