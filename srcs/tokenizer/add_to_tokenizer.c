/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_tokenizer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 16:03:13 by rreimann          #+#    #+#             */
/*   Updated: 2025/02/16 17:10:55 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*str_cpy(t_minishell *minishell, char *str)
{
	size_t	str_len;
	char	*new_str;
	size_t	index;

	str_len = ft_strlen(str);
	new_str = gc_malloc(sizeof(char) * (str_len + 1), minishell);
	index = 0;
	while (index < str_len)
	{
		new_str[index] = str[index];
		index++;
	}
	new_str[index] = 0;
	return (new_str);
}

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
	new_tokenized[old_length++] = str_cpy(minishell, str);
	new_tokenized[old_length] = NULL;
	minishell->tokenized = new_tokenized;
}

void	tokens_to_array(t_minishell *minishell, t_vec *vec)
{
	size_t	index;
	t_token	*token;
	char	*previous_string_pointer;
	char	*new_string;

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
			if (ft_strlen(token->string) > 0)
			{
				add_to_tokenized(minishell, token->string);
				previous_string_pointer = get_last_token(minishell);
			}
		}
		else
		{
			new_string = gc_ft_strjoin(previous_string_pointer,
					token->string, minishell);
			gc_free(previous_string_pointer, minishell);
			previous_string_pointer = new_string;
			replace_last_token(minishell, previous_string_pointer);
		}
	}
}
