/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 18:21:07 by rreimann          #+#    #+#             */
/*   Updated: 2025/01/21 18:23:28 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	vec_print_as_strings(t_vec *vec)
{
	size_t	index;
	char	*vec_element;

	index = 0;
	while (index < vec->length)
	{
		vec_element = vec_get(vec, index);
		printf("Vec string: %s\n", vec_element);
		index++;
	}
}