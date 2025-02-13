/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_libft.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:39:50 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/13 15:37:20 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*gc_ft_substr(char *s, unsigned int start, size_t len,
t_minishell *minishell)
{
	char	*result;

	result = ft_substr(s, start, len);
	if (!result)
	{
		ft_putendl_fd("malloc failed", STDERR_FILENO);
		gc_exit(minishell, EXIT_FAILURE);
	}
	gc_add_to_allocs(result, minishell);
	return (result);
}

char	*gc_ft_strjoin(char *s1, char *s2, t_minishell *minishell)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	if (!result)
	{
		ft_putendl_fd("malloc failed", STDERR_FILENO);
		gc_exit(minishell, EXIT_FAILURE);
	}
	gc_add_to_allocs(result, minishell);
	return (result);
}

char	*gc_ft_strdup(char *s1, t_minishell *minishell)
{
	char	*result;

	result = ft_strdup(s1);
	if (!result)
	{
		ft_putendl_fd("malloc failed", STDERR_FILENO);
		gc_exit(minishell, EXIT_FAILURE);
	}
	gc_add_to_allocs(result, minishell);
	return (result);
}

static int	determine_sign(char *str)
{
	if (*str == '-')
		return (-1);
	return (1);
}

int	ft_atoi_error(char *str, int *num)
{
	long long	result;
	int			sign;

	result = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	sign = determine_sign(str);
	if (*str == '-' || *str == '+')
		str++;
	if (!*str)
		return (-1);
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
			result = result * 10 + (*str - '0');
		else
			return (-1);
		if ((sign == 1 && result > INT_MAX)
			|| (sign == -1 && (-1 * result) < INT_MIN))
			return (-1);
		str++;
	}
	*num = result * sign;
	return (0);
}
