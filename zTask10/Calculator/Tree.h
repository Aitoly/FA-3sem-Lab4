struct Node
{
	char* key;
	int data;
	struct Node* l;
	struct Node* r;
};

struct Node* NewNodeByStr(char* key, int data)
{
	struct Node* pNode = NULL;
	pNode = (struct Node*)malloc(sizeof(struct Node));
	pNode->key = (char*)malloc(strlen(key) + 1);
	strcpy(pNode->key, key);
	pNode->data = data;
	pNode->l = pNode->r = NULL;

	return pNode;
}

void SetLeft(struct Node* node, struct Node* left)
{
	node->l = left;
}

void SetRight(struct Node* node, struct Node* right)
{
	node->r = right;
}

struct Node* NewNodeByCh(char key, int data)
{
	struct Node* pNode = NULL;
	pNode = (struct Node*)malloc(sizeof(struct Node));
	pNode->key = (char*)malloc(2);
	*(pNode->key) = key;
	*(pNode->key + 1) = 0;
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

	if (root->l)
	{
		Delete_Tree(root->l);
		root->l = NULL;
	}
	

	if (root->r)
	{
		Delete_Tree(root->r);
		root->r = NULL;
	}
	

	free(root->key);
	free(root);
}

struct Node* CreateTreeFromTree_h(struct Node* old_tree, struct Node* new_tree)
{
	if (old_tree == NULL)
		return NULL;

	new_tree = NewNodeByStr(old_tree->key, old_tree->data);
	new_tree->l = CreateTreeFromTree_h(old_tree->l, new_tree->l);
	new_tree->r = CreateTreeFromTree_h(old_tree->r, new_tree->r);
	return new_tree;
}

struct Node* CreateTreeFromTree(struct Node* old_tree, struct Node* new_tree)
{
	if (old_tree)
	{
		Delete_Tree(new_tree);
		new_tree = NewNodeByStr(old_tree->key, old_tree->data);
		new_tree->l = CreateTreeFromTree_h(old_tree->l, new_tree->l);
		new_tree->r = CreateTreeFromTree_h(old_tree->r, new_tree->r);
		return new_tree;
	}
	else
		return NULL;
}

void SetValForAllKeys(struct Node* node, char key, int data)
{
	if (!node)
		return;

	if (*(node->key) == key)
	{
		node->data = data;
	}

	SetValForAllKeys(node->l, key, data);
	SetValForAllKeys(node->r, key, data);
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


