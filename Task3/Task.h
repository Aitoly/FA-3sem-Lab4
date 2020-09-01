typedef struct
{
	char* str;
	int priority;
}Task;

Task* Create_Task(char* str, int priority)
{
	Task* task = (Task*)malloc(sizeof(Task));
	task->str = (char*)malloc(strlen(str) + 1);

	strcpy(task->str, str);
	task->priority = priority;

	return task;
}

void Destroy_Task(Task* task)
{
	free(task->str);
	free(task);
}

int EnqueuePriority(Task* t1, Task* t2)
{
	return t1->priority > t2->priority;
}