#include"Calculator/Tree.h"
#include"Calculator/stackOFtrees.h"
#include"Calculator/stack.h"

void NormString(char* s)
{
	int len = strlen(s);
	for (int i = 0; i < len; ++i)
		if (*(s + i) == '>')*(s + i) = ' ';
}

void ERROR(stack* stack, stackOFtrees* TREES)
{
	Destroy_stack(stack);
	Destroy_stack_trees(TREES);
}

int PRIOR(char a)
{
	switch (a)
	{
	case '~':
		return 4;

	case '?':
	case '!':
	case '+': //+>
	case '&':
		return 3;

	case '|':
	case '-': //->
	case '<': //<>
	case '=':
		return 2;

	case '(':
		return 1;
	case '{':
		return 1;
	case '[':
		return 1;
	}
}

int Simple_Bin_Calculator(int left, char operation, int right, char* error = NULL)
{
#ifdef DEBUG
	printf("%d %c %d\n", left, operation, right);
#endif

	if (left < 0 || left > 1 || right < 0 || right > 1)
	{
		if (error)
		{
			strcpy(error, "Only binary!");
			return 0;
		}
		else
		{
			printf("Error! Only binary!\n");
			return 0;
		}
	}

	if (operation == '&')//����������
		return left && right;
	if (operation == '|')//����������
		return left || right;
	if (operation == '~')//���������
		return !left;
	if (operation == '-')//����������		->
		return !(left && !right);
	if (operation == '<')//������� ����������(�������� �� ������ 2)		<>
		return left != right;
	if (operation == '=')//������������
		return left == right;
	if (operation == '!')//����� �������
		return !(left && right);
	if (operation == '?')//������� �����
		return !(left || right);
	if (operation == '+')//������������		+>
		return !(!left && right);
}

bool open_bracket(char c)
{
	return c == '(' || c == '[' || c == '{';
}

bool close_bracket(char c)
{
	return c == ')' || c == ']' || c == '}';
}

bool operation(char c)
{
		return c == '&'//����������
		|| c == '|' //����������
		|| c == '~' //���������
		|| c == '-' //����������		->
		|| c == '<' //������� ����������(�������� �� ������ 2)		<>
		|| c == '=' //������������
		|| c == '!' //����� �������
		|| c == '?' //������� �����
		|| c == '+';//������������		+>
}

char open_bracket_for_close(char c)
{
	switch (c)
	{
	case')': return'('; break;
	case'}': return'{'; break;
	case']': return'['; break;
	}
	return' ';
}

bool nesting_of_brackets_in_string(char* string)
{
	stack s;//���� ������
	Create_stack(&s);

	while (*string != '\0')//���� ������ �� ����� �����
	{
		//----------------------------
		if (open_bracket(*string))
		{
			Push_stack(&s, *string);
		}
		else if (close_bracket(*string))
		{
			if (Top_stack(&s) == open_bracket_for_close(*string))
			{
				Pop_stack(&s);
			}
			else
			{
				//printf("Nesting of brackets is not observed.\n");
				Destroy_stack(&s);
				return false;
			}
		}
		string++;
	}

	if (Empty_stack(&s))
	{
		//printf("Nesting of brackets is observed.\n");
		Destroy_stack(&s);
		return true;
	}
	else
	{
		//printf("Nesting of brackets is not observed.\n");
		Destroy_stack(&s);
		return false;
	}
}

void solve_tree_h(struct Node* node, char* error)
{
	if (*(node->key) == '~')
	{
		if (node->l && !node->r)
		{
			if (!operation(*(node->l->key)))
			{
				int res = Simple_Bin_Calculator(node->l->data, *(node->key), 0/*, char* error = NULL*/);
				Delete_Tree(node->l);
				node->l = NULL;

				*(node->key) = 'a';
				node->data = res;
			}
			else
			{
				solve_tree_h(node->l, error);
				solve_tree_h(node, error);
			}
		}
		else
		{
#ifdef DEBUG
			printf("\nError! It`s a problem with ~ %c\n", c);
#endif
			if (error)
			{
				strcpy(error, "It`s a problem with ~!");
			}
			return;
		}
	}
	else if (!operation(*(node->l->key)) && !operation(*(node->r->key)) && operation(*(node->key)))
	{
		int res = Simple_Bin_Calculator(node->l->data, *(node->key), node->r->data/*, char* error = NULL*/);
		Delete_Tree(node->l);
		node->l = NULL;
		Delete_Tree(node->r);
		node->r = NULL;

		*(node->key) = 'a';
		node->data = res;
	}
	else
	{
		if (node->l && operation(*(node->l->key)))
			solve_tree_h(node->l, error);
	

		if (node->r && operation(*(node->r->key)))
			solve_tree_h(node->r, error);

		solve_tree_h(node, error);
	}
}

int solve_tree(struct Node* node, char* error)
{
	solve_tree_h(node, error);

	if (*error != 0)
	{
		return 0;
	}

	if (HeightOfTree(node) == 1)
	{
		return node->data;
	}
	else
	{
		return 2;
	}
}

void SolveForAllData(struct Node* node_original, char* variables, int count, char* error)
{
	struct Node* node = NULL;
	int mask, print, res, i;
	mask = (int)(pow(2, count) + 0.5);

	while (mask)
	{
		Delete_Tree(node);
		node = NULL;
		node = CreateTreeFromTree(node_original, node);
		mask--;
		print = count;
		int t = mask;
		i = 0;
		while (t)
		{
			if (t & 1)
			{
				printf("1 ");
				print--;
				SetValForAllKeys(node, *(variables + i), 1);
				i++;
			}
			else
			{
				printf("0 ");
				print--;
				SetValForAllKeys(node, *(variables + i), 0);
				i++;
			}
			t = t >> 1;
		}
		while (print)
		{
			printf("0 ");
			print--;
			SetValForAllKeys(node, *(variables + i), 0);
			i++;
		}

		int result = solve_tree(node, error);
		if (*error != 0)
		{
			return;
		}
		printf(" = %d\n", result);
		
	}
}







int Calculator(char* in_string, char* result_string, char* print_string, char* error)
{
	if (error)
	{
		*error = 0;
	}

	if (*in_string == '\0' ||  * in_string == '\n' || * in_string == EOF)
	{
		if (error)
		{
			strcpy(error, "Empty line!");
		}
		return 0;
	}

	if (!nesting_of_brackets_in_string(in_string))
	{
		if (error)
		{
			strcpy(error, "Nesting of brackets is not observed.");
		}
		return 0;
	}

	stack OPERS;//���� ���p����
	Create_stack(&OPERS);

	stackOFtrees TREES;//���� ��������
	Create_stack_trees(&TREES);

	char* variables = NULL;
	int count_var = 0;

	char c = '\0';//������� ������
	char prev_c = '\0';//������ �� ��������

	//===============================================================================================================
	//��������� ������
	//===============================================================================================================

	c = *in_string++;

	while (c != '\0' &&  c != '\n' && c != EOF)// �����p��� , ���� �� ������ �� '\0' 
	{
		if (!isspace(c))
		{
			if (result_string)
			{
				*result_string = c;
				result_string++;
			}
		}


		if (c == '~')// ���� ���p����� ������ - ����������(��������������) , ��
		{
			
		}
		else if (isalpha(c) && !isalpha(prev_c))// ���� ���p����� ������ - ����������(��������������) , ��
		{
			if (prev_c == '~')
			{
				Push_stack_trees(&TREES, NewNodeByCh(prev_c, 0));
				struct Node* top = Top_stack_trees(&TREES);
				top->l = NewNodeByCh(c, 0);
			}
			else
			{
				Push_stack_trees(&TREES, NewNodeByCh(c, 0));// ������� ���� ������ � ������ ��� � ���� ��������
			}

			int i;
			for (i = 0; i < count_var; ++i)
			{
				if (*(variables + i) == c)
						break;
			}
			if (i == count_var)
			{
				count_var++;
				variables = (char*)realloc(variables, sizeof(char)*count_var);
				*(variables + count_var - 1) = c;
			}
		}
		else if (isdigit(c) && !isdigit(prev_c))// ���� ���p����� ������ - ����������(��������������) , ��
		{
			if (c - '0' < 0 || c - '0' > 1)
			{
#ifdef DEBUG
				printf("\nError! Invalid constant value %c\n", c);
#endif
				if (error)
				{
					strcpy(error, "Invalid constant value!");
				}
				ERROR(&OPERS, &TREES);
				return 0;
			}
			if (prev_c == '~')
			{
				Push_stack_trees(&TREES, NewNodeByCh(prev_c, 0));
				struct Node* top = Top_stack_trees(&TREES);
				top->l = NewNodeByCh('#', c - '0');
			}
			else
			{
				Push_stack_trees(&TREES, NewNodeByCh('#', c - '0'));// ������� ���� ������ � ������ ��� � ���� ��������
			}
		}
		else if (close_bracket(c))// ���� ���p����� ������ - ')' 	
		{
			while (!open_bracket(Top_stack(&OPERS)))
				//����������� �� ����� �������� ��� ����� ���p���� �� ��������� ���p������� ������ ������� ��� ��� ������� 
			{
				struct Node* tree = NULL;
				tree = NewNodeByCh(Top_stack(&OPERS), 0);
				Pop_stack(&OPERS);

				SetLeft(tree, Top_stack_trees(&TREES));
				Pop_stack_trees(&TREES);

				SetRight(tree, Top_stack_trees(&TREES));
				Pop_stack_trees(&TREES);

				Push_stack_trees(&TREES, tree);
			}
			Pop_stack(&OPERS);// ������� �� ����� �������� ���� ���p������� ������ 
			if (Top_stack(&OPERS) == '~')
			{
				struct Node* tree = NULL;
				tree = NewNodeByCh(Top_stack(&OPERS), 0);
				Pop_stack(&OPERS);

				SetLeft(tree, Top_stack_trees(&TREES));
				Pop_stack_trees(&TREES);

				Push_stack_trees(&TREES, tree);
			}
		}
		else if (open_bracket(c))// ���� ���p����� ������ - '(' , �� ����������� � � ���� �������� 	
		{
			if (prev_c == '~')
			{
				Push_stack(&OPERS, prev_c);
			}
			Push_stack(&OPERS, c);
		}
		else if (operation(c))// ���� ������ - ���� ���p���� , ��: 
		{
			if ((isalpha(prev_c) || close_bracket(prev_c) || isdigit(prev_c)) && c != '~')// ���� �� ����� ���� ���������� ��� ), ��
			{
				if (Empty_stack(&OPERS))// ���� ���� �������� ���� ���������� � ���� ���p���� 
				{
					Push_stack(&OPERS, c);
				}
				else// ���� �� ����
				{
					if (PRIOR(Top_stack(&OPERS)) < PRIOR(c))// ���� �p��p���� ����������� ���p���� ������ 
						//�p��p����� ���p���� �� ��p���� ����� ��������
					{
						Push_stack(&OPERS, c);// ����������� ����������� ���p���� �� ���� ��������
					}
					else// ���� �p��p���� ������
					{
						while (!(Empty_stack(&OPERS)) && (PRIOR(Top_stack(&OPERS)) >= PRIOR(c)))
						{
							//����������� �� ����� �������� ��� ����� � ������� ��� p����� �p��p������ � ������� ��� ��� ������� 
							struct Node* tree = NULL;
							tree = NewNodeByCh(Top_stack(&OPERS), 0);
							Pop_stack(&OPERS);

							SetLeft(tree, Top_stack_trees(&TREES));
							Pop_stack_trees(&TREES);

							SetRight(tree, Top_stack_trees(&TREES));
							Pop_stack_trees(&TREES);

							Push_stack_trees(&TREES, tree);
						}
						Push_stack(&OPERS, c);// ���������� � ���� �������� �����������  ���p����
					}
				}
			}
			else//������
			{
#ifdef DEBUG
				printf("\nError! I can not find a digit before operator %c\n", c);
#endif
				if (error)
				{
					strcpy(error, "I can not find a digit before operator!");
				}
				ERROR(&OPERS, &TREES);
				return 0;
			}

		}
		else if (c != ' ')// ������ ������� ����� ������� �� �������������
		{
#ifdef DEBUG
			printf("\nError! Not expected symbol: %c\n", c);
#endif
			if (error)
			{
				strcpy(error, "Not expected symbol!");
			}
			ERROR(&OPERS, &TREES);
			return 0;
		}

		if (!isspace(c))// ��� ������� �� ���� ��� ���������� ��� ������� ������
		{
			prev_c = c;
		}

		c = *in_string++;// ��p���� � ���������� ������� ������� ��p���
	}
	//===============================================================================================================	

	// ����� p������p���� ����� ��p������
	while (!(Empty_stack(&OPERS)))//���������� ������� � ����� �������� �������� � ���� ������
	{
		struct Node* tree = NULL;
		tree = NewNodeByCh(Top_stack(&OPERS), 0);
		Pop_stack(&OPERS);

		SetLeft(tree, Top_stack_trees(&TREES));
		Pop_stack_trees(&TREES);

		SetRight(tree, Top_stack_trees(&TREES));
		Pop_stack_trees(&TREES);

		Push_stack_trees(&TREES, tree);
	}

	printf("\n%s\n\n", print_string);

	for (int i = 0; i < count_var; ++i)
	{
		printf("%c ", *(variables + i));
	}
	printf("\n");

#ifdef DEBUG
	PrintTree(Top_stack_trees(&TREES));
#endif
	
	SolveForAllData(Top_stack_trees(&TREES), variables, count_var, error);
	
	Destroy_stack(&OPERS);

	Destroy_stack_trees(&TREES);
	free(variables);

	return 1;
}
