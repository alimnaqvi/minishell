/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:17:35 by rreimann          #+#    #+#             */
/*   Updated: 2025/01/23 13:46:13 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	get_next_token(t_minishell *minishell, size_t index)
{
	if (ft_isspace(minishell->input[index]))
		return (tokenize_space(minishell, index));
	else if (ft_isquote(minishell->input[index]))
		return (tokenize_quote(minishell, index, minishell->input[index]));
	else if (ft_isoperator(minishell->input[index]))
		return (tokenize_operator(minishell, index));
	else
		return (tokenize_word(minishell, index));
}
