/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_specifiers2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 20:08:18 by anaqvi            #+#    #+#             */
/*   Updated: 2024/10/24 11:58:27 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	handle_hex_upper(va_list *arg, int *char_count)
{
	unsigned int	x;
	int				chars_written;
	int				digit_count;

	x = va_arg(*arg, unsigned int);
	chars_written = ft_puthex_upper(x);
	digit_count = count_digits_hex(x);
	if (chars_written == -1 || chars_written != digit_count)
		return (-1);
	*char_count += chars_written;
	return (chars_written);
}

int	handle_hex_lower(va_list *arg, int *char_count)
{
	unsigned int	x;
	int				chars_written;
	int				digit_count;

	x = va_arg(*arg, unsigned int);
	chars_written = ft_puthex_lower(x);
	digit_count = count_digits_hex(x);
	if (chars_written == -1 || chars_written != digit_count)
		return (-1);
	*char_count += chars_written;
	return (chars_written);
}

int	handle_percent(int *char_count)
{
	int	chars_written;

	chars_written = write(1, "%", 1);
	if (chars_written == -1)
		return (-1);
	*char_count += chars_written;
	return (chars_written);
}

int	handle_other(const char *format, int *char_count)
{
	int	chars_written;

	chars_written = 0;
	if (write(1, format, 1) == -1)
		return (-1);
	chars_written++;
	if (*(format + 1))
	{
		if (write(1, format + 1, 1) == -1)
			return (-1);
		chars_written++;
	}
	*char_count += chars_written;
	return (chars_written);
}
