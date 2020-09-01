#pragma warning(disable:4996)

#include<stdio.h>
#include<iostream>
#include<ctype.h>
#include<math.h>
#include<time.h>

#include"List.h"

int Scan_File(List* l, const char* name)
{
	FILE* f;
	f = fopen(name, "r");
	if (!f)
		return 1;

	int prior;
	char buf[BUFSIZ];

	while (!feof(f))
	{
		if (!fscanf(f, "prior=%d task=\"", &prior))
		{
			return 2;
		}

		*buf = 0;
		fgets(buf, BUFSIZ, f);


		if (*buf)
		{
			*(buf + strlen(buf) - 2) = 0;
			//printf("%d::%s\n", prior, buf);
			Insert_Task(l, Create_Task(buf, prior));
		}

	}

	fclose(f);
	return 0;
}

int main()
{
	List list;
	Create(&list);

	char cmd[BUFSIZ];

	do
	{
		printf("Enter the command: ");
		gets_s(cmd);
		if (!strcmp(cmd, "help"))
		{
			printf("[help] - help\n");
			printf("[stop] - stop\n");
			printf("[print] - print queue\n");
			printf("[file [name_of_file].txt] - scan file\n");
		}
		else if (!strcmp(cmd, "print"))
		{
			PrintList(&list);
		}
		else if (*cmd == 'f' && *(cmd + 1) == 'i' && *(cmd + 2) == 'l' && *(cmd + 3) == 'e' && *(cmd + 4) == ' ')
		{

			int error = Scan_File(&list, cmd + 5);
			switch (error)
			{
			case 1: printf("I can`t open file \"%s\"\n", cmd + 5);
				break;
			case 2: printf("Invalid input file\n");
				break;
			}
		}
		else
		{
			if (strcmp(cmd, "stop"))
			{
				printf("Inavalid command\n");
			}
		}

	} while (strcmp(cmd, "stop"));

	Destroy_List(&list);
	return 0;
}