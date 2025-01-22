/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 21:10:02 by rreimann          #+#    #+#             */
/*   Updated: 2025/01/22 21:10:51 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	token_init()
{
	t_token	token;

	token.parsed_length = 0;
	token.read_length = 0;
	token.string = NULL;
	token.type = TOKEN_SPACE;
}
