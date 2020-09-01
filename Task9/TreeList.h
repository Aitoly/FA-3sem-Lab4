
struct NodeList
{
	char key;
	struct NodeList** data;
	int count;
};

struct NodeList* NewNode(char key)
{
	struct NodeList* pNode = NULL;
	pNode = (struct NodeList*)malloc(sizeof(struct NodeList));
	pNode->key = key;
	pNode->data = NULL;
	pNode->count = 0;
	
	return pNode;
}

void DeleteTree(NodeList* node)
{
	if (node)
	{
		for (int i = 0; i < node->count; ++i)
		{
			DeleteTree(*(node->data + i));
		}
		free(node);
	}
}

void addTreeToNode(NodeList* node, NodeList* new_node)
{
	node->count++;
	node->data = (NodeList**)realloc(node->data, sizeof(NodeList*) * node->count);
	NodeList** n = node->data + node->count - 1;
	*n = new_node;
}

void PrintTree(NodeList* node, int x = 0)
{
	for (int i = 0; i < x; ++i)
	{
		printf(" ");
	}

	printf("%c\n", node->key);

	for (int i = 0; i < node->count; ++i)
	{
		PrintTree(*(node->data + i), x + 1);
	}
}