/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_tokenizer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 16:03:13 by rreimann          #+#    #+#             */
/*   Updated: 2025/02/17 04:14:20 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_last_token(t_minishell *minishell)
{
	size_t	index;

	index = 0;
	while (minishell->tokenized[index] != NULL)
	{
		index++;
	}
	index--;
	return (minishell->tokenized[index]);
}

void	replace_last_token(t_minishell *minishell, char *replacement)
{
	size_t	index;

	index = 0;
	while (minishell->tokenized[index] != NULL)
	{
		index++;
	}
	index--;
	minishell->tokenized[index] = replacement;
}

// The passed `str` must always be a valid C string, 
// and have at least one character
void	add_to_tokenized(t_minishell *minishell, char *str)
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
	new_tokenized[old_length++] = gc_malloc_str(minishell, str);
	new_tokenized[old_length] = NULL;
	minishell->tokenized = new_tokenized;
}

void	tokens_to_array(t_minishell *minishell, t_vec *vec)
{
	size_t	index;
	t_token	*token;
	char	*previous_string_pointer;

	previous_string_pointer = NULL;
	index = 0;
	while (index < vec->length)
	{
		token = vec_get(vec, index++);
		if (token->type == TOKEN_SPACE)
			previous_string_pointer = NULL;
		else if (token->type == TOKEN_OPERATOR)
		{
			previous_string_pointer = NULL;
			add_to_tokenized(minishell, token->string);
		}
		else if (previous_string_pointer == NULL)
		{
			if (ft_strlen(token->string) == 0 && index == 1)
				continue ;
			second_helper(minishell, token, &previous_string_pointer);
		}
		else
			helper_function(&previous_string_pointer, token, minishell);
	}
}
