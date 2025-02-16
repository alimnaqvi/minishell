/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_environment_variables.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:57:18 by rreimann          #+#    #+#             */
/*   Updated: 2025/02/16 18:55:44 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*replace_environment_variables(t_minishell *minishell, char *input)
{
	char	*new_string;
	size_t	str_len;
	size_t	index;

	new_string = NULL;
	str_len = ft_strlen(input);
	ft_strlcpy(new_string, input, str_len);
	index = 0;
	while (minishell->mini_env[index])
		printf("Env %zu: %s\n", index, minishell->mini_env[index]); // is this for debugging?
	return (new_string);
}
