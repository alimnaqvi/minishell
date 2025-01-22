/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 18:21:07 by rreimann          #+#    #+#             */
/*   Updated: 2025/01/22 21:27:49 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*vec_get_string(t_vec *vec, size_t index)
{
	return (*(char **)vec_get(vec, index));
}

void	vec_print_as_strings(t_vec *vec)
{
	size_t	index;
	char	*vec_element;

	index = 0;
	while (index < vec->length)
	{
		vec_element = vec_get_string(vec, index);
		printf("Vec string: %s\n", vec_element);
		index++;
	}
}

const char	*token_types[] = {
	[TOKEN_WORD] = "TOKEN_WORD",
	[TOKEN_OPERATOR] = "TOKEN_OPERATOR",
	[TOKEN_SPACE] = "TOKEN_SPACE"
};

void	vec_print_as_tokens(t_vec *vec)
{
	size_t	index;
	t_token	*token;

	index = 0;
	while (index < vec->length)
	{
		token = vec_get(vec, index);
		printf("Token %zu (%s): '%s'\n", index,\
			token_types[token->type], token->string);
		index++;
	}
}
