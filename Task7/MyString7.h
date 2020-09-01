#include<iostream>

typedef struct
{
	char *data;
	int size;
}String;

void create(String *s)
{
	s->data = NULL;
	s->size = 0;
}

void create(String *s, char *in)
{
	s->data = (char*)malloc(strlen(in) + 1);
	s->size = strlen(in);
	strcpy(s->data, in);
}

void del(String *s)
{
	if (s->data)
	{
		delete[] s->data;
	}
}

void add(String *s, char c)
{
	int old_size = s->size;
	if (old_size > 0)
	{
		char* toCopy = new char[old_size + 1];
		strcpy(toCopy, s->data);
		delete[] s->data;

		s->data = new char[old_size + 2];
		strcpy(s->data, toCopy);

		s->data[old_size] = c;
		s->data[old_size + 1] = '\0';
		delete[] toCopy;
	}
	else
	{
		s->data = new char[2];
		s->data[0] = c;
		s->data[1] = '\0';
	}
	s->size++;

}

void add_in_home(String *s, char c)
{
	int old_size = s->size;
	if (old_size > 0)
	{
		char* toCopy = new char[old_size + 1];
		strcpy(toCopy, s->data);
		delete[] s->data;

		s->data = new char[old_size + 2];
		s->data[0] = c;
		for (int i = 1; i < old_size + 2; i++)
		{
			s->data[i] = toCopy[i - 1];
		}
		s->data[old_size + 1] = '\0';
		delete[] toCopy;
	}
	else
	{
		s->data = new char[2];
		s->data[0] = c;
		s->data[1] = '\0';
	}
	s->size++;

}