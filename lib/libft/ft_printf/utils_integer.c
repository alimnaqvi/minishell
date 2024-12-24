/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_integer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 18:20:38 by anaqvi            #+#    #+#             */
/*   Updated: 2024/10/24 12:06:10 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putnbr_err(int n)
{
	char	to_write;
	int		chars_written;
	long	num;

	chars_written = 0;
	num = n;
	if (num < 0)
	{
		if (write(1, "-", 1) == -1)
			return (-1);
		chars_written++;
		num = -num;
	}
	if (num >= 10)
	{
		chars_written += ft_putnbr_err(num / 10);
		if (chars_written == -1)
			return (-1);
	}
	to_write = num % 10 + '0';
	if (write(1, &to_write, 1) == -1)
		return (-1);
	return (chars_written + 1);
}

int	count_digits_signed(int n)
{
	int		count;
	long	num;

	num = n;
	count = 0;
	if (num == 0)
		return (1);
	if (num < 0)
	{
		num = -num;
		count++;
	}
	while (num != 0)
	{
		num /= 10;
		count++;
	}
	return (count);
}

int	ft_putnbr_err_unsigned(unsigned int n)
{
	char			to_write;
	int				chars_written;
	unsigned int	num;

	chars_written = 0;
	num = n;
	if (num >= 10)
	{
		chars_written += ft_putnbr_err_unsigned(num / 10);
		if (chars_written == -1)
			return (-1);
	}
	to_write = num % 10 + '0';
	if (write(1, &to_write, 1) == -1)
		return (-1);
	return (chars_written + 1);
}

int	count_digits_unsigned(unsigned int n)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}
