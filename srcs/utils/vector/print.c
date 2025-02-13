/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 18:21:07 by rreimann          #+#    #+#             */
/*   Updated: 2025/02/13 17:22:27 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*vec_get_string(t_vec *vec, size_t index)
{
	return (*(char **)vec_get(vec, index));
}

void	vec_print_as_strings(t_vec *vec) // remeber to delete
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

static const char	*g_token_types[] = {[TOKEN_WORD] = "TOKEN_WORD", // remeber to delete
[TOKEN_OPERATOR] = "TOKEN_OPERATOR", [TOKEN_SPACE] = "TOKEN_SPACE"};

void	vec_print_as_tokens(t_vec *vec)  // remeber to delete
{
	size_t	index;
	t_token	*token;

	index = 0;
	while (index < vec->length)
	{
		token = vec_get(vec, index);
		printf("Token %zu (%s): '%s'\n", index, g_token_types[token->type],
			token->string);
		index++;
	}
}
