/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmorandi <nmorandi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 13:23:20 by nmorandi          #+#    #+#             */
/*   Updated: 2022/12/15 14:42:58 by nmorandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	len_to_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i + 1);
		i++;
	}
	return (0);
}

static char	*ft_init(char *init, size_t len)
{
	size_t	i;

	i = 0;
	if (init)
		free(init);
	init = malloc(sizeof(char) * len);
	if (!init)
		return (NULL);
	while (i < len)
	{
		init[i] = '\0';
		i++;
	}
	return (init);
}

static char	*read_and_store(int fd, char *extra)
{
	char	*buffer;
	ssize_t	ret;

	buffer = NULL;
	buffer = ft_init(buffer, BUFFER_SIZE + 1);
	ret = 1;
	if (extra == NULL)
		extra = ft_init(extra, 1);
	if (!extra || !buffer)
		return (NULL);
	while (!ft_strchr(extra, '\n') && ret != 0)
	{
		ret = (int)read(fd, buffer, BUFFER_SIZE);
		if (ret == -1 || (extra == NULL && ret == 0))
		{
			if (extra)
				free(extra);
			free(buffer);
			return (NULL);
		}
		extra = ft_store_extra(extra, buffer, ret);
	}
	free(buffer);
	return (extra);
}

static char	*ft_clear_extra(char *extra)
{
	char	*ptr;

	ptr = extra;
	extra = ft_substr(extra, len_to_char(extra, '\n'), ft_strlen(extra));
	free(ptr);
	return (extra);
}

char	*get_next_line(int fd)
{
	static char	*extra;
	char		*line;

	if (BUFFER_SIZE < 1 || fd < 0)
		return (NULL);
	line = NULL;
	extra = read_and_store(fd, extra);
	if (extra == NULL)
		return (NULL);
	if (!ft_strchr(extra, '\n'))
	{
		if (extra[0] == '\0')
		{
			free(extra);
			extra = NULL;
			return (NULL);
		}
		line = ft_substr(extra, 0, ft_strlen(extra));
		extra = ft_init(extra, 1);
		return (line);
	}
	line = ft_substr(extra, 0, len_to_char(extra, '\n'));
	extra = ft_clear_extra(extra);
	return (line);
}
