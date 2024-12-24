/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_specifiers1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 13:24:54 by anaqvi            #+#    #+#             */
/*   Updated: 2024/10/24 11:56:45 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	handle_char(va_list *arg, int *char_count)
{
	char	c;

	c = (char)va_arg(*arg, int);
	if (write(1, &c, 1) == -1)
		return (-1);
	*char_count += 1;
	return (1);
}

int	handle_str(va_list *arg, int *char_count)
{
	int		len;
	char	*str;

	len = 0;
	str = va_arg(*arg, char *);
	if (str == NULL)
	{
		if (write(1, "(null)", 6) == -1)
			return (-1);
		*char_count += 6;
		return (6);
	}
	while (str[len])
		if (write(1, &str[len++], 1) == -1)
			return (-1);
	*char_count += len;
	return (len);
}

int	handle_ptr(va_list *arg, int *char_count)
{
	void	*p;
	int		chars_written;

	p = va_arg(*arg, void *);
	chars_written = ft_putaddress(p);
	if (chars_written == -1)
		return (-1);
	*char_count += chars_written;
	return (chars_written);
}

int	handle_num(va_list *arg, int *char_count)
{
	int	n;
	int	chars_written;
	int	digit_count;

	n = va_arg(*arg, int);
	chars_written = ft_putnbr_err(n);
	digit_count = count_digits_signed(n);
	if (chars_written == -1 || chars_written != digit_count)
		return (-1);
	*char_count += chars_written;
	return (chars_written);
}

int	handle_unsigned(va_list *arg, int *char_count)
{
	unsigned int	n;
	int				chars_written;
	int				digit_count;

	n = va_arg(*arg, unsigned int);
	chars_written = ft_putnbr_err_unsigned(n);
	digit_count = count_digits_unsigned(n);
	if (chars_written == -1 || chars_written != digit_count)
		return (-1);
	*char_count += chars_written;
	return (chars_written);
}
