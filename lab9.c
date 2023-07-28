#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int id;
	char name;
	int order;
};

// Define the maximum size of the hash table
#define MAX_HASH_SIZE 10

// Node to store a record in the hash table
struct HashNode
{
	struct RecordType data;
	struct HashNode* next;
};

// HashType to store the hash table
struct HashType
{
	struct HashNode* hashTable[MAX_HASH_SIZE];
};

// Compute the hash function
int hash(int x)
{
	return x % MAX_HASH_SIZE;
}

// Parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}

		for (i = 0; i < dataSz; ++i)
		{
			fscanf(inFile, "%d ", &(*ppData)[i].id);
			fscanf(inFile, "%c ", &(*ppData)[i].name);
			fscanf(inFile, "%d ", &(*ppData)[i].order);
		}

		fclose(inFile);
	}

	return dataSz;
}

// Function to insert a record into the hash table
void insertRecordIntoHash(struct HashType* pHashArray, struct RecordType record)
{
	int hashIndex = hash(record.id);
	struct HashNode* newNode = (struct HashNode*)malloc(sizeof(struct HashNode));
	newNode->data = record;
	newNode->next = NULL;

	// If the slot is empty, add the new node
	if (pHashArray->hashTable[hashIndex] == NULL)
	{
		pHashArray->hashTable[hashIndex] = newNode;
	}
	else
	{
		// If the slot is not empty, add the new node at the end of the chain
		struct HashNode* currentNode = pHashArray->hashTable[hashIndex];
		while (currentNode->next != NULL)
		{
			currentNode = currentNode->next;
		}
		currentNode->next = newNode;
	}
}

// Display records in the hash structure
// The output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType* pHashArray, int hashSz)
{
	int i;
	for (i = 0; i < hashSz; ++i)
	{
		if (pHashArray->hashTable[i] != NULL)
		{
			struct HashNode* currentNode = pHashArray->hashTable[i];
			printf("index %d -> ", i);
			while (currentNode != NULL)
			{
				printf("%d %c %d", currentNode->data.id, currentNode->data.name, currentNode->data.order);
				currentNode = currentNode->next;
				if (currentNode != NULL)
					printf(" -> ");
			}
			printf("\n");
		}
	}
}

int main(void)
{
	struct RecordType* pRecords;
	int recordSz = 0;

	// Parse the data from the input file
	recordSz = parseData("input.txt", &pRecords);

	// Create the hash table
	struct HashType hashTable;
	for (int i = 0; i < MAX_HASH_SIZE; i++)
	{
		hashTable.hashTable[i] = NULL;
	}

	// Insert records into the hash table
	for (int i = 0; i < recordSz; i++)
	{
		insertRecordIntoHash(&hashTable, pRecords[i]);
	}

	// Display the records in the hash table
	displayRecordsInHash(&hashTable, MAX_HASH_SIZE);

	// Free dynamically allocated memory for pRecords
	free(pRecords);

	return 0;
}


