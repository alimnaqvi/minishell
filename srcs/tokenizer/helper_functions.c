/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 04:12:15 by rreimann          #+#    #+#             */
/*   Updated: 2025/02/17 04:22:17 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	helper_function(char **previous_string_pointer, t_token	*token, \
	t_minishell *minishell)
{
	char	*new_string;

	new_string = gc_ft_strjoin(*previous_string_pointer,
			token->string, minishell);
	gc_free(*previous_string_pointer, minishell);
	*previous_string_pointer = new_string;
	replace_last_token(minishell, *previous_string_pointer);
}

void	second_helper(t_minishell *minishell, t_token *token, \
	char **previous_string_pointer)
{
	add_to_tokenized(minishell, token->string);
	*previous_string_pointer = get_last_token(minishell);
}
