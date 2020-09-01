#pragma warning(disable:4996)

#include<stdio.h>
#include<iostream>
#include<ctype.h>
#include<math.h>
#include<time.h>

#define DEBUGG

#include"Tree.h"
#include"MyString.h"

void sbor_leks(FILE* f, String* string)
{
	del(string);
	create(string);
	char c = ' ';

	while (c == ' ' || c == '\t' || c == '\n' || c == '\0' || c == EOF)
	{
		if (c == EOF)
		{
			//*eof = true;
			return;
		}
		c = getc(f);
	}

	while (true)
	{
		if (c == ' ' || c == '\t' || c == '\n' || c == '\0')
		{
			return;
		}
		else if (c == EOF)
		{
			//*eof = true;
			return;
		}
		else
		{
			add(string, c);
		}
		c = getc(f);
	}
}

struct Node* ScanFileToTree(const char* name)
{
	struct Node* tree = NULL;

	FILE* in;
	in = fopen(name, "r");
	if (!in)
	{
		return tree;
	}

	String leks;
	create(&leks);

	while (!feof(in))
	{
		sbor_leks(in, &leks);

		if (leks.data)
		{
#ifdef DEBUG
			printf("# %s\n", leks.data);
#endif
			struct Node* search = SearchByKey(tree, leks.data);

			if (search)
			{
				search->data++;
			}
			else
			{
				tree = Inseart_Node(tree, NewNode(leks.data, 1));
			}
		}
	}

	del(&leks);
	return tree;
}

int StatisticsAboutWorld(struct Node* tree, const char* world)
{
	struct Node* search = SearchByKey(tree, world);

	if (search)
	{
		return search->data;
	}
	else
	{
		return 0;
	}
}


/////////////////////TheLongest
struct Node* TheLongest_h(struct Node* tree, struct Node* res)
{
	if (tree->l)
		res = TheLongest_h(tree->l, res);

	if (strlen(tree->key) > strlen(res->key))
		res = tree;

	if (tree->r)
		res = TheLongest_h(tree->r, res);

	return res;

}

struct Node* TheLongest(struct Node* tree)
{
	if (tree == NULL)
	{
		return NULL;
	}
	else
	{
		struct Node* res = tree;
		if (tree->l)
			res = TheLongest_h(tree->l, res);

		if (tree->r)
			res = TheLongest_h(tree->r, res);

		return res;
	}
}
//////////////////


/////////////////////TheShortest
struct Node* TheShortest_h(struct Node* tree, struct Node* res)
{
	if (tree->l)
		res = TheShortest_h(tree->l, res);

	if (strlen(tree->key) < strlen(res->key))
		res = tree;

	if (tree->r)
		res = TheShortest_h(tree->r, res);

	return res;

}

struct Node* TheShortest(struct Node* tree)
{
	if (tree == NULL)
	{
		return NULL;
	}
	else
	{
		struct Node* res = tree;
		if (tree->l)
			res = TheShortest_h(tree->l, res);

		if (tree->r)
			res = TheShortest_h(tree->r, res);

		return res;
	}
}
//////////////////

/////////////////////TheShortest
struct Node* MostCommon_h(struct Node* tree, struct Node* res)
{
	if (tree->l)
		res = MostCommon_h(tree->l, res);

	if (tree->data > res->data)
		res = tree;

	if (tree->r)
		res = MostCommon_h(tree->r, res);

	return res;

}

struct Node* MostCommon(struct Node* tree)
{
	if (tree == NULL)
	{
		return NULL;
	}
	else
	{
		struct Node* res = tree;
		if (tree->l)
			res = MostCommon_h(tree->l, res);

		if (tree->r)
			res = MostCommon_h(tree->r, res);

		return res;
	}
}
//////////////////


/////////////////////MostCommonWords
struct Node** MostCommonWords(struct Node* tree, const int n)
{
	int N = 0;
	struct Node** mass = NULL;
	struct Node* tree_t = NULL;
	struct Node* find = NULL;
	tree_t = CreateTreeFromTree(tree, tree_t);

	for (int i = 0; i < n; i++)
	{
		if (!tree_t)
			break;

		find = MostCommon(tree_t);
		//printf("!!! %s\n", find->key);
		N++;
		mass = (struct Node**)realloc(mass, sizeof(struct Node*) * N);
		struct Node** in = mass + N - 1;
		*in = NewNode(find->key, find->data);
		find->data = 0;
	}

	Delete_Tree(tree_t);
	return mass;
}

void PrintMostCommonWords(struct Node** nodes, const int n)
{
	if (!nodes)
		return;

	for (int i = 0; i < n; i++)
	{
		if (*(nodes + i))
			printf("%s\n", (*(nodes + i))->key);
	}
}
//////////////////


int main()
{
	struct Node* tree = ScanFileToTree("in.txt");

	PrintTree(tree);

	printf("\nWord \"%s\" mets %d times\n", "word!", StatisticsAboutWorld(tree, "word!"));

	printf("\n%d Most Common Words:\n", 3);
	PrintMostCommonWords(MostCommonWords(tree, 3), 3);

	printf("\nThe longest word: %s\n", TheLongest(tree)->key);

	printf("\nThe shortest word: %s\n", TheShortest(tree)->key);

	printf("\nHeight Of Tree: %d\n", HeightOfTree(tree));

	SaveTree(tree, "xyy.txt");

	Delete_Tree(tree);
	///////////////////////////////////
	tree = NULL;

	tree = LoadTree("xyy.txt");

	PrintTree(tree);

	Delete_Tree(tree);

	return 0;
}