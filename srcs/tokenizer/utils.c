/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:18:43 by rreimann          #+#    #+#             */
/*   Updated: 2025/02/13 18:46:41 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char c) // likely need to remove \n here to match bash
{
	return (c == '\t' || c == '\n' || c == '\v' || \
		c == '\f' || c == '\r' || c == ' ');
}

int	ft_isquote(char c)
{
	return (c == '\'' || c == '"');
}
