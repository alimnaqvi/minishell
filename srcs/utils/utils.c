/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 12:10:25 by anaqvi            #+#    #+#             */
/*   Updated: 2024/12/25 12:30:47 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_array_size(char **arr)
{
	int	len;

	if (!arr)
		return (0);
	len = 0;
	while (arr[len])
		len++;
	return (len);
}

char	**copy_2d_char_arr(char **arr, t_minishell *minishell)
{
	char	**result;
	int		arr_len;
	int		i;

	if (!arr)
		return (NULL);
	arr_len = get_array_size(arr);
	result = gc_malloc((sizeof(char *) * arr_len) + 1, minishell);
	i = 0;
	while (arr[i])
	{
		result[i] = ft_strdup(arr[i]);
		if (!result[i])
		{
			ft_putendl_fd("malloc failed", STDERR_FILENO);
			gc_exit(minishell, EXIT_FAILURE);
		}
		gc_add_to_allocs(result[i], minishell);
		i++;
	}
	result[i] = NULL;
	return (result);
}
