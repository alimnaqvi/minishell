/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokenized.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 17:01:05 by rreimann          #+#    #+#             */
/*   Updated: 2025/01/13 17:01:21 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokenized(char **tokenized)
{
	int	index;

	index = 0;
	while (tokenized[index] != NULL)
	{
		printf("%d tokenized: '%s'\n", index, tokenized[index]);
		index++;
	}
}
