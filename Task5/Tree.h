struct Node
{
	char* key;
	int data;
	struct Node* l;
	struct Node* r;
};

struct Node* NewNode(char* key, int data)
{
	struct Node* pNode = NULL;
	pNode = (struct Node*)malloc(sizeof(struct Node));
	pNode->key = (char*)malloc(strlen(key) + 1);
	strcpy(pNode->key, key);
	pNode->data = data;
	pNode->l = pNode->r = NULL;

	return pNode;
}

void PrintNode(struct Node* node, FILE *f = NULL, char side = ' ')
{
	if (node == NULL)
		return;

	if (f)
	{
		fprintf(f, "%s = %d %c\n", node->key, node->data, side);
	}
	else
	{
		printf("%s = %d;", node->key, node->data);
#ifdef DEBUG
		if (node->l)
			printf(" L = %s = %d;", node->l->key, node->l->data);
		if (node->r)
			printf(" R = %s = %d;", node->r->key, node->r->data);
#endif
		printf("\n");
	}
}

void PrintTree(struct Node* root, int mode = 0)
{
	if (root == NULL)
		return;
	
	switch (mode)
	{
	case 0://симметричный обход бинарного дерева
		PrintTree(root->l);
		PrintNode(root);
		PrintTree(root->r);
		break;
	case 1://прямой обход бинарного дерева
		PrintNode(root);
		PrintTree(root->l);
		PrintTree(root->r);
		break;
	case 2://обратный обход бинарного дерева
		PrintTree(root->l);
		PrintTree(root->r);
		PrintNode(root);
		break;
	}
}

struct Node* SearchByKey(struct Node* root, const char* key)
{
	if (root == NULL)
		return NULL;

	int cmp = strcmp(root->key, key);
	if (!cmp)
		return root;
	else if (cmp > 0)
		SearchByKey(root->l, key);
	else
		SearchByKey(root->r, key);
}

void Inseart_Node_help(struct Node* root, struct Node* new_node)
{
	int cmp = strcmp(root->key, new_node->key);

	if (!cmp)
	{
		return;
	}
	else if (cmp > 0)
	{
		if (root->l == NULL)
		{
			root->l = new_node;
			return;
		}
		else
			Inseart_Node_help(root->l, new_node);
	}
	else
	{
		if (root->r == NULL)
		{
			root->r = new_node;
			return;
		}
		else
			Inseart_Node_help(root->r, new_node);
	}

}

struct Node* Inseart_Node(struct Node* root, struct Node* new_node)
{
	if (root == NULL)
	{
		return new_node;
	}
	else
	{
		Inseart_Node_help(root, new_node);
		return root;
	}
}

void Delete_Tree(struct Node* root)
{
	if (root == NULL)
		return;

	Delete_Tree(root->l);
	Delete_Tree(root->r);

	free(root->key);
	free(root);
}

struct Node* CreateTreeFromTree_h(struct Node* old_tree, struct Node* new_tree)
{
	if (old_tree == NULL)
		return new_tree;

	new_tree = CreateTreeFromTree_h(old_tree->l, new_tree);
	new_tree = Inseart_Node(new_tree, NewNode(old_tree->key, old_tree->data));
	new_tree = CreateTreeFromTree_h(old_tree->r, new_tree);

	return new_tree;
}

struct Node* CreateTreeFromTree(struct Node* old_tree, struct Node* new_tree)
{
	if (old_tree)
	{
		Delete_Tree(new_tree);
		new_tree = CreateTreeFromTree_h(old_tree, new_tree);
		return new_tree;
	}
	else
		return NULL;
}

////////////////////////////////////////
////////////////////////////////////////

int HeightOfTree(struct Node* tree, int n = 0)
{
	if (tree == NULL)
		return n;

	n++;
	int a = HeightOfTree(tree->l, n);
	int b = HeightOfTree(tree->r, n);

	if (a > b)
		return a;
	else
		return b;
}

void SaveTree_h(struct Node* tree, FILE* f, char side = 'c')
{
	if (!f)
		return;

	if (!tree)
		return;

	PrintNode(tree, f, side);
	if (tree->l && tree->r)
	{
		SaveTree_h(tree->l, f, 'B');
		SaveTree_h(tree->r, f, 'b');
	}
	else
	{
		SaveTree_h(tree->l, f, 'l');
		SaveTree_h(tree->r, f, 'r');
	}
		
	
	
}

void SaveTree(struct Node* tree, const char* name)
{
	FILE* f;
	if (!(strlen(name) > 4 && !strcmp(name + strlen(name) - 4, ".txt")))
		return;

	f = fopen(name, "w");
	if (f)
	{
		SaveTree_h(tree, f);
		fclose(f);
	}
}

struct Node* LoadTree(const char* name)
{
	FILE* f;
	if (!(strlen(name) > 4 && !strcmp(name + strlen(name) - 4, ".txt")))
		return NULL;

	f = fopen(name, "r");
	if (!f)
	{
		return NULL;
	}

	struct Node* res = NULL;
	struct Node* now = NULL;
	struct Node** now_both = NULL;
	int now_both_count = 0;
	char key[BUFSIZ];
	int data;
	char side;

	while (!feof(f))
	{
		if (fscanf(f, "%s = %d %c\n", key, &data, &side) == 3)
		{
			switch (side)
			{
			case'c':
				res = NewNode(key, data);
				now = res;
				break;
			case'l':
				now->l = NewNode(key, data);
				now = now->l;
				break;
			case'r':
				now->r = NewNode(key, data);
				now = now->r;
				break;
			case'B':
				now_both_count++;
				now_both = (struct Node**)realloc(now_both, now_both_count * sizeof(struct Node*));
				*(now_both + now_both_count - 1) = now;

				now->l = NewNode(key, data);
				now = now->l;
				break;
			case'b':
				now_both_count--;
				(*(now_both + now_both_count))->r = NewNode(key, data);
				now = (*(now_both + now_both_count))->r;

				now_both = (struct Node**)realloc(now_both, now_both_count * sizeof(struct Node*));
				break;
			}
		}
	}

	return res;
}