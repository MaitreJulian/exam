#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 10


int ft_strlen(char *str)
{
    int count;


    count = 0;
    while (str && str[0])
    {
        str++;
        count++;
    }
    return (count);
}

int ft_strchr(char *str, char c)
{
    
    int i = 0;

    while (str[i] && str[i] != c)
        i++;
    if (str[i] == c)
        return(1);
    return (0);
}

char *ft_strjoin(char *s1, char *s2)
{
    int i = 0;
    int j = 0;
    char *new_str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));

    if (!new_str)
        return(NULL);
    if (!s1 && !s2)
        return(NULL);
    if (!s1)
        return(s2);
    if (!s2)
        return(s1);
    while (s1[i])
    {
        new_str[i] = s1[i];
        i++;
    }
    while (s2[j])
        new_str[i++] = s2[j++];
    new_str[i] = '\0';
    free(s1);
    return(new_str);
}

char *ft_left(char *buffer)
{
    {
    char *new_buffer;
    int i = 0;
    int j = 0;
    int k = 0;

    while (buffer[i])
        i++;
    while(buffer[j] && buffer[j] != '\n')
        j++;
    new_buffer = malloc(sizeof(char) * (i - j + 1));
    if (!new_buffer)
    {
        free(buffer);
        return(NULL);
    }
    j++;
    while (j < i)
    {
        new_buffer[k] = buffer[j];
        j++;
        k++;
    }
    if (buffer[j] && buffer[j] == '\n')
        new_buffer[k++] = '\n';
    new_buffer[k] = '\0';
    free(buffer);
    return(new_buffer);
}
}
char *ft_line(char *buffer)
{
    char *line;
    int i = 0;
    int j = 0;

    while (buffer[i] && buffer[i] != '\n')
        i++;
    line = malloc(sizeof(char) * (i + 2));
    if (!line)
    {
        free(buffer);
        return(NULL);
    }
    while (j < i)
    {
        line[j] = buffer[j];
        j++;
    }
    if (buffer[j] && buffer[j] == '\n')
        line[j++] = '\n';
    line[j] = '\0';
    return(line);
}
char *ft_readline(int fd, char *buffer)
{
    int byte_read = 1;
    char *temp;

    if(!buffer)
    {
            buffer =malloc(sizeof(char));
            buffer[0] = '\0';
    }
    if(!buffer)
        return(NULL);
    while (byte_read > 0)
    {
        temp = malloc(sizeof(char) * (BUFFER_SIZE + 1));
        if (!temp)
        {
            free(buffer);
            return(NULL);
        }
        byte_read = read(fd, temp, BUFFER_SIZE);
        if (byte_read == -1)
        {
            free(temp);
            free(buffer);
            return(NULL);
        }
        temp[byte_read] = '\0';
        buffer = ft_strjoin(buffer, temp);
        free(temp);
        if (ft_strchr(buffer, '\n'))
            break;
        if(byte_read == 0)
        {
            free(buffer);
            return(NULL);
        }
    }
    return(buffer);
}

char *get_next_line(int fd)
{
    static char *buffer;
    char *line;

    if(fd < 0 || BUFFER_SIZE <= 0 )
        return(NULL);
    buffer = ft_readline(fd, buffer);
    if (buffer == NULL)
        return(NULL);
    line = ft_line(buffer);
    buffer = ft_left(buffer);
    return(line);
}

int main(void)
{
    int fd = open("text.txt", O_RDONLY);
    char *line = get_next_line(fd);

    while( line)
    {
        printf("%s", line);
        free(line);
        line = get_next_line(fd);
    }
    return(0);
}