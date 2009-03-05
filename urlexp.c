#include <stdio.h>
#include <string.h>

#define MAXRANG 1000

void strips(char *str)
{
	char *ptr;
	int len, i;

	len = strlen(str);
	for (i = 0, ptr = str; i < len; i++)
	{
		if (str[i] != ' ' && str[i] != '\t')
		{
			*(ptr++) = str[i];
		}
	}
	*ptr = '\0';
}

int urlexp(FILE *in)
{
	char buf[256];
	char buf2[256];
	char *http;
	char *pat;
	char *num1;
	char *num2;
	char *ptr;
	int begin, end, width, i;
	int lineno;

	lineno = 0;
	for (;;)
	{
		ptr = fgets(buf, sizeof(buf), in);
		if (!ptr)
			break;
		lineno++;
		if (buf[0] == '\n' || buf[0] == '#')
		{
			continue;
		}
		http = strtok(buf, " \t\n");
		//if (!http)
		//{
		//	continue;
		//}
		ptr = strtok(NULL, "\n");
		if (!ptr)
		{
			strips(http);
			printf("%s\n", buf);
			continue;
		}
		strips(ptr);
		pat = strtok(ptr, "=");
		num1 = strtok(NULL, "~");
		num2 = strtok(NULL, "");
		if (!(pat && num1 && num2))
		{
			fprintf(stderr, "line %d: Sytax error.\n", lineno);
			return 0;
		}
		width = strlen(num1);
		begin = atoi(num1);
		end = atoi(num2);
		if (end <= begin)
		{
			fprintf(stderr, "line %d: Can't determine rang.\n", lineno);
			return 0;
		}
		else if (begin - end > MAXRANG)
		{
			fprintf(stderr, "line %d: Out of rang.\n", lineno);
			return 0;
		}
		ptr = strstr(http, pat);
		if (!ptr)
		{
			fprintf(stderr, "line %d: Pattern doesn't apear in URL.\n", lineno);
			return 0;
		}
		*(ptr) = '\0';
		ptr += strlen(pat);
		sprintf(buf2, "%s%%0%dd%s\n", http, width, ptr);
		for (i = begin; i <= end; i++)
		{
			printf(buf2, i);
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	FILE *expin;

	expin = (argc > 1) ? fopen(argv[1], "r") : stdin;
	if (!expin)
		perror(argv[1]);

	return urlexp(expin);
}
