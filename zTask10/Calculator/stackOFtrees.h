struct ItemStackTree//������� ����� ��������
{
	Node* data;
	struct ItemStackTree* prev;
};

typedef struct//����
{
	struct ItemStackTree* top;
	int size;
}stackOFtrees;

bool Create_stack_trees(stackOFtrees* s)//������� ����
{
	if (!s)
		return false;
	s->top = NULL;
	s->size = 0;
	return true;
}

bool Empty_stack_trees(stackOFtrees* s)//���� �� ����
{
	if (!s)
		return true;
	return s->top == NULL;
}

int Size_stack_trees(stackOFtrees* s)//������ �����
{
	if (!s)
		return -1;
	return s->size;
}

bool Push_stack_trees(stackOFtrees* s, Node* t)//�������� � ����
{
	if (!s)
		return false;
	struct ItemStackTree* i = (ItemStackTree*)malloc(sizeof(struct ItemStackTree));
	if (!i)
		return false;
	i->data = t;
	i->prev = s->top;
	s->top = i;
	s->size++;
	return true;
}

bool Pop_stack_trees(stackOFtrees* s)//������ �� �����
{
	if (!s)
	{
		return 0;
	}

	if (!s->size)
	{
		return 0;
	}

	struct ItemStackTree* i = s->top;
	s->top = s->top->prev;
	s->size--;
	free(i);
	return true;
}

Node* Top_stack_trees(stackOFtrees* s)//�������� �����
{
	if (!s)
		return 0;
	if (s->top)return s->top->data;
}

bool Destroy_stack_trees(stackOFtrees* s)//�������� ����
{
	if (!s)
		return false;
	while (s->top)
	{
		struct ItemStackTree* i = s->top;
		s->top = s->top->prev;
		free(i);
	}
	s->top = 0;
	s->size = 0;
	return true;
}