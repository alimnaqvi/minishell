/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 14:41:42 by anaqvi            #+#    #+#             */
/*   Updated: 2024/10/18 19:08:30 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*append_buffer(char **output_line, char *buffer, size_t len)
{
	char	*new_line;
	char	*temp;

	temp = ft_substr(buffer, 0, len);
	if (!temp)
		return (free(*output_line), NULL);
	new_line = ft_strjoin(*output_line, temp);
	if (!new_line)
		return (free(*output_line), free(temp), NULL);
	free(temp);
	temp = NULL;
	free(*output_line);
	*output_line = new_line;
	return (new_line);
}

static char	*process_leftover(char **leftover_buffer, char **output_line)
{
	char	*newline_pos;

	newline_pos = ft_strchr(*leftover_buffer, '\n');
	if (newline_pos)
	{
		*output_line = ft_substr(*leftover_buffer, 0, newline_pos
				- *leftover_buffer + 1);
		if (!*output_line)
			return (NULL);
		newline_pos = ft_strdup(newline_pos + 1);
		if (!newline_pos)
			return (free(*output_line), NULL);
		free(*leftover_buffer);
		*leftover_buffer = newline_pos;
		return (*output_line);
	}
	*output_line = ft_strdup(*leftover_buffer);
	free(*leftover_buffer);
	*leftover_buffer = NULL;
	return (NULL);
}

static char	*handle_newline(char **output_line, char **buffer,
		char **leftover_buffer, char **newline_pos)
{
	if (!append_buffer(output_line, *buffer, *newline_pos - *buffer + 1))
		return (free(*buffer), NULL);
	*leftover_buffer = ft_strdup(*newline_pos + 1);
	if (!*leftover_buffer)
		return (free(*buffer), free(*output_line), NULL);
	return (free(*buffer), *output_line);
}

static char	*read_line(int fd, char **leftover_buffer, char **output_line)
{
	char	*buffer;
	ssize_t	bytes_read;
	char	*newline_pos;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (free(*output_line), NULL);
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		newline_pos = ft_strchr(buffer, '\n');
		if (newline_pos)
			return (handle_newline(output_line, &buffer, leftover_buffer,
					&newline_pos));
		if (!append_buffer(output_line, buffer, bytes_read))
			return (free(buffer), NULL);
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	if (bytes_read < 0)
		return (free(buffer), free(*output_line), NULL);
	else if (bytes_read == 0 && (!*output_line || **output_line == '\0'))
		return (free(buffer), free(*output_line), NULL);
	return (free(buffer), *output_line);
}

char	*get_next_line(int fd)
{
	static char	*leftover_buffer;
	char		*output_line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (leftover_buffer)
	{
		if (process_leftover(&leftover_buffer, &output_line))
			return (output_line);
		if (!output_line)
			return (NULL);
	}
	else
	{
		output_line = ft_strdup("");
		if (!output_line)
			return (NULL);
	}
	return (read_line(fd, &leftover_buffer, &output_line));
}
