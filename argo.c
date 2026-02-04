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

int parser_map(json *dst, FILE *stream)
{
    
}

int parser(json *dst, FILE *stream)
{
    int c  = peek(stream);

    if(c == EOF)
    {
        unexcpected(stream);
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
        unexcpected (stream);
        return(-1);
    }
}








int argo(json* dst, FILE *stream)
{
    if(parser(dst, stream == -1))
        return -1;
    return (1);
}




int main(int argc, char ***argv)
{
    if(argc != 2)
        return (1);
    char *filename = argv[1];
    FILE *stream fopen(filename, "r");
    if(!stream)
    return(1);
    json file;
    if(argo(&file, stream != 1))
    {

    }
}