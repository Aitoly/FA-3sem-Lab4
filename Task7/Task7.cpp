#pragma warning(disable:4996)

#include<stdio.h>
#include<iostream>
#include<ctype.h>
#include<math.h>
#include<time.h>

//#define DEBUG 1

#include"MyString7.h"
#include"Tree7.h"

#define COMMANS_COUNT 10

#define CALC_COUNT 7

/////////////////////////////////////////////////
/////////////        Commands
/////////////////////////////////////////////////
void SetCommand(char** commands, int i, char* new_command)
{
	free(*(commands + i));
	*(commands + i) = (char*)malloc(sizeof(char) * strlen(new_command) + 1);
	strcpy(*(commands + i), new_command);
}

void SetCommandDef(char** commands, int i, const char* new_command)
{
	*(commands + i) = (char*)malloc(sizeof(char) * strlen(new_command) + 1);
	strcpy(*(commands + i), new_command);
}

char** SetCommandsDef()
{
	char** commands = (char**)malloc(sizeof(char*) * COMMANS_COUNT);

	SetCommandDef(commands, 0, "ADD");
	SetCommandDef(commands, 1, "MULT");
	SetCommandDef(commands, 2, "SUB");
	SetCommandDef(commands, 3, "POW");
	SetCommandDef(commands, 4, "DIV");
	SetCommandDef(commands, 5, "REM");
	SetCommandDef(commands, 6, "XOR");
	SetCommandDef(commands, 7, "INPUT");
	SetCommandDef(commands, 8, "OUTPUT");
	SetCommandDef(commands, 9, "=");

	return commands;
}

void SetPropety(char* name, char* commands[COMMANS_COUNT], char* save, char* op)
{
	FILE* f;
	f = fopen(name, "r");
	if (!f)
		return;

	char buf[BUFSIZ], * ptr;
	int comment = 0;

	while (!feof(f))
	{
		*buf = 0;
		fgets(buf, BUFSIZ, f);

		if (strlen(buf) > 3)
		{
			*(buf + strlen(buf) - 1) = 0;

			for (int i = 0; i < strlen(buf); i++)
			{
				if (*(buf + i) == '#')
				{
					*(buf + i) = 0;
					break;
				}
				*(buf + i) = toupper(*(buf + i));
			}

#ifdef DEBUG
			printf("FIND propety: %s\n", buf);
#endif

			if ((ptr = buf) && *ptr++ == 'A' && *ptr++ == 'D' && *ptr++ == 'D' && *ptr++ == ' ')
			{
				SetCommandDef(commands, 0, ptr);
			}
			else if ((ptr = buf) && *ptr++ == 'M' && *ptr++ == 'U' && *ptr++ == 'L' && *ptr++ == 'T' && *ptr++ == ' ')
			{
				SetCommandDef(commands, 1, ptr);
			}
			else if ((ptr = buf) && *ptr++ == 'S' && *ptr++ == 'U' && *ptr++ == 'B' && *ptr++ == ' ')
			{
				SetCommandDef(commands, 2, ptr);
			}
			else if ((ptr = buf) && *ptr++ == 'P' && *ptr++ == 'O' && *ptr++ == 'W' && *ptr++ == ' ')
			{
				SetCommandDef(commands, 3, ptr);
			}
			else if ((ptr = buf) && *ptr++ == 'D' && *ptr++ == 'I' && *ptr++ == 'V' && *ptr++ == ' ')
			{
				SetCommandDef(commands, 4, ptr);
			}
			else if ((ptr = buf) && *ptr++ == 'R' && *ptr++ == 'E' && *ptr++ == 'M' && *ptr++ == ' ')
			{
				SetCommandDef(commands, 5, ptr);
			}
			else if ((ptr = buf) && *ptr++ == 'X' && *ptr++ == 'O' && *ptr++ == 'R' && *ptr++ == ' ')
			{
				SetCommandDef(commands, 6, ptr);
			}
			else if ((ptr = buf) && *ptr++ == 'I' && *ptr++ == 'N' && *ptr++ == 'P' && *ptr++ == 'U' && *ptr++ == 'T' && *ptr++ == ' ')
			{
				SetCommandDef(commands, 7, ptr);
			}
			else if ((ptr = buf) && *ptr++ == 'O' && *ptr++ == 'U' && *ptr++ == 'T' && *ptr++ == 'P' && *ptr++ == 'U' && *ptr++ == 'T' && *ptr++ == ' ')
			{
				SetCommandDef(commands, 8, ptr);
			}
			else if ((ptr = buf) && *ptr++ == '=' && *ptr++ == ' ')
			{
				SetCommandDef(commands, 9, ptr);
			}
			else if ((ptr = buf) && !strcmp(buf, "LEFT="))
			{
				*save = 'L';
			}
			else if ((ptr = buf) && !strcmp(buf, "RIGHT="))
			{
				*save = 'R';
			}
			else if ((ptr = buf) && !strcmp(buf, "OP()"))
			{
				strcpy(op, "OP()");
			}
			else if ((ptr = buf) && !strcmp(buf, "()OP"))
			{
				strcpy(op, "()OP");
			}
			else
			{
				printf("Unknown propety: %s\n", buf);
			}
		}
	}
}

void PrintCommands(char** commands, char* save, char* op)
{
	printf("add = %s\n", commands[0]);
	printf("mult = %s\n", commands[1]);
	printf("sub = %s\n", commands[2]);
	printf("pow = %s\n", commands[3]);
	printf("div = %s\n", commands[4]);
	printf("rem = %s\n", commands[5]);
	printf("xor = %s\n", commands[6]);
	printf("input = %s\n", commands[7]);
	printf("output = %s\n", commands[8]);
	printf("= = %s\n", commands[9]);
	printf("save to %c\n", *save);
	printf("op mode %s\n", op);
}

void DeleteCommands(char** commands)
{
	for (int i = 0; i < COMMANS_COUNT; i++)
	{
		free(*(commands + i));
	}
	free(commands);
}

/////////////////////////////////////////////////

int Get_Info(FILE* in, char* line)
{
	if (!in)
	{
		return 0;
	}



	char c;
	int mode = 0;

	//0 - нет
	//-1 - однострочный коментарий
	//>0 - просто коментарий

	while (!feof(in))
	{
		c = fgetc(in);
		c = toupper(c);

		if (mode == 0)
		{
			if (isspace(c) || c == '\n' || c == EOF)
			{
				continue;
			}

			if (c == ';')
			{
				*line++ = c;
				*line = '\0';
				return 1;
			}
			else if (c == '#')
			{
				mode = -1;
			}
			else if (c == '[')
			{
				mode++;
			}
			else if (c == ']')
			{
				mode--;
				if (mode < 0)
				{
					*line = '\0';
					return 0;
				}
			}
			else
			{
				*line++ = c;
			}
		}
		else if (mode == -1)
		{
			if (c == '\n')
			{
				mode = 0;
			}
		}
		else if (mode > 0)
		{
			if (c == ']')
			{
				mode--;
				if (mode < 0)
				{
					*line = '\0';
					return 0;
				}
			}
			else if (c == '[')
			{
				mode++;
			}
		}
	}

	if (!mode)
	{
		return 1;
	}
	else
	{
		*line = '\0';
		return 0;
	}
}

int StartCompare(char* str, char* command)
{
	for (int i = 0; i < strlen(command); i++)
	{
		if (i > strlen(str))
			return 0;

		if (*(str + i) != *(command + i))
			return 0;
	}

	return 1;
}

int IsCommand(char* str, char** commands)
{
	for (int i = 0; i < CALC_COUNT; i++)
	{
		if (StartCompare(str, *(commands + i)))
		{
			return i;
		}
	}

	return -1;
}

char* FindClose(char* line)
{
	for (int i = 0; i < strlen(line); i++)
	{
		if (*(line + i) == ')')
		{
			return line + i + 1;
		}
	}
	return line;
}

void FindLeft(char* line, char* left, char* right, char* command)
{
	*left = 0;
	*right = 0;
	int i = 0;
	int j = 0;
	for (; i < strlen(line) + 1; i++)
	{
		if (StartCompare(line + i, command))
		{
			*(left + i) = 0;
			break;
		}

		*(left + i) = *(line + i);
	}

	i += strlen(command);

	for (; i < strlen(line) + 1; i++, j++)
	{
		*(right + j) = *(line + i);
	}
}

int PlusMod2(int a, int b)
{
	int c = 0, i = 1;
	while (a || b)
	{
		if ((!(a & 1) && (b & 1)) || ((a & 1) && !(b & 1)))
		{
			c += i;
			i *= 2;
		}
		else
		{
			i *= 2;
		}
		a = a >> 1;
		b = b >> 1;
	}
	return c;
}

int Simple_Calculator(int left, int i_operation, int right, char* error = NULL)
{
	if (i_operation == 0)//add
		return left + right;
	if (i_operation == 1)//mult
		return left * right;
	if (i_operation == 2)//sub
		return left - right;
	if (i_operation == 4)//div
	{
		if (right == 0)
		{
			if (error)
			{
				strcpy(error, "Divizion by zero!");
				return 0;
			}
			else
			{
				printf("Error! Divizion by zero!\n");
				return 0;
			}
		}
		return left / right;
	}
	if (i_operation == 3)//pow
	{
		int temp = left;
		for (int i = right; i > 1; i--)
		{
			left *= temp;
		}
		return left;
	}
	if (i_operation == 5)//rem
		return left % right;
	if (i_operation == 6)//xor
		return PlusMod2(left, right);
}

/////////////////////////////////////////////

struct Node* Read(struct Node* tree, char* line, char save)
{
	if (save == 'R')
	{
		if (strlen(line) < 3)
			return NULL;

		if (*(line + strlen(line) - 1) == ';')
		{
			*(line + strlen(line) - 1) = 0;
		}
		else
		{
			return NULL;
		}
	}


	if (strlen(line) < 1)
		return NULL;

	int x = 0;
	printf("Enter %s: ", line);
	scanf("%d", &x);

	return Inseart_Node(tree, NewNode(line, x));
}

int Write(struct Node* tree, char* line, const char* end)
{
	if (strlen(line) < 1)
		return 1;

	if (end)
		for (int i = 0; i < strlen(end); i++)
		{
			if (*(line + strlen(line) - 1) == *(end + i))
			{
				*(line + strlen(line) - 1) = 0;
			}
			else
			{
				return 2;
			}
		}

	struct Node* find = SearchByKey(tree, line);

	if (find)
		PrintNode(find);
	else
		return 3;

	return 0;
}

struct Node* DoCalc(struct Node* tree, char* left, char* right, int mode, char save, char* op)
{
	if (strlen(right) < 4)
		return NULL;
	if (strlen(left) < 1)
		return NULL;

	if (save == 'L')
	{
		if (!strcmp(op, "OP()"))
		{
			if (*(right + strlen(right) - 1) == ';')
			{
				*(right + strlen(right) - 1) = 0;
			}
			else
			{
				return NULL;
			}

			if (*(right + strlen(right) - 1) == ')')
			{
				*(right + strlen(right) - 1) = 0;
			}
			else
			{
				return NULL;
			}
		}
	}
	else
	{
		if (!strcmp(op, "OP()"))
		{
			if (*(right + strlen(right) - 1) == ')')
			{
				*(right + strlen(right) - 1) = 0;
			}
			else
			{
				return NULL;
			}

			if (*(left + strlen(left) - 1) == ';')
			{
				*(left + strlen(left) - 1) = 0;
			}
			else
			{
				return NULL;
			}
		}
	}

	char* second = NULL;

	for (int i = 0; i < strlen(right); i++)
	{
		if (right[i] == ',')
		{
			*(right + i) = 0;
			second = right + i + 1;
		}
	}

	if (!second)
		return NULL;

#ifdef DEBUG
	printf("%s = <op>(%s,%s);\n", left, right, second);
#endif

	struct Node* to = SearchByKey(tree, left);
	struct Node* l = SearchByKey(tree, right);
	struct Node* r = SearchByKey(tree, second);

	if (!l || !r)
		return NULL;

	if (to)
	{
		to->data = Simple_Calculator(l->data, mode, r->data);
		return tree;
	}
	else
	{
		return Inseart_Node(tree, NewNode(left, Simple_Calculator(l->data, mode, r->data)));
	}

	return NULL;
}

void ErrorExit(char* line, const char* reason, FILE* in, struct Node* tree, char** commands)
{
	printf("It is an error in line \"%s\" (%s)\n", line, reason);
	fclose(in);
	Delete_Tree(tree);
	DeleteCommands(commands);
	system("pause");
	exit(-1);
}


int main(int arg_c, char** arg_v)
{
#ifdef DEBUG
	printf("arg_c: %d\n", arg_c);
	for (int i = 1; i < arg_c; i++)
	{
		printf("arg_v[%d]: %s\n", i, arg_v[i]);
	}
#endif

	if (arg_c < 3)
	{
		printf("I need more arg_v\n");
		exit(-1);
	}

	char** commands = SetCommandsDef();
	char save = 'L';
	char op[5] = "OP()";

	if (arg_c > 1)
		SetPropety(arg_v[1], commands, &save, op);
#ifdef DEBUG
	PrintCommands(commands, &save, op);
#endif	

	FILE* in;
	in = fopen(arg_v[2], "r");
	if (!in)
	{
		DeleteCommands(commands);
		exit(1);
	}

	char line[BUFSIZ], left[BUFSIZ], right[BUFSIZ], * ptr;
	struct Node* tree = NULL;
	int command_i = -1;

	while (!feof(in))
	{
		line[0] = 0;
		command_i = -1;
		if (!Get_Info(in, line))
		{
			ErrorExit(line, "Invalid input file", in, tree, commands);
		}
		if (strlen(line) == 0)
		{
			continue;
		}

		FindLeft(line, left, right, *(commands + 9));
#ifdef DEBUG
		printf("Line \"%s\" (len = %d)\n", line, strlen(line));
		printf("Left \"%s\"\n", left);
		printf("Right \"%s\"\n\n", right);
#endif
		/////////////////////////////////////////// output(A);
		if (!strcmp(op, "OP()"))
		{
			if (strlen(line) > strlen(*(commands + 8)) && StartCompare(line, *(commands + 8)) && (ptr = line + strlen(*(commands + 8)))
				&& *ptr++ == '(')
			{
				int err = Write(tree, ptr, ";)");

				switch (err)
				{
				case 1:ErrorExit(line, "too short line", in, tree, commands);
					break;
				case 2:ErrorExit(line, "i can`t find \");\"", in, tree, commands);
					break;
				case 3:ErrorExit(line, "Not init variable", in, tree, commands);
					break;
				}
				continue;
			}
		}
		else
		{
			ptr = left;
			char* ptr2 = FindClose(left) - 1;
			char* end = ptr + strlen(ptr) - 1;
			if (*ptr++ == '(' && *ptr2++ == ')' && *end == ';' && StartCompare(ptr2, *(commands + 8)))
			{
				*(ptr2 - 1) = 0;

				int err = Write(tree, ptr, NULL);

				switch (err)
				{
				case 1:ErrorExit(line, "too short line", in, tree, commands);
					break;
				case 2:ErrorExit(line, "i can`t find \");\"", in, tree, commands);
					break;
				case 3:ErrorExit(line, "Not init variable", in, tree, commands);
					break;
				}
				continue;
			}
		}
		//////////////
		if (save == 'L')
		{
			/////////////////////////////////////////// input(A);
			if (!strcmp(op, "OP()"))
			{
				if (strlen(right) > strlen(*(commands + 7)) && StartCompare(right, *(commands + 7)) && (ptr = right + strlen(*(commands + 7)))
					&& *ptr++ == '(' && *ptr++ == ')' && *ptr++ == ';' && *ptr++ == 0)
				{
					tree = Read(tree, left, save);
					if (!tree)
						ErrorExit(line, "Error in input", in, tree, commands);
					continue;
				}
			}
			else
			{
				ptr = right;
				char* end = ptr + strlen(ptr) - 1;
				if (*ptr++ == '(' && *ptr++ == ')' && *end == ';' && StartCompare(ptr, *(commands + 7)))
				{
					*end == 0;
					tree = Read(tree, left, save);
					if (!tree)
						ErrorExit(line, "Error in input", in, tree, commands);
					continue;
				}
			}
			/////////////////////////////////////////// A=<op>(B, C);
			if (!strcmp(op, "OP()"))
			{
				if (strlen(right) > 0)command_i = IsCommand(right, commands);
				if (command_i != -1 && (ptr = right + strlen(*(commands + command_i))) && *ptr++ == '(')
				{
					tree = DoCalc(tree, left, ptr, command_i, save, op);
					if (!tree)
						ErrorExit(line, "Error in calc", in, tree, commands);
					continue;
				}
			}
			else
			{
				char* ptr2 = FindClose(right);
				if (strlen(ptr2) > 0)command_i = IsCommand(ptr2, commands);
				if (command_i != -1 && (ptr = ptr2 + strlen(*(commands + command_i))) && *ptr++ == ';')
				{
					*--ptr2 = 0;
					tree = DoCalc(tree, left, right + 1, command_i, save, op);
					if (!tree)
						ErrorExit(line, "Error in calc", in, tree, commands);
					continue;
				}
			}

			///////////////////////////////////////////
			ErrorExit(line, "Unknown command", in, tree, commands);
		}
		else
		{
			/////////////////////////////////////////// input(A);
			if (!strcmp(op, "OP()"))
			{
				if (strlen(left) > strlen(*(commands + 7)) && StartCompare(left, *(commands + 7)) && (ptr = left + strlen(*(commands + 7)))
					&& *ptr++ == '(' && *ptr++ == ')')
				{
					tree = Read(tree, right, save);
					if (!tree)
						ErrorExit(line, "Error in input", in, tree, commands);
					continue;
				}
			}
			else
			{
				ptr = left;
				if (*ptr++ == '(' && *ptr++ == ')' && strlen(left) > strlen(*(commands + 7)) && !strcmp(ptr, *(commands + 7)))
				{
					tree = Read(tree, right, save);
					if (!tree)
						ErrorExit(line, "Error in input", in, tree, commands);
					continue;
				}
			}
			/////////////////////////////////////////// A=<op>(B, C);
			if (!strcmp(op, "OP()"))
			{
				if (strlen(left) > 0)command_i = IsCommand(left, commands);
				if (command_i != -1 && (ptr = left + strlen(*(commands + command_i))) && *ptr++ == '(')
				{
					tree = DoCalc(tree, right, ptr, command_i, save, op);
					if (!tree)
						ErrorExit(line, "Error in calc", in, tree, commands);
					continue;
				}
			}
			else
			{
				char* ptr2 = FindClose(left);
				if (strlen(ptr2) > 0)command_i = IsCommand(ptr2, commands);
				ptr = right + strlen(right) - 1;
				if (command_i != -1 && *ptr == ';')
				{
					*ptr = 0;
					*--ptr2 = 0;
					tree = DoCalc(tree, right, left + 1, command_i, save, op);
					if (!tree)
						ErrorExit(line, "Error in calc", in, tree, commands);
					continue;
				}
			}
			///////////////////////////////////////////
			ErrorExit(line, "Unknown command", in, tree, commands);
		}
	}

	printf("==========END==========\n", line);
	PrintTree(tree);
	Delete_Tree(tree);
	DeleteCommands(commands);

	system("pause");
	return 0;
}