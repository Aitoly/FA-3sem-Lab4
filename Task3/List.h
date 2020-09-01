#include"Task.h"

struct Item
{
	struct Item* prev;
	struct Item* next;
	Task* data;
};

typedef struct
{
	struct Item* node;
}Iterator;

int Equal(const Iterator* left, const Iterator* right)
{
	return left->node == right->node;
}

typedef struct
{
	struct Item* head;
	int size;
}List;

void Create(List* l)
{
	l->head = (struct Item*)malloc(sizeof(struct Item));
	l->head->next = l->head->prev = l->head;
	l->size = 0;
}

Iterator First(const List* l)
{
	Iterator res = { l->head->next };
	return res;
}

Iterator Last(const List* l)
{
	Iterator res = { l->head };
	return res;
}

int Empty(const List* l)
{
	Iterator first = First(l);
	Iterator last = Last(l);
	return Equal(&first, &last);
}

int Size(const List* l)
{
	return l->size;
}

Iterator Insert(List* l, Iterator* i, Task* t)
{
	Iterator res = { (struct Item*)malloc(sizeof(struct Item)) };

	if (!res.node)
		return Last(l);

	res.node->data = t;

	res.node->next = i->node;
	res.node->prev = i->node->prev;
	res.node->prev->next = res.node;
	i->node->prev = res.node;

	l->size++;

	return res;
}

void Insert_Task(List* l, Task* t)//Enqueue
{
	if (l->size == 0)
	{
		Insert(l, &First(l), t);
	}
	else
	{
		Iterator Iter = First(l);
		int ok = 0;

		for (int i = 0; i < l->size; i++)
		{
			if (EnqueuePriority(Iter.node->data, t))//Iter.node->data->priority > t->priority
			{
				Insert(l, &Iter, t);
				ok = 1;
				break;
			}
			Iter.node = Iter.node->next;
		}

		if (!ok)
		{
			Insert(l, &Last(l), t);
		}
	}
}

Iterator Delete(List* l, Iterator* i)//Dequeue
{
	Iterator res = Last(l);
	if (Equal(i, &res))
		return res;

	res.node = i->node->next;
	res.node->prev = i->node->prev;
	i->node->prev->next = res.node;
	l->size--;
	Destroy_Task(i->node->data);
	free(i->node);
	i->node = 0;
	return res;
}

void Destroy_List(List* l)
{
	struct Item* i = l->head->next;
	while (i != l->head)
	{
		struct Item* pi = i;//l->head
		i = i->next;
		Destroy_Task(pi->data);
		free(pi);
	}
	free(l->head);
	l->head = 0;
	l->size = 0;
}

void PrintListMember(const Iterator* i)
{
	if (i->node)
		printf("prior=%d task=\"%s\"\n", i->node->data->priority, i->node->data->str);
}

void PrintListItem(const struct Item* i)
{
	if (i)
		printf("prior=%d task=\"%s\"\n", i->data->priority, i->data->str);
}

void PrintList(const List* l)
{
	struct Item* i = l->head->next;
	while (i != l->head)
	{
		PrintListItem(i);
		i = i->next;
	}
}