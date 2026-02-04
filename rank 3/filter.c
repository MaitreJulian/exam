#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define BUFFER_SIZE 10

int ft_strlen(char *str)
{
	int i = 0;

	if(!str || !str[0])
		return(0);
	while (str[i])
		i++;
	return (i);
}

int ft_strchr(char *str, int c)
{
	int i = 0;

	if (!str)
		return (0);
	while (str[i] && str[i] != c)
		i++;
	return (str[i] == c);
}

char *ft_strjoin(char *s1, char *s2)
{
	int i = 0;
	int j = 0;
	int len = ft_strlen(s1) + ft_strlen (s2); 
	char *new_str;

	new_str = malloc(sizeof(char) * (len + 1));
	while (s1 && s1[i])
	{
		new_str[i] = s1[i];
		i++;
	}
	while (s2 && s2[j])
	{
		new_str[i++] = s2[j++];
	}
	free(s1);
	new_str[len] = '\0';
	return (new_str);
}


char *ft_filter(char *str, char *filter)
{
	int i;
	int j;
	int len_str;
	int len_filter;
	char *new_str;
	char *temp;

	len_filter = ft_strlen(filter);
	len_str = ft_strlen(str);
	i = 0;
	new_str = malloc(sizeof(char) * (len_str + 1));
	new_str[len_str] = '\0';
	while (*str)
	{
		j = 0;
		len_str = ft_strlen(str);
		temp = memmem(str, len_str, filter, len_filter);
		while (str != temp && str[0])
		{
			new_str[i] = *str;
			str++;
			i++;
		}
		if (str == temp && str[0])
		{
			while (j < len_filter)
			{
				new_str[i] = '*';
				i++;
				j++;
				str++;
			}
		}
	}
	return(new_str);
}

int main(int argc, char **argv)
{
    char *buffer;
    int byte_read = 1;
    char *temp;
    char *new_str;

    if (argc != 2)
        return (1);
    while (byte_read > 0)
    {
        temp = malloc(1);
        if (!temp)
            return (1);
        temp[0] = '\0';
		byte_read = 1;
        while (byte_read > 0)
        {
            buffer = malloc(BUFFER_SIZE + 1);
            if (!buffer)
                return (1);
            byte_read = read(0, buffer, BUFFER_SIZE);
            if (byte_read == -1)
            {
                free(buffer);
                free(temp);
                return (1);
            }
            buffer[byte_read] = '\0';
            temp = ft_strjoin(temp, buffer);
            if (ft_strchr(buffer, '\n'))
            {
                free(buffer);
                break;
            }
            free(buffer);
        }
        new_str = ft_filter(temp, argv[1]);
        free(temp);
        printf("%s", new_str);
        free(new_str);
    }
    return (0);
}


