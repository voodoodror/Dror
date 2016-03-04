#include <stdio.h>
#include <stdlib.h>
#include "assembler.h"

int main(int argc, char **argv)
{
    int i;
    FILE *fp;
    char file_name[20];
    char *lineToProcess = malloc(sizeof(char) * BUF_SIZE);

    for (i = 1; i < argc; i++)
    {
        int count = 0, buf_count = BUF_SIZE;

        sprintf(file_name, "%s", argv[i]);
        fp = fopen(file_name, "r");

        if (fp == NULL)
        {
            printf("Error: can't find file %s\n", file_name);
            return -1;
        }

        while (fgets(lineToProcess, MAX_LINE, fp))
        {
        	first_parsing_line(lineToProcess, &count);
        	count++;
        }
        fclose(fp);
    }
    return 0;
}

void first_parsing_line (char *line, int *count) {
	int i=0, ic=0, dc=0;
	/* remove pre spaces */


	strip_extra_spaces(line);
	/*ignore_comments(line);*/
	if (hasSymbol(line) != NULL) {
		if (hasSymbol(hasSymbol(line)+1) != NULL) {
			printf("%s",line);
		} else {
			printf("%s has 1 symbol exactly.\n",line);
		}
	}

	/* removes comment from beginning*/
	if (line[0] == ';') {
		printf("%d: Comment found, ignoring...\n",*count);
	} else {
		printf("%d: %s\n",*count,line);
	}
}

void strip_extra_spaces(char* str) {
  int i,x;
  for(i=x=0; str[i]; ++i)
    if(!isspace(str[i]) || (i>0 && !isspace(str[i-1])))
      str[x++] = str[i];
  str[x] = '\0';
}

char *hasSymbol(char* str) {
	char symbolChar = ':';
	char *symbolPos = strchr(str, symbolChar);

	return symbolPos;
}
