/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_ptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:14:14 by anaqvi            #+#    #+#             */
/*   Updated: 2024/10/22 20:03:57 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putaddress(void *ptr)
{
	int			to_write;
	int			chars_written;
	char		*buffer;
	uintptr_t	p;
	int			addr_len;

	p = (uintptr_t)ptr;
	addr_len = get_address_length(p);
	buffer = malloc(2 + addr_len);
	if (!buffer)
		return (-1);
	to_write = convert_to_hex(p, &buffer, addr_len);
	if (to_write == -1 || to_write != (2 + addr_len))
		return (free(buffer), -1);
	chars_written = write(1, buffer, to_write);
	free(buffer);
	return (chars_written);
}

int	convert_to_hex(uintptr_t p, char **buffer, int addr_len)
{
	char	*hex_digits;
	int		i;
	int		to_write;

	hex_digits = "0123456789abcdef";
	to_write = 0;
	(*buffer)[to_write++] = '0';
	(*buffer)[to_write++] = 'x';
	if (p == 0)
		(*buffer)[to_write++] = '0';
	else
	{
		i = 2 + addr_len;
		while (p > 0)
		{
			(*buffer)[--i] = hex_digits[p % 16];
			p /= 16;
		}
		to_write += addr_len;
	}
	return (to_write);
}

int	get_address_length(uintptr_t p)
{
	int	len;

	if (p == 0)
		return (1);
	len = 0;
	while (p > 0)
	{
		p /= 16;
		len++;
	}
	return (len);
}
