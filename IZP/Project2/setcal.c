/*
		2.Projekt: Prace s datovymi strukturami
				Worked: 
		Marek Kozumplík, xkozum08
		Jakub Valeš, xvales04
		Václav Zelinka, xzelin27
		Dias Assatulla, xassat00
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MIN_ARGC 2
#define MAX_ARGC 3
#define MAX_WORD 30

// Data type for set:
typedef struct set
{
	size_t count;
	char **elements;
	// row parameter holds the index of row, where the set/relace is located in file - helpful with commands
	int row;
} setT;

// Array of struct type setT
typedef struct setArr
{
	size_t count;
	struct set **sElem;
} setArrT;

// Data type for tuple:
typedef struct tuple
{
	char *x;
	char *y;
} tupleT;

// Data type for relace
typedef struct relace
{
	size_t count;
	struct tuple **tuples;
	int row;
} relaceT;
// Array of struct type relaceT
typedef struct relaceArr
{
	size_t count;
	struct relace **rElem;
} relaceArrT;
// Data type fo commands
typedef struct command
{
	char **function;
	size_t count;
} commandT;
// Array of struct type commandT
typedef struct commandArr
{
	struct command **functions;
	size_t count;
} commandArrT;

enum OutCode
{
	SUCCESS,
	NO_ARGG,
	ARGS_MANY,
	FILE_CRASH,
	MEM_ERROR,
	UNKNOWN_COMMAND,
	ELEMENTS_ERROR
};

// Constructor and intializate functions: Constractors allocates memory for the array of structs and initilizate functions allocates memory for the structs and sets their base values
setArrT *setArray_Const()
{
	setArrT *S_array = NULL;
	S_array = malloc(sizeof(*S_array));
	S_array->sElem = malloc(sizeof(char *));
	S_array->count = 0;
	return S_array;
}

setT *setInitialize()
{
	setT *S = malloc(sizeof(*S));
	S->elements = malloc(sizeof(char *));
	S->count = 0;
	return S;
}

tupleT *tupleCtor()
{
	tupleT *T = malloc(sizeof(*T));
	return T;
}

relaceArrT *relaceArray_Const()
{
	relaceArrT *R_array = NULL;
	R_array = malloc(sizeof(*R_array));
	R_array->rElem = malloc(sizeof(relaceT *));
	R_array->count = 0;
	return R_array;
}

relaceT *relaceInitialize()
{
	relaceT *R = malloc(sizeof(*R));
	R->tuples = malloc(sizeof(tupleT *));
	R->count = 0;
	return R;
}

commandArrT *commandArray_Const()
{
	commandArrT *C_array = NULL;
	C_array = malloc(sizeof(*C_array));
	C_array->functions = malloc(sizeof(commandT));
	C_array->count = 0;
	return C_array;
}

commandT *commandInitialize()
{
	commandT *C = malloc(sizeof(*C));
	C->function = malloc(sizeof(char *));
	C->count = 0;
	return C;
}

// Add functions reallocates memory for the array of structs so we can store in the sturct more than one of them
int setArrayAdd(setArrT *S_array, setT *S)
{
	S_array->sElem = realloc(S_array->sElem, sizeof(setT *) * (S_array->count + 1));
	if (S_array->sElem == NULL)
	{
		fprintf(stderr, "\nERROR: Can not re-allocate memory.\n");
		return MEM_ERROR;
	}
	S_array->sElem[S_array->count] = S;
	S_array->count++;
	return SUCCESS;
}

int relaceArrayAdd(relaceArrT *R_array, relaceT *R)
{
	R_array->rElem = realloc(R_array->rElem, sizeof(relaceT *) * (R_array->count + 1));
	if (R_array->rElem == NULL)
	{
		fprintf(stderr, "\nERROR: Can not re-allocate memory.\n");
		return MEM_ERROR;
	}
	R_array->rElem[R_array->count] = R;
	R_array->count++;
	return SUCCESS;
}

int commandArrayAdd(commandArrT *C_array, commandT *C)
{
	C_array->functions = realloc(C_array->functions, sizeof(commandT *) * (C_array->count + 1));
	if (C_array->functions == NULL)
	{
		fprintf(stderr, "\nERROR: Can not re-allocate memory.\n");
		return MEM_ERROR;
	}
	C_array->functions[C_array->count] = C;
	C_array->count++;
	return SUCCESS;
}

// Destructor functions frees memory witch was previously allocated and sets the base values bact to 0
void setDestructor(setT *S)
{
	if (S != NULL)
	{
		if (S->elements != NULL)
		{
			// We need to free every array from the array of structs
			for (unsigned int i = 0; i < S->count; i++)
				free(S->elements[i]);
		}
		free(S->elements);
		S->count = 0;
		S->row = 0;
		free(S);
	}
}

void setArrayDest(setArrT *S_array)
{
	for (unsigned int i = 0; i < S_array->count; i++)
	{
		setDestructor(S_array->sElem[i]);
	}
	free(S_array->sElem);
	free(S_array);
	S_array = NULL;
}

void tupleDestructor(tupleT *T)
{
	if (T != NULL)
	{
		free(T);
	}
}

void relaceDestructor(relaceT *R)
{
	if (R != NULL)
	{
		if (R->tuples != NULL)
		{
			for (unsigned int i = 0; i < R->count; i++)
			{
				free(R->tuples[i]->x);
				free(R->tuples[i]->y);
				tupleDestructor(R->tuples[i]);
			}
		}
		free(R->tuples);
		R->count = 0;
		R->row = 0;
		free(R);
	}
}

void relaceArrayDest(relaceArrT *R_array)
{
	for (unsigned int i = 0; i < R_array->count; i++)
	{
		relaceDestructor(R_array->rElem[i]);
	}
	free(R_array->rElem);
	free(R_array);
	R_array = NULL;
}

void commandDestructor(commandT *C)
{
	if (C != NULL)
	{
		if (C->function != NULL)
		{
			for (unsigned int i = 0; i < C->count; i++)
				free(C->function[i]);
		}
		free(C->function);
		C->count = 0;
		free(C);
	}
}

void commandArrayDest(commandArrT *C_array)
{
	if (C_array != NULL)
	{
		for (unsigned int i = 0; i < C_array->count; i++)
		{
			commandDestructor(C_array->functions[i]);
		}
		free(C_array->functions);
		free(C_array);
		C_array = NULL;
	}
}

// Error Handling function
int errorHandling(int argc, FILE *fp)
{
	if (argc < MIN_ARGC)
	{
		fprintf(stderr, "\nERROR: Not enough arguments.\n");
		return NO_ARGG;
	}
	if (argc >= MAX_ARGC)
	{
		fprintf(stderr, "\nERROR: Too many arguments.\n");
		return ARGS_MANY;
	}
	if (fp == NULL)
	{
		fprintf(stderr, "\nERROR: Can not open the file.\n");
		return FILE_CRASH;
	}
	return SUCCESS;
}

// After initializing all pointer, this function checks, if they all are not NULLptr
int checkElements(setT *U, setArrT *S_array, relaceArrT *R_array, commandArrT *C_array)
{
	if (S_array == NULL || U == NULL || U->elements == NULL || R_array == NULL || R_array->rElem == NULL || S_array->sElem == NULL || C_array == NULL || C_array->functions == NULL)
		return MEM_ERROR;
	else
		return SUCCESS;
}

// This function added in order not to write everytime a function, that destroy 1 pointer
void destroyAllElements(setT *U, setArrT *S_array, relaceArrT *R_array, commandArrT *C_array)
{
	setDestructor(U);
	setArrayDest(S_array);
	relaceArrayDest(R_array);
	commandArrayDest(C_array);
}

// Next two functions are checking if every element in set/relace is from univerzum, returns error if thats not the case
int isSetInUniv(setArrT *S_array, setT *U)
{
	bool isPart;
	for (unsigned int i = 0; i < S_array->count; i++)
	{
		for (unsigned int j = 0; j < S_array->sElem[i]->count; j++)
		{
			isPart = false;
			for (unsigned int k = 0; k < U->count; k++)
			{
				if (strcmp(S_array->sElem[i]->elements[j], U->elements[k]) == 0)
				{
					isPart = true;
					break;
				}
			}
			if (!isPart)
			{
				fprintf(stderr, "\nERROR: Elemets in set arent from univerzum\n");
				return ELEMENTS_ERROR;
			}
		}
	}
	return SUCCESS;
}

int isRelaceInUniv(relaceArrT *R_array, setT *U)
{
	bool isPartX;
	bool isPartY;
	for (unsigned int i = 0; i < R_array->count; i++)
	{
		for (unsigned int j = 0; j < R_array->rElem[i]->count; j++)
		{
			isPartX = false;
			isPartY = false;
			for (unsigned int k = 0; k < U->count; k++)
			{
				if (!isPartX)
				{
					if (strcmp(R_array->rElem[i]->tuples[j]->x, U->elements[k]) == 0)
						isPartX = true;
				}

				if (!isPartY)
				{
					if (strcmp(R_array->rElem[i]->tuples[j]->y, U->elements[k]) == 0)
						isPartY = true;
				}
			}
			if (!isPartX || !isPartY)
			{
				fprintf(stderr, "\nERROR: Elemets in relace arent from univerzum\n");
				return ELEMENTS_ERROR;
			}
		}
	}
	return SUCCESS;
}

// LOADING DATA FROM FILE FUNCTIONS:
// I used function strtok, which contain a token. But when cycle is end, it free's those tokens
// So I use this function, to topy data from token to new pointer, and then use it to assign for elements in structure setT or relaceT
char *safecopy(char *token)
{
	char *word;
	word = malloc(sizeof(char) * (strlen(token) + 1)); // + 1 because '\0'
	if (word != NULL)
	{
		strcpy(word, token);
	}
	return word;
}
// Add token to elements of structure setT
int addDataToset(char *token, setT *X)
{
	X->elements = realloc(X->elements, ++(X->count) * sizeof(char *));
	if (X->elements == NULL)
	{
		fprintf(stderr, "\nERROR: Can not re-allocate memory.\n");
		return MEM_ERROR;
	}
	// Every line has at the end '\n', so I decided to remove last symbol
	if (token[strlen(token) - 1] == '\n')
	{
		token[strlen(token) - 1] = '\0'; // Removes \n
	}

	X->elements[X->count - 1] = safecopy(token);
	return SUCCESS;
}

// Add token to elements of structure relaceT
// in that function indexForXY chooses whether the element falls in x or in y
int addDataToRelace(char *token, relaceT *X, int *indexForXY)
{
	if (*indexForXY % 2 == 0)
	{
		X->tuples = realloc(X->tuples, ++(X->count) * sizeof(tupleT *));
		if (X->tuples == NULL)
		{
			fprintf(stderr, "\nERROR: Can not re-allocate memory.\n");
			return MEM_ERROR;
		}
		X->tuples[X->count - 1] = tupleCtor();
	}
	if (*indexForXY % 2 == 0)
	{
		token++; //this removes the ( bracket
		X->tuples[X->count - 1]->x = safecopy(token);
	}
	else
	{

		if (token[strlen(token) - 1] == '\n')
		{ // This function need at the end, when for example:
			// the last Relace member -> (dad mother)
			// Mother -> 'm' 'o' 't' 'h' 'e' 'r' ')' '\n' => So it need to delete this '\n'
			token[strlen(token) - 1] = '\0';
		}

		token[strlen(token) - 1] = '\0'; // this removes the ) bracket

		X->tuples[X->count - 1]->y = safecopy(token);
	}
	return SUCCESS;
}

// Add token to elements of structure commandT
int addDataToCommand(char *token, commandT *C)
{
	C->function = realloc(C->function, ++(C->count) * sizeof(char *));
	if (C->function == NULL)
	{
		fprintf(stderr, "\nERROR: Can not re-allocate memory.\n");
		return MEM_ERROR;
	}
	if (token[strlen(token) - 1] == '\n')
		token[strlen(token) - 1] = '\0';
	C->function[C->count - 1] = safecopy(token);
	return SUCCESS;
}

// The get fuctions gets data from line to structs (setT or RelaceT or CommandT) by splitting them for tokens
int getDataForset(char *line, setT *X, int row)
{
	const char space[] = " ";
	char *token;
	token = strtok(line, space);
	while ((token = strtok(NULL, space)) != NULL)
	{
		if (addDataToset(token, X) != SUCCESS)
			return MEM_ERROR;
	}
	X->row = row;
	return SUCCESS;
}

int getDataForRelace(char *line, relaceT *X, int row)
{
	const char space[] = " ";
	char *token;
	token = strtok(line, space);
	int indexForXY = 0; // This variable will depend, will the word be in x or y
	while ((token = strtok(NULL, space)) != NULL)
	{

		if (addDataToRelace(token, X, &indexForXY) != SUCCESS)
			return MEM_ERROR;
		indexForXY++;
	}
	X->row = row;
	return SUCCESS;
}

int getDataForCommand(char *line, commandT *C)
{
	const char space[] = " ";
	char *token;
	token = strtok(line, space);
	while ((token = strtok(NULL, space)) != NULL)
	{
		if (addDataToCommand(token, C) != SUCCESS)
		{
			return MEM_ERROR;
		}
	}
	return SUCCESS;
}

// Functions to print sets and relace
void printEachSet(setT *S)
{
	for (unsigned int i = 0; i < S->count; i++)
	{
		printf(" %s", S->elements[i]);
	}
	printf("\n");
}

void printEachRel(relaceT *R)
{
	for (unsigned int i = 0; i < R->count; i++)
	{
		printf(" (%s %s)", R->tuples[i]->x, R->tuples[i]->y);
	}
	printf("\n");
}

// Function checkы the first character on line and depending on him, it calls other function to work with the data on line
int getDataFromLine(char *line, setT *U, setArrT *S_array, relaceArrT *R_array, commandArrT *C_array, int row)
{
	if (line[0] == 'U')
	{
		printf("U");
		if (getDataForset(line, U, row) != SUCCESS)
		{
			return MEM_ERROR;
		}
		printEachSet(U);
	}
	else if (line[0] == 'S')
	{
		setT *S = setInitialize();
		printf("S");
		if (getDataForset(line, S, row) != SUCCESS)
		{
			setDestructor(S);
			return MEM_ERROR;
		}
		if (setArrayAdd(S_array, S) != SUCCESS)
		{
			setDestructor(S);
			return MEM_ERROR;
		}
		printEachSet(S);
	}
	else if (line[0] == 'R')
	{
		relaceT *R = relaceInitialize();
		printf("R");
		if (getDataForRelace(line, R, row) != SUCCESS)
		{
			relaceDestructor(R);
			return MEM_ERROR;
		}
		if (relaceArrayAdd(R_array, R) != SUCCESS)
		{
			relaceDestructor(R);
			return MEM_ERROR;
		}
		printEachRel(R);
	}
	else if (line[0] == 'C')
	{
		commandT *C = commandInitialize();
		if (getDataForCommand(line, C) != SUCCESS)
		{
			commandDestructor(C);
			return MEM_ERROR;
		}
		if (commandArrayAdd(C_array, C) != SUCCESS)
		{
			commandDestructor(C);
			return MEM_ERROR;
		}
	}
	else
		return FILE_CRASH;

	return SUCCESS;
}

//Function to load data from file - it reads file row by row and dynamically allocates memory for each line -
//then it calls other function to work with these lines
int getDataFromFile(FILE *fp, setT *U, setArrT *S_array, relaceArrT *R_array, commandArrT *C_array)
{
	int row = 1;
	char buffer[30];
	size_t len = sizeof(buffer);
	char *line = malloc(len);
	if (line == NULL)
	{
		fprintf(stderr, "\nERROR: Unable to allocate memory of the line.\n");
		return MEM_ERROR;
	}
	line[0] = '\0';
	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		size_t len_used = strlen(line);
		size_t buffer_used = strlen(buffer);
		if (len - len_used < buffer_used)
		{
			len *= 2;
			if ((line = realloc(line, len)) == NULL)
			{
				fprintf(stderr, "\nERROR: Unable to reallocate memory of the line.\n");
				free(line);
				return MEM_ERROR;
			}
		}
		strncpy(line + len_used, buffer, len - len_used);
		len_used += buffer_used;
		if (line[len_used - 1] == '\n')
		{
			if (row > 1000)
			{
				fprintf(stderr, "ERORR: Too many rows");
				return FILE_CRASH;
			}
			if (getDataFromLine(line, U, S_array, R_array, C_array, row) != SUCCESS)
			{
				line[0] = '\0';
				free(line);
				return FILE_CRASH;
			}
			row++;
			line[0] = '\0';
		}
	}
	free(line);
	return SUCCESS;
}

// WORK WITH COMMANDS FUNCTIONS:
// Functions for work with sets
//Returns set from setArrT with selected row
struct set *getSetFromSetArr(setArrT *S_array, int row)
{
	struct set *set = NULL;
	for (size_t i = 0; i < S_array->count; i++)
	{
		if (S_array->sElem[i]->row == row)
		{
			set = S_array->sElem[i];
			break;
		}
	}
	return set;
}

// There are the other functions for work with sets u can redo them however you want, its again only to check if it works
int emptySet(setArrT *S_array, int idx, setT *U)
{

	setT *set;
	if (idx == 1)
	{
		set = U;
	}
	else
		set = S_array->sElem[idx - 2];

	if (set == NULL)
	{
		fprintf(stderr, "set with row %d not found\n", idx);
		return ELEMENTS_ERROR;
	}
	if ((set->count == 0))
	{
		printf("true\n");
	}
	else
	{
		printf("false\n");
	}
	return SUCCESS;
}

int cardSet(setArrT *S_array, int idx, setT *U)
{
	setT *set;
	if (idx == 1)
	{
		set = U;
	}
	else
		set = S_array->sElem[idx - 2];
	if (set == NULL)
	{
		fprintf(stderr, "set with row %d not found\n", idx);
		return ELEMENTS_ERROR;
	}

	printf("%ld\n", set->count);
	return SUCCESS;
}
// we make complement of set A which means we print items which are in universe but not in set A
int complementSet(setArrT *S_array, setT *U, int idx)
{
	//we check if that set is not empty
	setT *set;
	if (idx == 1)
	{
		set = U;
	}
	else
		set = S_array->sElem[idx - 2];
	//if that set is empty we print out error
	if (set == NULL)
	{
		fprintf(stderr, "set with row %d not found\n", idx);
		return ELEMENTS_ERROR;
	}
	//we check which are in Universe but not in A
	printf("S");
	for (unsigned long int i = 0; i < U->count; i++)
	{
		for (unsigned long int j = 0; j < set->count; j++)
		{
			if ((strcmp(set->elements[j], U->elements[i]) == 0))
			{
				break;
			}

			else if ((strcmp(set->elements[j], U->elements[i]) != 0) && (j == set->count))
			{
				//we print items out
				printf("%s",U->elements[i]);
			}
		}
	}
	printf("\n");
	return SUCCESS;
}
//we print which items are in both sets A and B
int intersectSet(setArrT *S_array, int *idx)
{
	setT *a = getSetFromSetArr(S_array, idx[0]);
	setT *b = getSetFromSetArr(S_array, idx[1]);
	//we check if those sets are not empty, if yes we print out error
	if (a == NULL)
	{
		fprintf(stderr, "set with row %d not found\n", idx[0]);
		return ELEMENTS_ERROR;
	}
	if (b == NULL)
	{
		fprintf(stderr, "set with row %d not found\n", idx[1]);
		return ELEMENTS_ERROR;
	}
	//we check which are in both sets
	for (unsigned long int i = 0; i < a->count; i++)
	{
		for (unsigned long int j = 0; j < b->count; j++)
		{
			if ((strcmp(a->elements[i], b->elements[j]) == 0))
			{
				//and we print them out
				printf("%s ", a->elements[i]);
			}
		}
	}
	printf("\n");
	return SUCCESS;
}
//we print items which are in first set but not in second
int minusSet(setArrT *S_array, int *idx)
{
	setT *a = getSetFromSetArr(S_array, idx[1]);
	setT *b = getSetFromSetArr(S_array, idx[2]);
	//we find largest set
	unsigned long int x = 0;
	if (a->count > b->count)
	{
		x = a->count;
	}
	else
		x = b->count;
	//we went with error if set would be empty
	if (a == NULL)
	{
		fprintf(stderr, "set with row %d not found\n", idx[1]);
		return ELEMENTS_ERROR;
	}
	if (b == NULL)
	{
		fprintf(stderr, "set with row %d not found\n", idx[2]);
		return ELEMENTS_ERROR;
	}
	//we print those items
	for (unsigned long int i = 0; i < a->count; i++)
	{
		for (unsigned long int j = 0; j < b->count; j++)
		{
			if ((strcmp(a->elements[j], b->elements[i]) == 0) && (strcmp(a->elements[j], "") != 0))
			{
				break;
			}

			else if ((strcmp(a->elements[j], b->elements[i]) != 0) && (j == x))
			{
				printf("%s", a->elements[i]);
			}
		}
	}
	return SUCCESS;
}

int equalsSet(setArrT *S_array, int *idx)
{
	setT *a = getSetFromSetArr(S_array, idx[1]);
	setT *b = getSetFromSetArr(S_array, idx[2]);
	//we check if lenght is correct
	if (a->count != b->count)
	{
		return -1;
	}
	// we check if those sets are not empty
	if (a == NULL)
	{
		fprintf(stderr, "set with row %d not found\n", idx[1]);
		return ELEMENTS_ERROR;
	}
	if (b == NULL)
	{
		fprintf(stderr, "set with row %d not found\n", idx[2]);
		return ELEMENTS_ERROR;
	}
	//we check if items are same
	for (unsigned long int i = 0; i < a->count; i++)
	{
		if (a->elements[i] != b->elements[i])
		{
			return printf("non equal\n");
		}
	}
	printf("equal\n");
	return SUCCESS;
}

// Relace funcitons:

//Returns relation from relaceArrT with selected row
struct relace *getRelFromRelaceArr(relaceArrT *R_array, int row)
{
	struct relace *rel = NULL;
	for (size_t i = 0; i < R_array->count; i++)
	{
		if (R_array->rElem[i]->row == row)
		{
			rel = R_array->rElem[i];
			break;
		}
	}
	return rel;
}

//prints true if relation is reflexive. Prints false if not.
//returns error if there is no relation with row = idx in R_array
int reflexiveRel(relaceArrT *R_array, int idx, setT *U)
{
	relaceT *rel = getRelFromRelaceArr(R_array, idx);
	if (rel == NULL)
	{
		fprintf(stderr, "relace with row %d not found\n", idx);
		return ELEMENTS_ERROR;
	}
	for (size_t i = 0; i < U->count; i++)
	{
		bool found = false;
		for (size_t j = 0; j < rel->count; j++)
		{
			//for every a from U, (a,a) has to be in rel
			if (strcmp(U->elements[i], rel->tuples[j]->x) == 0 && strcmp(U->elements[i], rel->tuples[j]->y) == 0)
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			printf("false\n");
			return SUCCESS;
		}
	}
	printf("true\n");
	return SUCCESS;
}

//used for symmetric/antisymmetric relation because only 1 line was different
//symm == true - returns true if symmetric | symm == false - returns true if antisymmetric
bool getSymmetry(relaceT *rel, bool symm)
{
	for (size_t i = 0; i < rel->count; i++)
	{
		if (strcmp(rel->tuples[i]->x, rel->tuples[i]->y) == 0)
			continue;

		bool inverseEl = false;
		for (size_t j = 0; j < rel->count; j++) //finds inverse element for element[i]
		{
			if (strcmp(rel->tuples[i]->x, rel->tuples[j]->y) == 0 && strcmp(rel->tuples[i]->y, rel->tuples[j]->x) == 0)
			{
				inverseEl = true;
				break;
			}
		}
		if ((!inverseEl && symm) || (inverseEl && !symm))
		{
			return false;
		}
	}
	return true;
}

//prints true if relation is symmetric. Prints false if not.
//returns error if there is no relation with row = idx in R_array
int symmetricRel(relaceArrT *R_array, int idx)
{
	relaceT *rel = getRelFromRelaceArr(R_array, idx);
	if (rel == NULL)
	{
		fprintf(stderr, "relace with row %d not found\n", idx);
		return ELEMENTS_ERROR;
	}
	if (getSymmetry(rel, true))
	{
		printf("true\n");
	}
	else
	{
		printf("false\n");
	}
	return SUCCESS;
}

//prints true if relation is symmetric. Prints false if not.
//returns error if there is no relation with row = idx in R_array
int antisymmetricRel(relaceArrT *R_array, int idx)
{
	relaceT *rel = getRelFromRelaceArr(R_array, idx);
	if (rel == NULL)
	{
		fprintf(stderr, "relace with row %d not found\n", idx);
		return ELEMENTS_ERROR;
	}
	if (getSymmetry(rel, false))
	{
		printf("true\n");
	}
	else
	{
		printf("false\n");
	}
	return SUCCESS;
}

//return true if tuple (a, b) is in relation rel
//used in transitiveRel()
bool isInRelation(relaceT *rel, char *a, char *b)
{
	for (size_t i = 0; i < rel->count; i++)
	{
		if (strcmp(rel->tuples[i]->x, a) == 0 && strcmp(rel->tuples[i]->y, b) == 0)
		{
			return true;
		}
	}
	return false;
}

//prints true if relation is transitive. Prints false if not.
//returns error if there is no relation with row = idx in R_array
int transitiveRel(relaceArrT *R_array, int idx)
{
	relaceT *rel = getRelFromRelaceArr(R_array, idx);
	if (rel == NULL)
	{
		fprintf(stderr, "relace with row %d not found\n", idx);
		return ELEMENTS_ERROR;
	}
	for (size_t i = 0; i < rel->count; i++)
	{
		for (size_t j = 0; j < rel->count; j++)
		{
			if (strcmp(rel->tuples[i]->y, rel->tuples[j]->x) == 0) //if (a,b) and (b,c) in rel
			{
				if (!isInRelation(rel, rel->tuples[i]->x, rel->tuples[j]->y)) //(a,c) has to be in rel too
				{
					printf("false\n");
					return SUCCESS;
				}
			}
		}
	}
	printf("true\n");
	return SUCCESS;
}

//prints true if relation is function. Prints false if not.
//returns error if there is no relation with row = idx in R_array
int functionRel(relaceArrT *R_array, int idx)
{
	relaceT *rel = getRelFromRelaceArr(R_array, idx);
	if (rel == NULL)
	{
		fprintf(stderr, "relace with row %d not found\n", idx);
		return ELEMENTS_ERROR;
	}
	for (size_t i = 0; i < rel->count; i++)
	{
		for (size_t j = 1; j < rel->count; j++)
		{
			//if (a,b) and (a, c) in rel, then rel is not function
			if (strcmp(rel->tuples[i]->y, rel->tuples[j]->y) && !strcmp(rel->tuples[i]->x, rel->tuples[j]->x))
			{
				printf("false\n");
				return SUCCESS;
			}
		}
	}
	printf("true\n");
	return SUCCESS;
}

//prints domain/codomain of relation rel
//used in domainRel() and codomainRel()
void printDomain(relaceT *rel, setT *U, bool codomain)
{
	printf("S");
	for (size_t i = 0; i < U->count; i++)
	{
		for (size_t j = 0; j < rel->count; j++)
		{
			if (codomain)
			{
				if (strcmp(U->elements[i], rel->tuples[j]->y) == 0)
				{
					printf(" %s", U->elements[i]);
					break;
				}
			}
			else
			{
				if (strcmp(U->elements[i], rel->tuples[j]->x) == 0)
				{
					printf(" %s", U->elements[i]);
					break;
				}
			}
		}
	}
	printf("\n");
}

//prints domain of relation
//returns error if there is no relation with row = idx in R_array
int domaninRel(relaceArrT *R_array, int idx, setT *U)
{
	relaceT *rel = getRelFromRelaceArr(R_array, idx);
	if (rel == NULL)
	{
		fprintf(stderr, "relace with row %d not found\n", idx);
		return ELEMENTS_ERROR;
	}
	printDomain(rel, U, false);
	return SUCCESS;
}

//prints codomain of relation
//returns error if there is no relation with row = idx in R_array
int codomaninRel(relaceArrT *R_array, int idx, setT *U)
{
	relaceT *rel = getRelFromRelaceArr(R_array, idx);
	if (rel == NULL)
	{
		fprintf(stderr, "relace with row %d not found\n", idx);
		return ELEMENTS_ERROR;
	}
	printDomain(rel, U, true);
	return SUCCESS;
}

//prints true if function is injective
//returns error if there is no relation with row = idx in R_array
int injectiveRel(relaceArrT *R_array, setArrT *S_array, int *idx)
{
	relaceT *rel = getRelFromRelaceArr(R_array, idx[0]);
	setT *a = getSetFromSetArr(S_array, idx[1]);
	setT *b = getSetFromSetArr(S_array, idx[2]);
	if (rel == NULL)
	{
		fprintf(stderr, "relace with row %d not found\n", idx[0]);
		return ELEMENTS_ERROR;
	}
	if (a == NULL)
	{
		fprintf(stderr, "set with row %d not found\n", idx[1]);
		return ELEMENTS_ERROR;
	}
	if (b == NULL)
	{
		fprintf(stderr, "set with row %d not found\n", idx[2]);
		return ELEMENTS_ERROR;
	}

	//every element in A has 1 image in B
	for (size_t i = 0; i < a->count; i++)
	{
		int count = 0;
		for (size_t j = 0; j < rel->count; j++)
		{
			if (strcmp(a->elements[i], rel->tuples[j]->x) == 0)
				count += 1;
		}
		if (count != 1)
		{
			printf("false\n");
			return SUCCESS;
		}
	}

	//every image in B has 0 or 1 element in A
	for (size_t i = 0; i < b->count; i++)
	{
		int count = 0;
		for (size_t j = 0; j < rel->count; j++)
		{
			if (strcmp(b->elements[i], rel->tuples[j]->y) == 0)
				count += 1;
		}
		if (count > 1)
		{
			printf("false\n");
			return SUCCESS;
		}
	}
	printf("true\n");
	return SUCCESS;
}

//prints true if function is surjective
//returns error if there is no relation with row = idx in R_array
int surjectiveRel(relaceArrT *R_array, setArrT *S_array, int *idx)
{
	relaceT *rel = getRelFromRelaceArr(R_array, idx[0]);
	setT *a = getSetFromSetArr(S_array, idx[1]);
	setT *b = getSetFromSetArr(S_array, idx[2]);
	if (rel == NULL)
	{
		fprintf(stderr, "relace with row %d not found\n", idx[0]);
		return ELEMENTS_ERROR;
	}
	if (a == NULL)
	{
		fprintf(stderr, "set with row %d not found\n", idx[1]);
		return ELEMENTS_ERROR;
	}
	if (b == NULL)
	{
		fprintf(stderr, "set with row %d not found\n", idx[2]);
		return ELEMENTS_ERROR;
	}

	//every element in A has 1 image in B
	for (size_t i = 0; i < a->count; i++)
	{
		int count = 0;
		for (size_t j = 0; j < rel->count; j++)
		{
			if (strcmp(a->elements[i], rel->tuples[j]->x) == 0)
				count += 1;
		}
		if (count != 1)
		{
			printf("false\n");
			return SUCCESS;
		}
	}

	//every image has from B at least 1 element in A
	for (size_t i = 0; i < b->count; i++)
	{
		int count = 0;
		for (size_t j = 0; j < rel->count; j++)
		{
			if (strcmp(b->elements[i], rel->tuples[j]->y) == 0)
				count += 1;
		}
		if (count == 0)
		{
			printf("false\n");
			return SUCCESS;
		}
	}
	printf("true\n");
	return SUCCESS;
}

//prints true if function is bijective
//returns error if there is no relation with row = idx in R_array
int bijectiveRel(relaceArrT *R_array, setArrT *S_array, int *idx)
{
	relaceT *rel = getRelFromRelaceArr(R_array, idx[0]);
	setT *a = getSetFromSetArr(S_array, idx[1]);
	setT *b = getSetFromSetArr(S_array, idx[2]);
	if (rel == NULL)
	{
		fprintf(stderr, "relace with row %d not found\n", idx[0]);
		return ELEMENTS_ERROR;
	}
	if (a == NULL)
	{
		fprintf(stderr, "set with row %d not found\n", idx[1]);
		return ELEMENTS_ERROR;
	}
	if (b == NULL)
	{
		fprintf(stderr, "set with row %d not found\n", idx[2]);
		return ELEMENTS_ERROR;
	}

	if (a->count != b->count)
	{
		printf("false\n");
	}

	//every element in A has 1 image in B
	for (size_t i = 0; i < a->count; i++)
	{
		int count = 0;
		for (size_t j = 0; j < rel->count; j++)
		{
			if (strcmp(a->elements[i], rel->tuples[j]->x) == 0)
				count += 1;
		}
		if (count != 1)
		{
			printf("false\n");
			return SUCCESS;
		}
	}

	//every image has 1 element
	for (size_t i = 0; i < b->count; i++)
	{
		int count = 0;
		for (size_t j = 0; j < rel->count; j++)
		{
			if (strcmp(b->elements[i], rel->tuples[j]->y) == 0)
				count += 1;
		}
		if (count != 0)
		{
			printf("false\n");
			return SUCCESS;
		}
	}
	printf("true\n");
	return SUCCESS;
}

// Function to decide which command has been called from file
int chooseCommand(setArrT *S_array, relaceArrT *R_array, commandArrT *C_array, setT *U)
{
	for (size_t i = 0; i < C_array->count; i++)
	{

		// The idx needs to be allocated dynamically because the C_array->functions[]->function[] is also allocated dynamically
		int *idx = malloc(C_array->functions[i]->count * sizeof(int));
		if (idx == NULL)
		{
			fprintf(stderr, "ERORR: Malloc fail");
			return MEM_ERROR;
		}
		// To convert strings with index of set/relace from C->functions to int so we can use them as an index of row where the relace/set is located
		for (unsigned int j = 0; j < C_array->functions[i]->count - 1; j++)
		{
			idx[j] = strtol(C_array->functions[i]->function[j + 1], NULL, 10);
		}

		int outcode;

		if (strcmp("empty", C_array->functions[i]->function[0]) == 0)
		{
			if (C_array->functions[i]->count != 2)
			{ //command + S
				free(idx);
				fprintf(stderr, "wrong number of arguments\n");
				return ELEMENTS_ERROR;
			}
			outcode = emptySet(S_array, idx[0], U);
		}
		else if (strcmp("card", C_array->functions[i]->function[0]) == 0)
		{
			if (C_array->functions[i]->count != 2)
			{ //command + S
				free(idx);
				fprintf(stderr, "wrong number of arguments\n");
				return ELEMENTS_ERROR;
			}
			outcode = cardSet(S_array, idx[0], U);
		}
		else if (strcmp("complement", C_array->functions[i]->function[0]) == 0)
		{
			if (C_array->functions[i]->count != 3)
			{ //command + S + S
				free(idx);
				fprintf(stderr, "wrong number of arguments\n");
				return ELEMENTS_ERROR;
			}
			outcode = complementSet(S_array, U, idx[0]);
		}
		else if (strcmp("intersect", C_array->functions[i]->function[0]) == 0)
		{
			if (C_array->functions[i]->count != 3)
			{ //command + S + S
				free(idx);
				fprintf(stderr, "wrong number of arguments\n");
				return ELEMENTS_ERROR;
			}
			outcode = intersectSet(S_array, idx);
		}
		else if (strcmp("minus", C_array->functions[i]->function[0]) == 0)
		{
			if (C_array->functions[i]->count != 3)
			{ //command + S + S
				free(idx);
				fprintf(stderr, "wrong number of arguments\n");
				return ELEMENTS_ERROR;
			}
			minusSet(S_array, idx);
		}
		else if (strcmp("equals", C_array->functions[i]->function[0]) == 0)
		{
			if (C_array->functions[i]->count != 3)
			{ //command + S + S
				free(idx);
				fprintf(stderr, "wrong number of arguments\n");
				return ELEMENTS_ERROR;
			}
			equalsSet(S_array, idx);
		}

		else if (strcmp("reflexive", C_array->functions[i]->function[0]) == 0)
		{
			if (C_array->functions[i]->count != 2)
			{ //command + R
				free(idx);
				fprintf(stderr, "wrong number of arguments\n");
				return ELEMENTS_ERROR;
			}
			outcode = reflexiveRel(R_array, idx[0], U);
		}
		else if (strcmp("symmetric", C_array->functions[i]->function[0]) == 0)
		{
			if (C_array->functions[i]->count != 2)
			{
				free(idx);
				fprintf(stderr, "wrong number of arguments\n");
				return ELEMENTS_ERROR;
			}
			outcode = symmetricRel(R_array, idx[0]);
		}
		else if (strcmp("antisymmetric", C_array->functions[i]->function[0]) == 0)
		{
			if (C_array->functions[i]->count != 2)
			{
				free(idx);
				fprintf(stderr, "wrong number of arguments\n");
				return ELEMENTS_ERROR;
			}
			outcode = antisymmetricRel(R_array, idx[0]);
		}
		else if (strcmp("transitive", C_array->functions[i]->function[0]) == 0)
		{
			if (C_array->functions[i]->count != 2)
			{
				free(idx);
				fprintf(stderr, "wrong number of arguments\n");
				return ELEMENTS_ERROR;
			}
			outcode = transitiveRel(R_array, idx[0]);
		}
		else if (strcmp("function", C_array->functions[i]->function[0]) == 0)
		{
			if (C_array->functions[i]->count != 2)
			{
				free(idx);
				fprintf(stderr, "wrong number of arguments\n");
				return ELEMENTS_ERROR;
			}
			outcode = functionRel(R_array, idx[0]);
		}
		else if (strcmp("domain", C_array->functions[i]->function[0]) == 0)
		{
			if (C_array->functions[i]->count != 2)
			{
				free(idx);
				fprintf(stderr, "wrong number of arguments\n");
				return ELEMENTS_ERROR;
			}
			outcode = domaninRel(R_array, idx[0], U);
		}
		else if (strcmp("codomain", C_array->functions[i]->function[0]) == 0)
		{
			if (C_array->functions[i]->count != 2)
			{
				free(idx);
				fprintf(stderr, "wrong number of arguments\n");
				return ELEMENTS_ERROR;
			}
			outcode = codomaninRel(R_array, idx[0], U);
		}
		else if (strcmp("injective", C_array->functions[i]->function[0]) == 0)
		{
			if (C_array->functions[i]->count != 5)
			{
				free(idx);
				fprintf(stderr, "wrong number of arguments\n");
				return ELEMENTS_ERROR;
			}
			outcode = injectiveRel(R_array, S_array, idx);
		}
		else if (strcmp("surjective", C_array->functions[i]->function[0]) == 0)
		{
			if (C_array->functions[i]->count != 5)
			{
				free(idx);
				fprintf(stderr, "wrong number of arguments\n");
				return ELEMENTS_ERROR;
			}
			outcode = surjectiveRel(R_array, S_array, idx);
		}
		else if (strcmp("bijective", C_array->functions[i]->function[0]) == 0)
		{
			if (C_array->functions[i]->count != 5)
			{
				free(idx);
				fprintf(stderr, "wrong number of arguments\n");
				return ELEMENTS_ERROR;
			}
			outcode = bijectiveRel(R_array, S_array, idx);
		}

		else
		{
			fprintf(stderr, "\nERROR: Unknown command\n");
			free(idx);
			return UNKNOWN_COMMAND;
		}
		free(idx);
		if (outcode != SUCCESS)
		{
			return outcode;
		}
	}
	return SUCCESS;
}

int main(int argc, char *argv[])
{
	FILE *fp = fopen(argv[1], "r");
	int ERR_CODE = errorHandling(argc, fp);
	if (ERR_CODE)
		return ERR_CODE;

	setT *U = setInitialize();
	setArrT *S_array = setArray_Const();
	relaceArrT *R_array = relaceArray_Const();
	commandArrT *C_array = commandArray_Const();

	if (checkElements(U, S_array, R_array, C_array) == MEM_ERROR)
	{
		fprintf(stderr, "\nERROR: Pointers were wrong initialized!\n");
		destroyAllElements(U, S_array, R_array, C_array);
		fclose(fp);
		return MEM_ERROR;
	}

	if (getDataFromFile(fp, U, S_array, R_array, C_array) != SUCCESS)
	{
		fprintf(stderr, "\nERROR: Something gone wrong while getting data from file!\n");
		destroyAllElements(U, S_array, R_array, C_array);
		fclose(fp);
		return MEM_ERROR;
	}

	if (isSetInUniv(S_array, U) != SUCCESS || isRelaceInUniv(R_array, U) != SUCCESS)
	{
		destroyAllElements(U, S_array, R_array, C_array);
		fclose(fp);
		return FILE_CRASH;
	}

	if (chooseCommand(S_array, R_array, C_array, U) != SUCCESS)
	{
		destroyAllElements(U, S_array, R_array, C_array);
		fclose(fp);
		return FILE_CRASH;
	}

	destroyAllElements(U, S_array, R_array, C_array);

	fclose(fp);
	return SUCCESS;
}
