/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:42:29 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/16 16:54:56 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*token_is_unsupp_op(char *token)
{
	if (!ft_strncmp(token, "||", 3))
		return ("||");
	if (!ft_strncmp(token, "&&", 3))
		return ("&&");
	if (!ft_strncmp(token, "\n", 2))
		return ("\\n (newline)");
	return (NULL);
}

int	is_external_cmd(char *cmd_name)
{
	if (!ft_strncmp(cmd_name, "echo", 5)
		|| !ft_strncmp(cmd_name, "cd", 3)
		|| !ft_strncmp(cmd_name, "pwd", 4)
		|| !ft_strncmp(cmd_name, "export", 7)
		|| !ft_strncmp(cmd_name, "unset", 6)
		|| !ft_strncmp(cmd_name, "env", 4)
		|| !ft_strncmp(cmd_name, "exit", 5)
		|| (ft_strchr(cmd_name, '=') && *cmd_name != '='))
	{
		return (0);
	}
	return (1);
}
