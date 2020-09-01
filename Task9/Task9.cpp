#pragma warning(disable:4996)

#include<stdio.h>
#include<iostream>
#include<ctype.h>
#include<math.h>
#include<time.h>

#include"TreeList.h"


void Get_Line(FILE* in, char* line)
{
	if (!in)
	{
		printf("I can`t open in.txt\n");
		exit(-1);
	}

	char c;

	while (!feof(in))
	{
		c = fgetc(in);

		if (isspace(c) || c == ',')
		{
			continue;
		}
		if (c == '\n' || c == EOF)
		{
			*line = '\0';
			return;
		}

		*line++ = c;
	}
}

NodeList* FindTree(char* line)
{
	NodeList** way = NULL;
	int countWay = 0;
	NodeList* current = NULL;
	NodeList* tree = NULL;

	if (isalpha(*line) && *(line + 1) == '(' && *(line + strlen(line) - 1) == ')')
	{
		tree = NewNode(*line);
		current = tree;
		*(line + strlen(line) - 1) = 0;
		line += 2;
	}
	else
	{
		return tree;
	}

	while (*line != 0)
	{
		if (*line == '(')
		{
			countWay++;
			way = (NodeList**)realloc(way, sizeof(NodeList*) * countWay);
			NodeList** n = way + countWay - 1;
			*n = current;
			current = *(current->data + current->count - 1);
		}
		else if (*line == ')')
		{
			countWay--;
			current = *(way + countWay);
			way = (NodeList**)realloc(way, sizeof(NodeList*) * countWay);
		}
		else if (isalpha(*line))
		{
			addTreeToNode(current, NewNode(*line));
		}
		else
		{
			DeleteTree(tree);
			printf("ERROR\n");
			exit(-1);
		}

		line++;
	}

	return tree;
}

int main()
{

	FILE* in;
	in = fopen("in.txt", "r");
	if (!in)
	{
		printf("I can`t open in.txt\n");
		exit(-1);
	}

	char line[BUFSIZ];

	while (!feof(in))
	{
		*line = 0;
		Get_Line(in, line);
		if (strlen(line) > 4)
		{
			printf("%s\n", line);
		}

		NodeList* tree = FindTree(line);

		PrintTree(tree);

		DeleteTree(tree);
	}

	fclose(in);

	return 0;
}