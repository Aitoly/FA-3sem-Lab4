#pragma warning(disable:4996)

#include<stdio.h>
#include<iostream>
#include<ctype.h>
#include<math.h>
#include<time.h>

#define DATA_SIZE 26

typedef struct
{
	int* data;
	int size;
} Mass;

void Delete(Mass* m)
{
	free(m->data);
}

////////////////////

void swap(int* massiv, int i, int j)
{
	int t = massiv[i];
	massiv[i] = massiv[j];
	massiv[j] = t;
}

int compare_plus(const void* a, const void* b)
{
	return *((int*)a) - *((int*)b);
}

int compare_minus(const void* a, const void* b)
{
	return *((int*)b) - *((int*)a);
}

int Get_Info(FILE* in, char* line)
{
	if (!in)
	{
		return 0;
	}

	char c;
	int mode = 0;

	//0 - нет
	//-1 - однострочный коментарий
	//>0 - просто коментарий

	while (!feof(in))
	{
		c = fgetc(in);
		if (mode == 0)
		{
			if (c == '\n' || c == EOF)
			{
				continue;
			}

			if (c == ';')
			{
				*line++ = c;
				*line = '\0';
				return 1;
			}
			else if (c == '%')
			{
				mode = -1;
			}
			else if (c == '{')
			{
				mode++;
			}
			else if (c == '}')
			{
				*line = '\0';
				return 0;
			}
			else
			{
				if (!(isspace(c) && *(line - 1) == ','))
					*line++ = c;
			}
		}
		else if (mode == -1)
		{
			if (c == '\n')
			{
				mode = 0;
			}
		}
		else if (mode > 0)
		{
			if (c == '}')
			{
				mode = 0;
			}
		}
	}

	if (!mode)
	{
		return 1;
	}
	else
	{
		*line = '\0';
		return 0;
	}

}

////////////////////

int Load(char* ptr, char* line, int* INIT, Mass* DATA)
{
	//Load A, in.txt; - загрузить в массив A целые числа из файла in.txt;
	int index_in = toupper(*ptr) - 'A'; ptr += 2;
	char fileName[BUFSIZ];
	char* ptr_to_file_name = fileName;
	while (*ptr != ';')
	{
		*ptr_to_file_name++ = *ptr;
		ptr++;
	}
	*ptr_to_file_name = '\0';
	//printf("It a line to read from file %s in mass %c\n", fileName, 'A' + index_in);

	////////////////
	Delete(DATA + index_in);

	DATA[index_in].data = NULL;

	FILE* f;
	f = fopen(fileName, "r");
	if (!f)
	{
		return 0;
	}

	int t_mass[50];
	int size_t_mass = 0;

	while (!feof(f))
	{
		if (fscanf(f, "%d ", &t_mass[size_t_mass]) == 1)
		{
			size_t_mass++;
			if (size_t_mass == 50)
			{
				DATA[index_in].size += size_t_mass;
				DATA[index_in].data = (int*)realloc(DATA[index_in].data, DATA[index_in].size * sizeof(int));
				for (int i = 0; i < size_t_mass; i++)
				{
					DATA[index_in].data[i] = t_mass[i];
				}
			}
		}
	}

	if (size_t_mass > 0)
	{
		DATA[index_in].size += size_t_mass;
		DATA[index_in].data = (int*)realloc(DATA[index_in].data, DATA[index_in].size * sizeof(int));
		for (int i = 0; i < size_t_mass; i++)
		{
			DATA[index_in].data[i] = t_mass[i];
		}
	}

	fclose(f);

	INIT[index_in] = 1;
	return 1;
}

int Save(char* ptr, char* line, int* INIT, Mass* DATA)
{
	//Save A, in2.txt; - выгрузить элементы массива A в файл in2.txt;
	int index_out = toupper(*ptr) - 'A'; ptr += 2;
	char fileName[BUFSIZ];
	char* ptr_to_file_name = fileName;
	while (*ptr != ';')
	{
		*ptr_to_file_name++ = tolower(*ptr);
		ptr++;
	}
	*ptr_to_file_name = '\0';
	//printf("It a line to save in file %s from mass %c\n", fileName, 'A' + index_out);

	if (!INIT[index_out])
		return 0;
	////////////////

	FILE* f;
	f = fopen(fileName, "w");
	if (!f)
	{
		return 0;
	}

	int t_mass[50];
	int size_t_mass = 0;

	for (int i = 0; i < DATA[index_out].size; i++)
	{
		fprintf(f, "%d ", DATA[index_out].data[i]);
	}

	fclose(f);
	return 1;
}

int Concat(char* ptr, char* line, int* INIT, Mass* DATA)
{
	//Concat A, b; -объединить два массива A и B результат сохранить в массив A;
	int index_in = toupper(*ptr) - 'A'; ptr += 2;
	int index_out = toupper(*ptr) - 'A';

	//printf("It a line to concat mass %c in %c\n", 'A' + index_out, 'A' + index_in);

	if (!INIT[index_out] || !INIT[index_in])
		return 0;
	////////////////
	int newSize = DATA[index_in].size + DATA[index_out].size;
	DATA[index_in].data = (int*)realloc(DATA[index_in].data, newSize * sizeof(int));

	int i_write = DATA[index_in].size;
	for (int i = 0; i < DATA[index_out].size; i++, i_write++)
	{
		DATA[index_in].data[i_write] = DATA[index_out].data[i];
	}

	DATA[index_in].size = newSize;
	return 1;
}

int Free(char* ptr, char* line, int* INIT, Mass* DATA)
{
	//Free(a); - очистить массив А;
	int index_in = toupper(*ptr) - 'A';

	//printf("It a line to free mass %c \n", 'A' + index_in);

	if (!INIT[index_in])
		return 0;
	////////////////
	free(DATA[index_in].data);
	DATA[index_in].data = NULL;
	INIT[index_in] = 0;
}

int Rand(char* ptr, char* line, int* INIT, Mass* DATA)
{
	//Rand A, count, lb, rb; - заполнить массив A случайными элементами из отрезка [lb;rb] в количестве count штук;
	int index_in = toupper(*ptr) - 'A'; ptr += 2;
	int count = 0, l = 0, r = 0;
	while (isdigit(*ptr))
	{
		count *= 10;
		count += *ptr - '0';
		ptr++;
	}
	ptr++;
	while (isdigit(*ptr))
	{
		l *= 10;
		l += *ptr - '0';
		ptr++;
	}
	ptr++;
	while (isdigit(*ptr))
	{
		r *= 10;
		r += *ptr - '0';
		ptr++;
	}

	if (l > r)
		return 0;

	//printf("It a line to rand mass %c. New size %d. [%d,%d]\n", 'A' + index_in, count, l , r);
	////////////////
	free(DATA[index_in].data);
	DATA[index_in].data = NULL;

	DATA[index_in].data = (int*)malloc(count * sizeof(int));
	srand(time(NULL));
	for (int i = 0; i < count; i++)
	{
		DATA[index_in].data[i] = l + (rand() % (r - l));
	}

	DATA[index_in].size = count;
	INIT[index_in] = 1;

	return 1;
}

int Remove(char* ptr, char* line, int* INIT, Mass* DATA)
{
	//Remove a, 2, 7; - удалить из массива a 7 элементов, начиная с элемента с индексом 2;
	int index_in = toupper(*ptr) - 'A'; ptr += 2;
	int count = 0, start_i = 0;
	while (isdigit(*ptr))
	{
		start_i *= 10;
		start_i += *ptr - '0';
		ptr++;
	}
	ptr++;
	while (isdigit(*ptr))
	{
		count *= 10;
		count += *ptr - '0';
		ptr++;
	}

	if (!INIT[index_in])
		return 0;

	if (start_i > DATA[index_in].size)
		return 0;

	if (count > DATA[index_in].size - start_i)
		return 0;

	//printf("It a line to remove from mass %c %d symbols [start index: %d]\n", 'A' + index_in, count, start_i);
	////////////

	for (int i = start_i + count; i < DATA[index_in].size; i++, start_i++)
	{
		DATA[index_in].data[start_i] = DATA[index_in].data[i];
	}
	DATA[index_in].size -= count;
	DATA[index_in].data = (int*)realloc(DATA[index_in].data, DATA[index_in].size * sizeof(int));

	return 1;
}

int Copy(char* ptr, char* line, int* INIT, Mass* DATA)
{
	//Copy A, 4, 10, b; - скопировать из массива А элементы с 4 по 10 и сохранить их в b;
	int index_out = toupper(*ptr) - 'A'; ptr += 2;
	int from = 0, to = 0;
	while (isdigit(*ptr))
	{
		from *= 10;
		from += *ptr - '0';
		ptr++;
	}
	ptr++;
	while (isdigit(*ptr))
	{
		to *= 10;
		to += *ptr - '0';
		ptr++;
	}
	ptr++;
	int index_in = toupper(*ptr) - 'A'; ptr += 2;

	if (from > to)
		return 0;

	if (!INIT[index_out])
		return 0;

	if (from > DATA[index_out].size)
		return 0;

	if (to > DATA[index_out].size - 1)
		return 0;

	int new_size_in = to - from + 1;
	//printf("It a line to copy from mass %c to %c. [symbols from %d to %d (%d)]\n", 'A' + index_out, 'A' + index_in, from, to, new_size_in);
	/////////

	if (index_out == index_in)
	{
		int* timeMass = (int*)malloc(new_size_in * sizeof(int));

		for (int i = 0; i < new_size_in; i++)
		{
			timeMass[i] = DATA[index_out].data[from++];
		}

		free(DATA[index_in].data);
		DATA[index_in].data = (int*)malloc(new_size_in * sizeof(int));

		for (int i = 0; i < new_size_in; i++)
		{
			DATA[index_in].data[i] = timeMass[i];
		}

		DATA[index_in].size = new_size_in;
		INIT[index_in] = 1;

		free(timeMass);
	}
	else
	{

		free(DATA[index_in].data);
		DATA[index_in].data = (int*)malloc(new_size_in * sizeof(int));

		for (int i = 0; i < new_size_in; i++)
		{
			DATA[index_in].data[i] = DATA[index_out].data[from++];
		}

		DATA[index_in].size = new_size_in;
		INIT[index_in] = 1;
	}
	return 1;
}

int Sort(char* ptr, char* line, int* INIT, Mass* DATA)
{
	//Sort A+(-); - сортировать элементы массива A по возрастанию (убыванию);
	int index_in = toupper(*ptr) - 'A'; ptr++;
	char sing = *ptr;

	if (!INIT[index_in])
		return 0;

	//printf("It a line to sort mass %c in mode %c\n", 'A' + index_in, sing);
	////////////////

	if (sing == '+')
	{
		qsort(DATA[index_in].data, DATA[index_in].size, sizeof(int), compare_plus);
	}
	else if (sing == '-')
	{
		qsort(DATA[index_in].data, DATA[index_in].size, sizeof(int), compare_minus);
	}
	else
		return 0;

	return 1;
}

int Permute(char* ptr, char* line, int* INIT, Mass* DATA)
{
	//Permute A; - переставить элементы массива в случайном порядке;
	int index_in = toupper(*ptr) - 'A'; ptr += 2;

	if (!INIT[index_in])
		return 0;

	//printf("It a line to permute mass %c\n", 'A' + index_in);
	////////////////

	srand(time(NULL));
	int in_i = 0;
	for (int i = 0; i < DATA[index_in].size; i++)
	{
		in_i = rand() % DATA[index_in].size;
		swap(DATA[index_in].data, i, in_i);
	}

	return 1;
}

int Stat(char* ptr, char* line, int* INIT, Mass* DATA)
{
	// Stat a; - вывести статистическую информацию о массиве а: размер массива, максимальный и минимальный элемент (и их индексы), 
	//наиболее часто встречающийся элемент, среднее значение элементов, максимальное отклонение элементов от среднего значения;

	int index_in = toupper(*ptr) - 'A'; ptr += 2;

	if (!INIT[index_in])
		return 0;

	printf("Stat massiv %c:\n", 'A' + index_in);
	////////////////

	printf("\tLen: %d\n", DATA[index_in].size);

	if (DATA[index_in].size)
	{
		int max = 0, min = 0, CommonSize = 0, isFind = 0;
		int* MostCommonData = NULL;
		int* MostCommonCount = NULL;
		double mean = 0;

		for (int i = 1; i < DATA[index_in].size; i++)
		{
			mean += DATA[index_in].data[i];

			if (DATA[index_in].data[i] > DATA[index_in].data[max])
				max = i;
			if (DATA[index_in].data[i] < DATA[index_in].data[min])
				min = i;

			isFind = 0;
			for (int j = 0; j < CommonSize; j++)
			{
				if (MostCommonData[j] == DATA[index_in].data[i])
				{
					MostCommonCount[j]++;
					isFind = 1;
				}
			}

			if (!isFind)
			{
				CommonSize++;
				MostCommonData = (int*)realloc(MostCommonData, CommonSize * sizeof(int));
				MostCommonCount = (int*)realloc(MostCommonCount, CommonSize * sizeof(int));

				MostCommonData[CommonSize - 1] = DATA[index_in].data[i];
				MostCommonCount[CommonSize - 1] = 1;
			}
		}

		isFind = 0;
		for (int j = 0; j < CommonSize; j++)
		{
			if (MostCommonCount[j] > MostCommonCount[isFind])
				isFind = j;
		}

		mean /= (double)DATA[index_in].size;

		double MaxDeviationFromMean = abs(mean - DATA[index_in].data[0]);
		for (int i = 1; i < DATA[index_in].size; i++)
		{
			if (abs(mean - DATA[index_in].data[i]) > MaxDeviationFromMean)
				MaxDeviationFromMean = abs(mean - DATA[index_in].data[i]);
		}

		printf("\tMax: %d[%d]\n", DATA[index_in].data[max], max);
		printf("\tMin: %d[%d]\n", DATA[index_in].data[min], min);

		printf("\tMost common: %d\n", MostCommonData[isFind]);

		printf("\tAverage value: %f\n", mean);

		printf("\tMax deviation from the mean: %f\n", MaxDeviationFromMean);
	}

	return 1;
}

int PrintAll(char* ptr, char* line, int* INIT, Mass* DATA)
{
	//Print a, all; - вывести на экран все элементы массива.
	ptr = line + 6;

	int index_out = toupper(*ptr) - 'A'; ptr += 2;

	if (!INIT[index_out])
		return 0;

	printf("PrintAll massiv %c: ", 'A' + index_out);
	////////////////

	for (int i = 0; i < DATA[index_out].size; i++)
	{
		printf("%d ", DATA[index_out].data[i]);
	}
	printf("\n");

	return 1;
}

int Print(char* ptr, char* line, int* INIT, Mass* DATA)
{
	int index_out = toupper(*ptr) - 'A'; ptr += 2;
	int from = 0, to = 0;
	while (isdigit(*ptr))
	{
		from *= 10;
		from += *ptr - '0';
		ptr++;
	}
	ptr++;
	while (isdigit(*ptr))
	{
		to *= 10;
		to += *ptr - '0';
		ptr++;
	}
	ptr++;
	int index_in = toupper(*ptr) - 'A'; ptr += 2;

	if (from > to)
		return 0;

	if (!INIT[index_out])
		return 0;

	if (from > DATA[index_out].size)
		return 0;

	if (to > DATA[index_out].size - 1)
		return 0;

	printf("Print massiv %c[%d,%d]: ", 'A' + index_out, from, to);
	////////////////

	for (int i = from; i <= to; i++)
	{
		printf("%d ", DATA[index_out].data[i]);
	}
	printf("\n");

	return 1;
}

int main()
{
	FILE* in;
	in = fopen("instuctions.txt", "r");
	if (!in)
	{
		printf("I can`t open input file");
		system("pause");
		exit(-1);
	}

	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	int INIT[DATA_SIZE] = { 0 };
	Mass* DATA = (Mass*)malloc(sizeof(Mass) * DATA_SIZE);
	for (int i = 0; i < DATA_SIZE; i++)
	{
		DATA[i].data = NULL;
		DATA[i].size = 0;
	}

	char line[BUFSIZ];
	char* ptr;

	while (!feof(in))
	{
		line[0] = 0;
		if (!Get_Info(in, line))
		{
			printf("It is an error in line \"%s\" (len = %d)\n", line, strlen(line));
			fclose(in);
			system("pause");
			exit(-1);
		}
		if (strlen(line) == 0)
		{
			continue;
		}

		//printf("Line : \"%s\" (len = %d)\n", line, strlen(line));

		/////////////////////////////////////////// Load
		ptr = line;
		if (toupper(*ptr++) == 'L' && toupper(*ptr++) == 'O' && toupper(*ptr++) == 'A' && toupper(*ptr++) == 'D' && *ptr++ == ' ')
		{
			if (!Load(ptr, line, INIT, DATA))
			{
				printf("It is an error in line \"%s\" (len = %d)\n", line, strlen(line));
				fclose(in);
				system("pause");
				exit(-1);
			}
			continue;
		}
		/////////////////////////////////////////// Save
		ptr = line;
		if (toupper(*ptr++) == 'S' && toupper(*ptr++) == 'A' && toupper(*ptr++) == 'V' && toupper(*ptr++) == 'E' && *ptr++ == ' ')
		{
			if (!Save(ptr, line, INIT, DATA))
			{
				printf("It is an error in line \"%s\" (len = %d)\n", line, strlen(line));
				fclose(in);
				system("pause");
				exit(-1);
			}
			continue;
		}
		/////////////////////////////////////////// Rand
		ptr = line;
		if (toupper(*ptr++) == 'R' && toupper(*ptr++) == 'A' && toupper(*ptr++) == 'N' && toupper(*ptr++) == 'D' && *ptr++ == ' ')
		{
			Rand(ptr, line, INIT, DATA);
			continue;
		}
		/////////////////////////////////////////// Concat
		ptr = line;
		if (toupper(*ptr++) == 'C' && toupper(*ptr++) == 'O' && toupper(*ptr++) == 'N' && toupper(*ptr++) == 'C' && toupper(*ptr++) == 'A' && toupper(*ptr++) == 'T' && *ptr++ == ' ')
		{
			if (!Concat(ptr, line, INIT, DATA))
			{
				printf("It is an error in line \"%s\" (len = %d)\n", line, strlen(line));
				fclose(in);
				system("pause");
				exit(-1);
			}
			continue;
		}
		/////////////////////////////////////////// Free()
		ptr = line;
		if (toupper(*ptr++) == 'F' && toupper(*ptr++) == 'R' && toupper(*ptr++) == 'E' && toupper(*ptr++) == 'E' && toupper(*ptr++) == '(')
		{
			if (!Free(ptr, line, INIT, DATA))
			{
				printf("It is an error in line \"%s\" (len = %d)\n", line, strlen(line));
				fclose(in);
				system("pause");
				exit(-1);
			}
			continue;
		}
		/////////////////////////////////////////// Remove
		ptr = line;
		if (toupper(*ptr++) == 'R' && toupper(*ptr++) == 'E' && toupper(*ptr++) == 'M' && toupper(*ptr++) == 'O' && toupper(*ptr++) == 'V' && toupper(*ptr++) == 'E' && *ptr++ == ' ')
		{
			if (!Remove(ptr, line, INIT, DATA))
			{
				printf("It is an error in line \"%s\" (len = %d)\n", line, strlen(line));
				fclose(in);
				system("pause");
				exit(-1);
			}
			continue;
		}
		/////////////////////////////////////////// Copy
		ptr = line;
		if (toupper(*ptr++) == 'C' && toupper(*ptr++) == 'O' && toupper(*ptr++) == 'P' && toupper(*ptr++) == 'Y' && *ptr++ == ' ')
		{
			if (!Copy(ptr, line, INIT, DATA))
			{
				printf("It is an error in line \"%s\" (len = %d)\n", line, strlen(line));
				fclose(in);
				system("pause");
				exit(-1);
			}
			continue;
		}
		/////////////////////////////////////////// Sort
		ptr = line;
		if (toupper(*ptr++) == 'S' && toupper(*ptr++) == 'O' && toupper(*ptr++) == 'R' && toupper(*ptr++) == 'T' && *ptr++ == ' ')
		{
			if (!Sort(ptr, line, INIT, DATA))
			{
				printf("It is an error in line \"%s\" (len = %d)\n", line, strlen(line));
				fclose(in);
				system("pause");
				exit(-1);
			}
			continue;
		}
		/////////////////////////////////////////// Permute
		ptr = line;
		if (toupper(*ptr++) == 'P' && toupper(*ptr++) == 'E' && toupper(*ptr++) == 'R' && toupper(*ptr++) == 'M' && toupper(*ptr++) == 'U' && toupper(*ptr++) == 'T' && toupper(*ptr++) == 'E' && *ptr++ == ' ')
		{
			if (!Permute(ptr, line, INIT, DATA))
			{
				printf("It is an error in line \"%s\" (len = %d)\n", line, strlen(line));
				fclose(in);
				system("pause");
				exit(-1);
			}
			continue;
		}
		/////////////////////////////////////////// Stat
		ptr = line;
		if (toupper(*ptr++) == 'S' && toupper(*ptr++) == 'T' && toupper(*ptr++) == 'A' && toupper(*ptr++) == 'T' && *ptr++ == ' ')
		{
			if (!Stat(ptr, line, INIT, DATA))
			{
				printf("It is an error in line \"%s\" (len = %d)\n", line, strlen(line));
				fclose(in);
				system("pause");
				exit(-1);
			}
			continue;
		}
		/////////////////////////////////////////// PrintAll
		ptr = line;
		if (toupper(*ptr++) == 'P' && toupper(*ptr++) == 'R' && toupper(*ptr++) == 'I' && toupper(*ptr++) == 'N' && toupper(*ptr++) == 'T' && *ptr++ == ' '
			&& isalpha(*ptr++) && *ptr++ == ',' && toupper(*ptr++) == 'A' && toupper(*ptr++) == 'L' && toupper(*ptr++) == 'L' && *ptr++ == ';')
		{
			if (!PrintAll(ptr, line, INIT, DATA))
			{
				printf("It is an error in line \"%s\" (len = %d)\n", line, strlen(line));
				fclose(in);
				system("pause");
				exit(-1);
			}
			continue;
		}
		/////////////////////////////////////////// Print
		ptr = line;
		if (toupper(*ptr++) == 'P' && toupper(*ptr++) == 'R' && toupper(*ptr++) == 'I' && toupper(*ptr++) == 'N' && toupper(*ptr++) == 'T' && *ptr++ == ' ')
		{
			if (!Print(ptr, line, INIT, DATA))
			{
				printf("It is an error in line \"%s\" (len = %d)\n", line, strlen(line));
				fclose(in);
				system("pause");
				exit(-1);
			}
			continue;
		}
		///////////////////////////////////////////


		printf("It is an error in line \"%s\" (len = %d)\n", line, strlen(line));
		fclose(in);
		system("pause");
		exit(-1);
	}

	fclose(in);

	for (int i = 0; i < DATA_SIZE; i++)
		Delete(DATA + i);
	free(DATA);

	system("pause");
	return 0;
}
