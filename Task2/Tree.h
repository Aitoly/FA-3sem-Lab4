struct Node
{
	char* key;
	int data;
	struct Node* l;
	struct Node* r;
};

struct Node* NewNode(char* s, int data)
{
	struct Node* pNode = NULL;
	pNode = (struct Node*)malloc(sizeof(struct Node));
	pNode->key = s;
	pNode->data = data;
	pNode->l = pNode->r = NULL;

	return pNode;
}

void PrintNode(struct Node* node)
{
	if (node == NULL)
		return;

	printf("%s = %d;", node->key, node->data);
	if (node->l)
		printf(" L = %s = %d;", node->l->key, node->l->data);
	if (node->r)
		printf(" R = %s = %d;", node->r->key, node->r->data);
	printf("\n");
}

void Walk(struct Node* root)
{
	if (root == NULL)
		return;

	Walk(root->l);
	PrintNode(root);
	Walk(root->r);
}

int SearchKey(struct Node* root, char* key)
{
	if (root == NULL)
		return 0;
	if (!strcmp(root->key, key))
		return root->data;

	if (strcmp(root->key, key) > 0)
		SearchKey(root->l, key);
	else
		SearchKey(root->r, key);
}