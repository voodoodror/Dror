#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assembler.h"

char *lineToProcess;
const char symbolChar = ':';
const char dotChar = '.';
const char spaceChar = ' ';
const char qmChar = '\"';
const char newlineChar = '\n';
const char commaChar = 44; /* , char */
int symbolLen = 0;
int dotLen = 0;
int symbolCounter = 0;
int errorFlag = 0;
int dc=0, ic=0;
int count;
mySymbolList *symbolList;
myHashTable *hashTable[MAX_ARRAY_SIZE];
myCommandTable commandTable[COMMAND_SIZE];
myDataTable *dataTable;

int main(int argc, char **argv)
{
    int i;

    FILE *fp;
    char file_name[20];
    char *lineToProcess = malloc(sizeof(char) * BUF_SIZE);

    init_command_table();

    for (i = 1; i < argc; i++)
    {
        sprintf(file_name, "%s", argv[i]);
        fp = fopen(file_name, "r");

        if (fp == NULL)
        {
            printf("Error: can't find file %s\n", file_name);
            return -1;
        }

        count=0;

        while (fgets(lineToProcess, MAX_LINE, fp))
        {
        	first_parsing_line(lineToProcess, &count);
        	count++;
        }
        fclose(fp);

        printf("\n\nSymbol Table:\n");
		mySymbolList* iter;
		for (iter = symbolList; NULL != iter; iter = iter->next)
			printf("NAME: \"%s\"\tADDR: \"%d\"\tEXTERNAL: \"%d\"\tACTION: \"%d\"\n",iter->Sym,iter->addr,iter->ext,iter->action);

		printf("\n\nData Table:\n");
		myDataTable* iterd;
		for (iterd = dataTable; NULL != iterd; iterd = iterd->next)
			printf("DC: \"%d\"\tDATA: \"%u\"\n",iterd->dc,iterd->data);
    }
    return 0;
}
void init_hash_table(myHashTable *il)
{
	il->addr = 0;
    il->era = 0;
    il->dest_reg = 0;
    il->dest_addr = 0;
    il->src_reg = 0;
    il->src_addr = 0;
    il->opcode = 0;
    il->group = 0;
    il->rnd = 0;
    il->not_in_use = 0;
}
void init_command_table()
{
	commandTable[0].command = "mov",commandTable[0].srcOperations=1,commandTable[0].destOperations=1,commandTable[0].srcAddrType0=1,commandTable[0].srcAddrType1=1,commandTable[0].srcAddrType2=1,commandTable[0].srcAddrType3=1,commandTable[0].srcAddrType2n3=0,commandTable[0].destAddrType0=0,commandTable[0].destAddrType1=1,commandTable[0].destAddrType2=1,commandTable[0].destAddrType3=1,commandTable[0].destAddrType2n3=1;
	commandTable[1].command = "cmp",commandTable[1].srcOperations=1,commandTable[1].destOperations=1,commandTable[1].srcAddrType0=1,commandTable[1].srcAddrType1=1,commandTable[1].srcAddrType2=1,commandTable[1].srcAddrType3=1,commandTable[1].srcAddrType2n3=0,commandTable[1].destAddrType0=1,commandTable[1].destAddrType1=1,commandTable[1].destAddrType2=1,commandTable[1].destAddrType3=1,commandTable[1].destAddrType2n3=0;
	commandTable[2].command = "add",commandTable[2].srcOperations=1,commandTable[2].destOperations=1,commandTable[2].srcAddrType0=1,commandTable[2].srcAddrType1=1,commandTable[2].srcAddrType2=1,commandTable[2].srcAddrType3=1,commandTable[2].srcAddrType2n3=0,commandTable[2].destAddrType0=0,commandTable[2].destAddrType1=1,commandTable[2].destAddrType2=1,commandTable[2].destAddrType3=1,commandTable[2].destAddrType2n3=1;
	commandTable[3].command = "sub",commandTable[3].srcOperations=1,commandTable[3].destOperations=1,commandTable[3].srcAddrType0=1,commandTable[3].srcAddrType1=1,commandTable[3].srcAddrType2=1,commandTable[3].srcAddrType3=1,commandTable[3].srcAddrType2n3=0,commandTable[3].destAddrType0=0,commandTable[3].destAddrType1=1,commandTable[3].destAddrType2=1,commandTable[3].destAddrType3=1,commandTable[3].destAddrType2n3=0;
	commandTable[4].command = "not",commandTable[4].srcOperations=0,commandTable[4].destOperations=1,commandTable[4].srcAddrType0=0,commandTable[4].srcAddrType1=0,commandTable[4].srcAddrType2=0,commandTable[4].srcAddrType3=0,commandTable[4].srcAddrType2n3=0,commandTable[4].destAddrType0=0,commandTable[4].destAddrType1=1,commandTable[4].destAddrType2=0,commandTable[4].destAddrType3=1,commandTable[4].destAddrType2n3=0;
	commandTable[5].command = "clr",commandTable[5].srcOperations=0,commandTable[5].destOperations=1,commandTable[5].srcAddrType0=0,commandTable[5].srcAddrType1=0,commandTable[5].srcAddrType2=0,commandTable[5].srcAddrType3=0,commandTable[5].srcAddrType2n3=0,commandTable[5].destAddrType0=0,commandTable[5].destAddrType1=1,commandTable[5].destAddrType2=0,commandTable[5].destAddrType3=1,commandTable[5].destAddrType2n3=0;
	commandTable[6].command = "lea",commandTable[6].srcOperations=1,commandTable[6].destOperations=1,commandTable[6].srcAddrType0=0,commandTable[6].srcAddrType1=1,commandTable[6].srcAddrType2=0,commandTable[6].srcAddrType3=0,commandTable[6].srcAddrType2n3=1,commandTable[6].destAddrType0=0,commandTable[6].destAddrType1=1,commandTable[6].destAddrType2=0,commandTable[6].destAddrType3=1,commandTable[6].destAddrType2n3=0;
	commandTable[7].command = "inc",commandTable[7].srcOperations=0,commandTable[7].destOperations=1,commandTable[7].srcAddrType0=0,commandTable[7].srcAddrType1=0,commandTable[7].srcAddrType2=0,commandTable[7].srcAddrType3=0,commandTable[7].srcAddrType2n3=0,commandTable[7].destAddrType0=0,commandTable[7].destAddrType1=1,commandTable[7].destAddrType2=0,commandTable[7].destAddrType3=1,commandTable[7].destAddrType2n3=0;
	commandTable[8].command = "dec",commandTable[8].srcOperations=0,commandTable[8].destOperations=1,commandTable[8].srcAddrType0=0,commandTable[8].srcAddrType1=0,commandTable[8].srcAddrType2=0,commandTable[8].srcAddrType3=0,commandTable[8].srcAddrType2n3=0,commandTable[8].destAddrType0=0,commandTable[8].destAddrType1=1,commandTable[8].destAddrType2=0,commandTable[8].destAddrType3=1,commandTable[8].destAddrType2n3=0;
	commandTable[9].command = "jmp",commandTable[9].srcOperations=0,commandTable[9].destOperations=1,commandTable[9].srcAddrType0=0,commandTable[9].srcAddrType1=0,commandTable[9].srcAddrType2=0,commandTable[9].srcAddrType3=0,commandTable[9].srcAddrType2n3=0,commandTable[9].destAddrType0=0,commandTable[9].destAddrType1=1,commandTable[9].destAddrType2=1,commandTable[9].destAddrType3=1,commandTable[9].destAddrType2n3=0;
	commandTable[10].command = "bne",commandTable[10].srcOperations=0,commandTable[10].destOperations=1,commandTable[10].srcAddrType0=0,commandTable[10].srcAddrType1=0,commandTable[10].srcAddrType2=0,commandTable[10].srcAddrType3=0,commandTable[10].srcAddrType2n3=0,commandTable[10].destAddrType0=0,commandTable[10].destAddrType1=1,commandTable[10].destAddrType2=1,commandTable[10].destAddrType3=1,commandTable[10].destAddrType2n3=0;
	commandTable[11].command = "red",commandTable[11].srcOperations=0,commandTable[11].destOperations=1,commandTable[11].srcAddrType0=0,commandTable[11].srcAddrType1=0,commandTable[11].srcAddrType2=0,commandTable[11].srcAddrType3=0,commandTable[11].srcAddrType2n3=0,commandTable[11].destAddrType0=0,commandTable[11].destAddrType1=1,commandTable[11].destAddrType2=1,commandTable[11].destAddrType3=1,commandTable[11].destAddrType2n3=0;
	commandTable[12].command = "prn",commandTable[12].srcOperations=0,commandTable[12].destOperations=1,commandTable[12].srcAddrType0=0,commandTable[12].srcAddrType1=0,commandTable[12].srcAddrType2=0,commandTable[12].srcAddrType3=0,commandTable[12].srcAddrType2n3=0,commandTable[12].destAddrType0=1,commandTable[12].destAddrType1=1,commandTable[12].destAddrType2=1,commandTable[12].destAddrType3=1,commandTable[12].destAddrType2n3=0;
	commandTable[13].command = "jsr",commandTable[13].srcOperations=0,commandTable[13].destOperations=1,commandTable[13].srcAddrType0=0,commandTable[13].srcAddrType1=0,commandTable[13].srcAddrType2=0,commandTable[13].srcAddrType3=0,commandTable[13].srcAddrType2n3=0,commandTable[13].destAddrType0=0,commandTable[13].destAddrType1=1,commandTable[13].destAddrType2=0,commandTable[13].destAddrType3=0,commandTable[13].destAddrType2n3=1;
	commandTable[14].command = "rts",commandTable[14].srcOperations=0,commandTable[14].destOperations=0,commandTable[14].srcAddrType0=0,commandTable[14].srcAddrType1=0,commandTable[14].srcAddrType2=0,commandTable[14].srcAddrType3=0,commandTable[14].srcAddrType2n3=0,commandTable[14].destAddrType0=0,commandTable[14].destAddrType1=0,commandTable[14].destAddrType2=0,commandTable[14].destAddrType3=0,commandTable[14].destAddrType2n3=0;
	commandTable[15].command = "stop",commandTable[15].srcOperations=0,commandTable[15].destOperations=0,commandTable[15].srcAddrType0=0,commandTable[15].srcAddrType1=0,commandTable[15].srcAddrType2=0,commandTable[15].srcAddrType3=0,commandTable[15].srcAddrType2n3=0,commandTable[15].destAddrType0=0,commandTable[15].destAddrType1=0,commandTable[15].destAddrType2=0,commandTable[15].destAddrType3=0,commandTable[15].destAddrType2n3=0;
}
void first_parsing_line (char *line, int *count) {
	int i=0, dupSymbol=0, symbolFound=0, commandFound=0, tmp=0;

	char *symbolPointer;
	char *dotCommand;
	char extractResult;

	strip_extra_spaces(line);
	if (line[0] == ';') {
		printf("%d: %s (comment line, ignoring)\n",*count,line);
	} else if (strlen(line) == 0){
		printf("%d: Empty line found, ignoring\n",*count);
	} else {
		if (!isalpha(line[0]) && line[0] != '.') {
			printf("%d: (line MUST begin with a letter OR a DOT!)\n",*count);
		} else {
			if (hasSymbol(line) != 0) {
				if (hasSymbol(line+(symbolLen+sizeof(symbolChar))) != 0) {
					printf("%d: %s (Syntax Error! multiple symbol signs found!)\n",*count,line);
				} else {
					if (symbolCounter!=0) {
						symbolPointer = getSymbol(line,hasSymbol(line));
						dupSymbol = findDuplicateSym(symbolList,symbolPointer);
					}
					if (!dupSymbol) {
						symbolFound=1;
						if (hasDot(line+(symbolLen+sizeof(symbolChar)+sizeof(spaceChar))) != NULL) {
							symbolPointer = getSymbol(line,hasSymbol(line));
							dotCommand = getNextString(line+(symbolLen+sizeof(symbolChar)+sizeof(spaceChar)+sizeof(dotChar)));
							if (strcmp(dotCommand,"string") == 0) {
								tmp = dc;
								extractResult = extractSymData("string");
							} else if (strcmp(dotCommand,"data") == 0) {
								tmp = dc;
								extractResult = extractSymData("data");
							} else {
								printf("%d: %s (unknown instruction line, ignoring)\n",*count,line);
							}
						} else {
							printf("%d: %s (NEW symbol found)\n",*count,line);
							symbolPointer = getSymbol(line,hasSymbol(line));
							if (symbolCounter == 0)
								symbolList = createSymbolNode(symbolPointer,100,0,0);
							else
								symbolList = addSymbolNode(symbolList,symbolPointer,100,0,0);
							symbolCounter++;
							dotCommand = getNextString(line+(symbolLen+sizeof(symbolChar)+sizeof(spaceChar)));
							commandFound = findCommand(dotCommand);
							if (commandFound!=-1) {
								extractResult = extractSym(commandFound);
								if (extractResult)
									printf("%d: %s (\"%s\")\n",*count,line,dotCommand);
							} else {
								printf("%d: %s (command not found)\n",*count,line);
							}
						}
					}
				}
			} else if (line[0] == '.'){
				symbolFound=0;
				dotCommand = getNextString(line+sizeof(dotChar));
				if (strcmp(dotCommand,"entry") == 0) {
					printf("%d: %s (entry found)\n",*count,line);
				} else if (strcmp(dotCommand,"extern") == 0) {
					symbolPointer = getNextString(line+(sizeof(spaceChar)+strlen(dotCommand)+sizeof(spaceChar)));
					if (symbolCounter == 0)
						symbolList = createSymbolNode(symbolPointer,0,1,0);
					else
						symbolList = addSymbolNode(symbolList,symbolPointer,0,1,0);
					symbolCounter++;
					printf("%d: %s (extern found)\n",*count,line);
				} else {
					printf("%d: %s (unknown instruction line, ignoring)\n",*count,line);
				}
			} else {
				symbolFound=0;
				i=0;
				dotCommand = getNextString(line);
				commandFound = findCommand(dotCommand);
				if (commandFound!=-1) {
					extractResult = extractOperands(line+(sizeof(spaceChar)+strlen((char *)dotCommand)),commandFound);
					if (extractResult)
						printf("%d: %s command found: (\"%s\")\n",*count,line,dotCommand);
				} else {
					printf("%d: %s (command not found)\n",*count,line);
				}
			}
		}
	}
}
int findDuplicateSym(mySymbolList *symbolList,char *sym) {
	mySymbolList* iter;
	for (iter = symbolList; NULL != iter; iter = iter->next) {
		if(strcmp(iter->Sym,sym) == 0) {
			printf("Duplicate symbol found!\n");
			return 1;
		}
	}
	return 0;
}
int findCommand(char *command) {
	int i=0;
	while (i<COMMAND_SIZE) {
		if (strcmp(command,commandTable[i].command)==0) {
			return i;
		}
		i++;
	}
	return -1;
}
void strip_extra_spaces(char* str) {
  int i,x;

  for(i=x=0; str[i]; ++i) {
	  if (str[i+1] == '\t')
	  		  str[i+1] = ' ';
	  if (isspace(str[i]) && !isalpha(str[i-1]) && (str[i-1] != symbolChar) && (!isdigit(str[i+1]) || !isdigit(str[i-1]) || str[i+1] =='-' || str[i-1] == qmChar))
		  	  i++;
	  if(!isspace(str[i]) || (i>0 && !isspace(str[i-1])))
		  str[x++] = str[i];
  }
  if (isspace(str[x]))
	  str[x] = '\0';
  if (strchr(str,newlineChar) != NULL)
	  str[x] = '\0';
  else
	  str[x] = '\0';
  if (isspace(str[strlen((char *)str)-1]))
  	  str[strlen((char *)str)-1] = '\0';
}

int hasSymbol(char* str) {
	char *symbolPos = strchr(str, symbolChar);
	int tmpLen;

	if (symbolPos == NULL)
		return 0;
	tmpLen = strlen(str)-strlen(symbolPos);
	if (!tmpLen)
		tmpLen+=1;

	symbolLen=tmpLen;
	return tmpLen;
}
char *hasDot(char* str) {
	char *tmpStr;
	tmpStr = strchr(str,dotChar);

	return tmpStr;
}
char *hasQM(char* str) {
	char *tmpStr;
	tmpStr = strchr(str,qmChar);

	return tmpStr;
}
int extractData(char *str, char *type) {

	char rwString[BUF_SIZE];
	char *rwPointer;
	strcpy(rwString,str);
	char *token;

	rwPointer = rwString;
	if (str == NULL) {
		printf("No data found in string");
		return 0;
	}
	if (strcmp(type,"data") == 0) {

		token = strsep(&rwPointer,",");
		if (token == NULL) {
			printf("%d: Error occurred while trying to parse data field.\n1. Please make sure you don\'t have comma at the beginning or end of the string.\n2. Don't put 2 commas side by side.",count);
			return 0;
		}
		while (token != NULL) {
			if ((strlen(token)<=1) && (!isdigit(token[0]))) {
				printf("%d: Syntax Error: Empty or invalid char found.\n",count);
				return 0;
			}
			token = strsep(&rwPointer,",");
		}
		strcpy(rwString,str);
		rwPointer = rwString;
		token = strsep(&rwPointer,",");

		while (token != NULL) {
			if (dc == 0) {
				dataTable = createDataNode(dc++,(int)*token);
			} else {
				dataTable = addDataNode(dataTable,dc++,(int)*token);
			}
			/*printf("TOKEN: %s\n", token);*/

			token = strsep(&rwPointer,",");
		}
		return 1;

		} else if (strcmp(type,"string") == 0) {
			int qmFound=0;
			token = str;
			while (token != NULL) {
				token = hasQM(token+1);
				qmFound++;
			}
			if (qmFound==2) {
				str = str+1;

				str[strlen(str)] = '\0';

				while (str[0]!='\0') {
					if (dc == 0) {
						dataTable = createDataNode(dc++,(int)str[0]);
						str++;
					} else {
						dataTable = addDataNode(dataTable,dc++,(int)str[0]);
						str++;
					}
				}
				return 1;
			} else if (qmFound<=2) {
				printf("%d: Syntax Error: No quotation marks found for string!\n",count);
				return 0;
			} else if (qmFound>2) {
				printf("%d: Syntax Error: Too much quotation marks has been found!\n",count);
				return 0;
			}
		}
	return 0;
}
int extractOperands(char *str, int opcode) {
	char rwString[BUF_SIZE];
	char *rwPointer;
	strcpy(rwString,str);
	char *token;
	int i=0,singleOperand=0,noOperands=0;
	rwPointer = rwString;

	token = strchr(str,commaChar);
	if (opcode==14 || opcode==15) {
		if (token!=NULL) {
			printf("Syntax Error! You can't use operands in this command!\n");
			return 0;
		}
		noOperands=1;
	}
	if (token == NULL && !noOperands) {
		if (str != NULL && commandTable[opcode].srcOperations==0 && commandTable[opcode].destOperations==1) {
			singleOperand=1;
		} else {
			printf("%d: Invalid operand for %s\n",count,commandTable[opcode].command);
			return 0;
		}
	}

	token = strsep(&rwPointer,",");

	while (token != NULL && !singleOperand && !noOperands) {
		if (strcmp(token,"\0")==0) {
			printf("%d: Syntax Error: Empty or invalid operand found.\n",count);
			return 0;
		}
		token = strsep(&rwPointer,",");
		i++;
	}
	if (i==2 && !singleOperand && !noOperands) {
		strcpy(rwString,str);
		rwPointer = rwString;

		/*if (dataTable == NULL) {
			token = strsep(&rwPointer,",");
			dataTable = createDataNode(count,count);
			token = strsep(&rwPointer,",");
			dataTable = addDataNode(dataTable,count,count);
		} else {
			token = strsep(&rwPointer,",");
			dataTable = addDataNode(dataTable,count,count);
			token = strsep(&rwPointer,",");
			dataTable = addDataNode(dataTable,count,count);
		}*/
		return 1;
	} else if (singleOperand) {
		if (singleOperand)
		printf("%d: Command 1 SRC or DEST OPERAND found.\n",count);
		return 1;
	} else if (noOperands) {
		printf("%d: STOP or RTS command found!\n",count);
		return 1;
	} else {
		printf("SYNTAX ERROR:\n1. Do not place 2 commas side by side\n2. Please make sure you do not exceed 2 operands limit.\n\n");
		return 0;
	}
}
char *getNextString(char* str) {
	char *dotPos = strchr(str, spaceChar);
	char *tmp = malloc(sizeof(str));
	int tmpLen;

	if (dotPos == NULL)
		return str;
	tmpLen = strlen(str)-strlen(dotPos);
	if (!tmpLen)
		tmpLen+=1;

	dotLen=tmpLen;
	memcpy(tmp,str,tmpLen+1);
	tmp[dotLen] = '\0';
	return tmp;
}
char *getSymbol(char* str, int pos) {
	char *myStr = malloc(sizeof(char*));
	memcpy(myStr,str,pos);
	myStr[pos] = '\0';
	return myStr;
}
mySymbolList *createSymbolNode (char* str, unsigned int dc, int external, int action) {
	mySymbolList* newSymbol = malloc(sizeof(mySymbolList));
	char * myStr = malloc(sizeof(strlen(str)));
	memcpy(myStr,str,strlen(str));

	if (NULL != newSymbol){
		newSymbol->Sym = myStr;
		newSymbol->addr = dc;
		newSymbol->ext = external;
		newSymbol->action = action;
		newSymbol->next = NULL;
	}
	return newSymbol;
}
mySymbolList *addSymbolNode (mySymbolList* symbolList, char* str, unsigned int dc, int external, int action) {
	mySymbolList* newSymbol = createSymbolNode(str,dc,external,action);
	mySymbolList *firstpos = symbolList;
	if (newSymbol != NULL) {
			while (symbolList->next != NULL) {
				symbolList = symbolList->next;
			}
			symbolList->next = newSymbol;
	}
	return firstpos;
}
myDataTable *createDataNode (int dc, unsigned int data) {
	myDataTable* newData = malloc(sizeof(myDataTable));

	if (NULL != newData){
		newData->dc = dc;
		newData->data = data;
		newData->next = NULL;
	}
	return newData;
}
myDataTable *addDataNode (myDataTable* dataTable, int dc, unsigned int data) {
	myDataTable* newData = createDataNode(dc,data);
	myDataTable *firstpos = dataTable;
	if (newData != NULL) {
			while (dataTable->next != NULL) {
				dataTable = dataTable->next;
			}
			dataTable->next = newData;
	}
	return firstpos;
}
