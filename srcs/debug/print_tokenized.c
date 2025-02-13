/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokenized.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 17:01:05 by rreimann          #+#    #+#             */
/*   Updated: 2025/02/13 18:47:34 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokenized(char **tokenized) // remember to delete
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
