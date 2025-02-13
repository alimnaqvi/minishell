/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_tokenizer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 16:03:13 by rreimann          #+#    #+#             */
/*   Updated: 2025/02/13 15:42:13 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_tokenizer(t_minishell *minishell)
{
	char	*tokenizer_terminator;

	tokenizer_terminator = gc_malloc(sizeof(char *), minishell);
	tokenizer_terminator = NULL;
}

// The passed `str` must always be a valid C string, 
// and have at least one character
void	add_to_tokenizer(t_minishell *minishell, char *str)
{
	int		old_length;
	char	**new_tokenized;

	old_length = 0;
	while (minishell->tokenized != NULL && \
		minishell->tokenized[old_length] != NULL)
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

void	tokens_to_array(t_minishell *minishell, t_vec *vec)
{
	size_t	index;
	t_token	*token;

	index = 0;
	while (index < vec->length)
	{
		token = vec_get(vec, index);
		if (token->type == TOKEN_SPACE)
		{
			index++;
			continue ;
		}
		add_to_tokenizer(minishell, token->string);
		index++;
	}
}
