#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

typedef struct	json {
	enum {
		MAP,
		INTEGER,
		STRING
	} type;
	union {
		struct {
			struct pair	*data;
			size_t		size;
		} map;
		int	integer;
		char	*string;
	};
}	json;
int argo(json *dst, FILE *stream);

typedef struct	pair {
	char	*key;
	json	value;
}	pair;

int	peek(FILE *stream)
{
	int	c = getc(stream);
	ungetc(c, stream);
	return c;
}

void	unexpected(FILE *stream)
{
	if (peek(stream) != EOF)
		printf("unexpected token '%c'\n", peek(stream));
	else
		printf("unexpected end of input\n");
}

int	accept(FILE *stream, char c)
{
	if (peek(stream) == c)
	{
		(void)getc(stream);
		return 1;
	}
	return 0;
}

char *get_str(FILE *stream)
{
	char *res = calloc(4096, sizeof(char));
	int i = 0;
	char c = getc(stream);

	while (1)
	{
		c = getc(stream);

		if(c == '"')
			break;
		if(c == EOF)
		{
			unexpected(stream);
			return (NULL);
		}
		if (c == '\\')
			c = getc(stream);
		res[i++] = c;
	}
	return(res);
}

int parser_int(json *dst ,FILE *stream)
{
	int n = 0;
	int c = peek(stream);
	fscanf(stream, "%d", &n);
	dst->type = INTEGER;
	dst->integer = n;
	return (1);
}

int parser_str(json *dst, FILE *stream)
{
	dst->type = STRING;
	dst->string = get_str(stream);
	if(dst->string == NULL)
	{
		return (-1);
	}
	return(1);
}



int parser_map(json *dst, FILE *stream)
{
	dst->type = MAP;
	dst->map.data = NULL;
	dst->map.size = 0;
	char c = getc(stream);
	if(peek(stream) == '}')
		return(1);
	
	while (1)
	{
		c = peek(stream);
		if (c != '"')
		{
			unexpected(stream);
			return(-1);
		}
		dst->map.data = realloc(dst->map.data, (dst->map.size + 1) * sizeof(pair));
		pair *current = &dst->map.data[dst->map.size];
		current->key = get_str(stream);
		if(current->key == NULL)
			return(-1);
		dst->map.size++;

		c = peek(stream);
        if (!accept(stream, ':'))
        {
            unexpected(stream);
            return -1;
        }
		if (argo(&current->value, stream) == -1)
			return (-1);
		c = peek(stream);
		if (c == '}')
		{
			accept(stream, c);
			break ;
		}

		if (c == ',')
			accept(stream, c);
		else
		{
			unexpected(stream);
			return(-1);
		}
		
	}
	return (1);
}

int parser(json *dst, FILE *stream)
{
	int c  = peek(stream);

	if(c == EOF)
	{
		unexpected(stream);
		return(-1);
	}
	if(isdigit(c))
		return (parser_int(dst, stream));
	else if (c == '"')
		return(parser_str(dst, stream));
	else if (c == '{')
		return(parser_map(dst, stream));
	else 
	{
		unexpected (stream);
		return(-1);
	}
}

int argo(json* dst, FILE *stream)
{
	if(parser(dst, stream) == -1)
		return -1;
	return (1);
}

int main(int argc, char **argv)
{
	if(argc != 2)
		return (1);
	char *filename = argv[1];
	FILE *stream = fopen(filename, "r");
	if(!stream)
		return(1);
	json file;
	if(argo(&file, stream) != 1)
	{
		return(1);
	}
}