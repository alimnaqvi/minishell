/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 15:54:06 by anaqvi            #+#    #+#             */
/*   Updated: 2024/10/23 19:36:11 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	handle_specifiers_start(const char *format, va_list *arg,
		int *char_count)
{
	int	chars_written;

	if (*(format + 1) == 'c')
		chars_written = handle_char(arg, char_count);
	else if (*(format + 1) == 's')
		chars_written = handle_str(arg, char_count);
	else if (*(format + 1) == 'p')
		chars_written = handle_ptr(arg, char_count);
	else if (*(format + 1) == 'd' || *(format + 1) == 'i')
		chars_written = handle_num(arg, char_count);
	else if (*(format + 1) == 'u')
		chars_written = handle_unsigned(arg, char_count);
	else if (*(format + 1) == 'X')
		chars_written = handle_hex_upper(arg, char_count);
	else if (*(format + 1) == 'x')
		chars_written = handle_hex_lower(arg, char_count);
	else if (*(format + 1) == '%')
		chars_written = handle_percent(char_count);
	else
		chars_written = handle_other(format, char_count);
	return (chars_written);
}

int	ft_printf(const char *format, ...)
{
	va_list	arg;
	int		char_count;

	char_count = 0;
	va_start(arg, format);
	while (*format)
	{
		if (*format == '%')
		{
			if (handle_specifiers_start(format, &arg, &char_count) == -1)
				return (va_end(arg), -1);
			format += 2;
		}
		else
		{
			if (write(1, format, 1) == -1)
				return (va_end(arg), -1);
			char_count++;
			format++;
		}
	}
	return (va_end(arg), char_count);
}
