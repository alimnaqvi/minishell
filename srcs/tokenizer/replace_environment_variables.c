/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_environment_variables.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:57:18 by rreimann          #+#    #+#             */
/*   Updated: 2025/01/23 14:02:01 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*replace_environment_variables(t_minishell *minishell, char *input)
{
	char	*new_string;
	size_t	str_len;

	new_string = NULL;
	str_len = ft_strlen(input);
	ft_strlcpy(new_string, input, str_len);

	size_t index = 0;
	while (minishell->mini_env[index])
	{
		printf("Env %zu: %s\n", index, minishell->mini_env[index]);
	}

	return (new_string);
}
