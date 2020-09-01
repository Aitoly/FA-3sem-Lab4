struct Item//элемент стека
{
	int data;
	struct Item* prev;
};

typedef struct//стек
{
	struct Item* top;
	int size;
}stack;

bool Create_stack(stack* s)//создать стек
{
	if (!s)
		return false;
	s->top = 0;
	s->size = 0;
	return true;
}

bool Empty_stack(stack* s)//пуст ли стек
{
	if (!s)
		return true;
	return s->top == 0;
}

int Size_stack(stack* s)//размер стека
{
	if (!s)
		return -1;
	return s->size;
}

bool Push_stack(stack* s, int t)//вставить в стек
{
	if (!s)
		return false;
	struct Item* i = (Item*)malloc(sizeof(struct Item));
	if (!i)
		return false;
	i->data = t;
	i->prev = s->top;
	s->top = i;
	s->size++;
	return true;
}

bool Pop_stack(stack* s)//убрать из стека
{
	if (!s)
	{
		return 0;
	}

	if (!s->size)
	{
		return 0;
	}

	struct Item* i = s->top;
	s->top = s->top->prev;
	s->size--;
	free(i);
	return true;
}

int Top_stack(stack* s)//верхушка стека
{
	if (!s)
		return 0;
	if (s->top)return s->top->data;
}

bool Destroy_stack(stack* s)//очистить стек
{
	if (!s)
		return false;
	while (s->top)
	{
		struct Item* i = s->top;
		s->top = s->top->prev;
		free(i);
	}
	s->top = 0;
	s->size = 0;
	return true;
}