/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 00:50:18 by rreimann          #+#    #+#             */
/*   Updated: 2025/02/17 00:50:25 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokenized(char **tokenized)
{
	int	index;

	if (tokenized == NULL)
		return ;
	index = 0;
	while (tokenized[index] != NULL)
	{
		printf("%d tokenized: '%s'\n", index, tokenized[index]);
		index++;
	}
}
