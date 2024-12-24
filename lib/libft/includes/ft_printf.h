/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 15:53:59 by anaqvi            #+#    #+#             */
/*   Updated: 2024/11/21 12:29:10 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdint.h>
# include <stdlib.h>
# include <unistd.h>

int	ft_printf(const char *format, ...);

int	handle_char(va_list *arg, int *char_count);
int	handle_str(va_list *arg, int *char_count);
int	handle_ptr(va_list *arg, int *char_count);
int	ft_putaddress(void *ptr);
int	convert_to_hex(uintptr_t p, char **buffer, int addr_len);
int	get_address_length(uintptr_t p);
int	handle_num(va_list *arg, int *char_count);
int	ft_putnbr_err(int n);
int	count_digits_signed(int n);
int	handle_unsigned(va_list *arg, int *char_count);
int	ft_putnbr_err_unsigned(unsigned int n);
int	count_digits_unsigned(unsigned int n);
int	handle_hex_upper(va_list *arg, int *char_count);
int	ft_puthex_upper(unsigned int X);
int	handle_hex_lower(va_list *arg, int *char_count);
int	ft_puthex_lower(unsigned int x);
int	count_digits_hex(unsigned int X);
int	handle_percent(int *char_count);
int	handle_other(const char *format, int *char_count);

#endif