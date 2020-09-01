#pragma warning(disable:4996)

#include<stdio.h>
#include<iostream>
#include<ctype.h>
#include<math.h>
#include<time.h>

//#define DEBUG 1

typedef struct {
	char* name;
	int data;
} MemoryBox;

MemoryBox* DATA = NULL;
int size = 0;

int compare(const void* box1, const void* box2)
{
	if (!box1 && box2)
	{
		return 1;
	}
	if (box1 && !box2)
	{
		return -1;
	}
	if (!box1 && !box2)
	{
		return 0;
	}
	return strcmp(((MemoryBox*)box1)->name, ((MemoryBox*)box2)->name);
}

MemoryBox* FindBad(char* name)
{
	for (int i = 0; i < size; i++)
	{
		if (!strcmp(name, DATA[i].name))
			return &DATA[i];
	}

	return NULL;
}

int FindBadInt(char* name)
{
	for (int i = 0; i < size; i++)
	{
		if (!strcmp(name, DATA[i].name))
			return i;
	}

	return -1;
}

MemoryBox* Find(char* name)
{
	if (DATA == NULL)
		return NULL;

	int x = -1, _x = -2, a = 0, b = size;
	const int res = FindBadInt(name);

	while (true)
	{
		_x = x;
		x = (a + b) / 2;
		if (x == _x)
			return NULL;

		if (!strcmp(name, DATA[x].name))
			return &DATA[x];

		if (strcmp(name, DATA[x].name) > 0)
		{
			a = x;
		}
		else
		{
			b = x;
		}
	}
}

void add(char* name, int data)
{
	size++;
	DATA = (MemoryBox*)realloc(DATA, size * sizeof(MemoryBox));
	DATA[size - 1].data = data;
	DATA[size - 1].name = (char*)malloc((strlen(name) + 1) * sizeof(char));
	strcpy(DATA[size - 1].name, name);

	qsort(DATA, size, sizeof(MemoryBox), compare);
}

int char_to_int(char c)
{
	if (isalpha(c))
	{
		c = toupper(c) - 'A' + 10;
	}
	else if (isdigit(c))
	{
		return c - '0';
	}
	else
	{
		return -1;
	}
}

int string_to_int(char* old_str, int old_base)
{
	if (old_str == NULL)
	{
		return 0;
	}
	int old_num = 0;
	for (int i = 0; i < strlen(old_str); i++)
	{
		old_num += char_to_int(old_str[strlen(old_str) - i - 1]) * pow(old_base, i);
	}
	return old_num;
}

int Simple_Calculator(int left, char operation, int right, char* error = NULL)
{
#ifdef DEBUG
	printf("%d %c %d\n", left, operation, right);
#endif
	if (operation == '+')
		return left + right;
	if (operation == '-')
		return left - right;
	if (operation == '*')
		return left * right;
	if (operation == '/')
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
	if (operation == '%')
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
	return left % right;

}

int isOperation(char c)
{
	switch (c)
	{
	case '=': return 1;  break;
	case '+': return 2;  break;
	case '-': return 3;  break;
	case '*': return 4;  break;
	case '/': return 5;  break;
	case '%': return 6;  break;
	}
	return 0;
}

int Get_Info(FILE* in, char* to, char* left, char* right, char* op)
{
	/*
	0 - ошибка
	1 - a=b;
	2 - a=b+c;
	3 - print a;
	4 - print;
	*/

	*to = 0; *left = 0; *right - 0;

	if (!in)
	{
		printf("File not opend!\n");
		return 0;
	}

	char c;
	int mode = 1;

	//1 - собираем to
	//2 - собираем left
	//3 - собираем rght

	char buf[BUFSIZ] = { 0 };
	char* pbuf = buf;

	while (!feof(in))
	{
		c = fgetc(in);

		if (mode == -2)
		{
			if (c == ' ')
			{
				mode = -1;
				pbuf = buf;
				*pbuf = '\0';
			}
			else if (c == ';')
				return 4;
			else
				mode = 1;
		}

		if (isspace(c) || c == '\n' || c == EOF)
		{
			continue;
		}

		if (mode == -1)
		{

			if (c == ';')
			{
				if (*buf == '\0')
				{
					return 4;
				}
				else
				{
					*pbuf = '\0';
					strcpy(to, buf);
					return 3;
				}
			}
			else if (isOperation(c))
			{
				printf("The operation %c beside print!\n", c);
				return 0;
			}
			else
			{
				*pbuf++ = c;
			}
		}
		else if (mode == 1)
		{

			if (c == ';')
			{
				printf("I find only to operand! (%s)\n", buf);
				return 0;
			}
			else if (isOperation(c))
			{
				if (isOperation(c) != 1)
				{
					printf("The first operation mast be =, but I find %c !\n", c);
					return 0;
				}

				strcpy(to, buf);
				pbuf = buf;
				*pbuf = '\0';
				mode++;
			}
			else
			{
				*pbuf++ = c;
				*pbuf = '\0';
				if (!strcmp("print", buf))
				{
					mode = -2;
				}
			}
		}
		else if (mode == 2)
		{
			if (c == ';')
			{
				*pbuf = '\0';
				strcpy(left, buf);
				return 1;
			}
			else if (isOperation(c))
			{
				*op = c;
				*pbuf = '\0';
				strcpy(left, buf);
				pbuf = buf;
				*pbuf = '\0';
				mode++;
			}
			else
			{
				*pbuf++ = c;
			}
		}
		else if (mode == 3)
		{
			if (c == ';')
			{
				*pbuf = '\0';
				strcpy(right, buf);
				return 2;
			}
			else if (isOperation(c))
			{
				printf("I find operation %c beside right operand!\n", c);
				return 0;
			}
			else
			{
				*pbuf++ = c;
			}
		}
	}

	return 0;
}

int Do_operation(int type, char* to, char* left, char* right, char op)
{
	MemoryBox* t;
	int BinINT = 0;
	int BisDIGIT = 1;
	int CinINT = 0;
	int CisDIGIT = 1;

	switch (type)
	{
	case 1: //a=b;
		if (isdigit(*to))
			return 0;

		for (int i = 0; i < strlen(left); i++)
		{
			if (!isdigit(left[i]))
			{
				BisDIGIT = 0;
			}
		}

		if (BisDIGIT)
		{
			BinINT = string_to_int(left, 10);
		}
		else
		{
			t = Find(left);
			if (t)
			{
				BinINT = t->data;
			}
			else
			{
				printf("You try to use not init variable %s\n", to);
				return 0;
			}
		}

		t = Find(to);
		if (t)
		{
			t->data = BinINT;
		}
		else
		{
			add(to, BinINT);
		}

		break;
	case 2: //a=b+с;
		if (isdigit(*to))
			return 0;

		for (int i = 0; i < strlen(left); i++)
		{
			if (!isdigit(left[i]))
			{
				BisDIGIT = 0;
			}
		}

		for (int i = 0; i < strlen(right); i++)
		{
			if (!isdigit(right[i]))
			{
				CisDIGIT = 0;
			}
		}

		if (BisDIGIT)
		{
			BinINT = string_to_int(left, 10);
		}
		else
		{
			t = Find(left);
			if (t)
			{
				BinINT = t->data;
			}
			else
			{
				printf("You try to use not init variable %s\n", left);
				return 0;
			}
		}

		if (CisDIGIT)
		{
			CinINT = string_to_int(right, 10);
		}
		else
		{
			t = Find(right);
			if (t)
			{
				CinINT = t->data;
			}
			else
			{
				printf("You try to use not init variable %s\n", right);
				return 0;
			}
		}

		t = Find(to);
		if (t)
		{
			char error[BUFSIZ] = { 0 };
			int res = Simple_Calculator(BinINT, op, CinINT, error);
			if (*error)
			{
				printf("%s\n", error);
				return 0;
			}
			t->data = res;
		}
		else
		{
			char error[BUFSIZ] = { 0 };
			int res = Simple_Calculator(BinINT, op, CinINT, error);
			if (*error)
			{
				printf("%s\n", error);
				return 0;
			}
			add(to, res);
		}

		break;
	case 3:// print(a);
		t = Find(to);
		if (t)
		{
			printf("%s = %d\n", t->name, t->data);
		}
		else
		{
			printf("You try to print not init variable %s\n", to);
		}
		break;
	case 4:// print();
		for (int i = 0; i < size; i++)
		{
			printf("%s = %d\n", DATA[i].name, DATA[i].data);
		}
		break;
	}

	return 1;
}

int main()
{
	FILE* in;
	in = fopen("in.txt", "r");
	if (!in)
	{
		printf("I can`t open input file\n");
		system("pause");
		exit(-1);
	}

	////////////////////////////////////////////////////
	////////////////////////////////////////////////////

	char DEF_TO[BUFSIZ] = { 0 };
	char DEF_LEFT[BUFSIZ] = { 0 };
	char DEF_RIGHRT[BUFSIZ] = { 0 };
	char op = ' ';

	while (!feof(in))
	{
		int type = Get_Info(in, DEF_TO, DEF_LEFT, DEF_RIGHRT, &op);

		if (!type)
		{
			system("pause");
			exit(-1);
		}
		else
		{
#ifdef DEBUG
			switch (type)
			{
			case 1: printf("\n%s = %s\n", DEF_TO, DEF_LEFT);  break;
			case 2: printf("\n%s = %s %c %s\n", DEF_TO, DEF_LEFT, op, DEF_RIGHRT);  break;
			case 3: printf("\nprint(%s);\n", DEF_TO);  break;
			case 4: printf("\nprint(all);\n");  break;
			}
#endif
			int res = Do_operation(type, DEF_TO, DEF_LEFT, DEF_RIGHRT, op);
		}


	}

	fclose(in);

	free(DATA);

	system("pause");
	return 0;
}
