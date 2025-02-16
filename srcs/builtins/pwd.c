/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 19:13:21 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/16 16:09:29 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(t_minishell *minishell)
{
	char	buffer[PATH_MAX];

	if (!getcwd(buffer, PATH_MAX))
	{
		perror("minishell: pwd");
		return (gc_exit(minishell, EXIT_FAILURE));
	}
	printf("%s\n", buffer);
	gc_exit(minishell, EXIT_SUCCESS);
}
