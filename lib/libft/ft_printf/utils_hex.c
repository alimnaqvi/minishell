/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_hex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:29:57 by anaqvi            #+#    #+#             */
/*   Updated: 2024/10/23 17:58:23 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_puthex_upper(unsigned int x)
{
	char			to_write;
	char			*hex_digits;
	int				chars_written;
	unsigned int	num;

	chars_written = 0;
	num = x;
	if (num >= 16)
	{
		chars_written += ft_puthex_upper(num / 16);
		if (chars_written == -1)
			return (-1);
	}
	hex_digits = "0123456789ABCDEF";
	to_write = hex_digits[num % 16];
	if (write(1, &to_write, 1) == -1)
		return (-1);
	return (chars_written + 1);
}

int	ft_puthex_lower(unsigned int x)
{
	char			to_write;
	char			*hex_digits;
	int				chars_written;
	unsigned int	num;

	chars_written = 0;
	num = x;
	if (num >= 16)
	{
		chars_written += ft_puthex_lower(num / 16);
		if (chars_written == -1)
			return (-1);
	}
	hex_digits = "0123456789abcdef";
	to_write = hex_digits[num % 16];
	if (write(1, &to_write, 1) == -1)
		return (-1);
	return (chars_written + 1);
}

int	count_digits_hex(unsigned int x)
{
	int	count;

	count = 0;
	if (x == 0)
		return (1);
	while (x != 0)
	{
		x /= 16;
		count++;
	}
	return (count);
}
