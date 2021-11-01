#include "20161559.h"

int main(void)
{
	char command[100];	//array that stores command
	char* command_trimmed;	//array that stores trimmed command(left space righr space removed.)
	FILE *pFile=NULL;	//file pointer to open "opcode.txt"
	
	memory = (unsigned char*)calloc(0xFFFFF, sizeof(unsigned char));	//dynamically allocating 1MB virtual memory

	symbolTable.top=NULL;	//Data structure that stores symbols in .asm file

	for(int i=0; i<0xFFFFF; i++)
		bp_status[i]=0;

	//Process of Generating HashTable
	for(int i=0; i<20; i++)
		hashTable[i]=NULL;
	for(int i=0; i<10; i++)
		ESTable[i]=NULL;
	pFile=fopen("opcode.txt", "r");
	if(pFile==NULL) {
		puts("File open Error!");
		return 1;
	}

	while(1) {	//loop until reaches end of file
		hashPointer temp=(hashPointer)malloc(sizeof(struct hashNode));
		temp->next=NULL;
		fscanf(pFile, "%x %s %s\n", &(temp->opcode), temp->op, temp->type);	//stores opcode, operation name, instrution format respectively
		int index=hashing(temp->op);
		if(hashTable[index]==NULL)
			hashTable[index]=temp;
		else {
			hashPointer temp2=hashTable[index];
			while(temp2->next!=NULL)
				temp2 = temp2->next;
			temp2->next=temp;
		}
		if(feof(pFile))	//checks if pFile reaches the end of the file
			break;
	}
	fclose(pFile);


	//******Main part of the program : Continues to receive command from user and manage appropriately******
	while(1) {
		printf("sicsim> ");
		fgets(command, 100, stdin);
		command[strlen(command)-1]='\0';
		command_trimmed=trim(command);

		//deallocate memories in heap area before the program exist.
		if(strcmp(command_trimmed, "quit")==0 || strcmp(command_trimmed, "q")==0) {
			history_cleaner();
			hashTable_cleaner();
			hashTable2_cleaner();
			file_list_cleaner();
			bp_cleaner();
			tree_cleaner(symbolTable.top);
			tree_cleaner(temp_symbol);
			free(memory);
			free(command_trimmed);

			break;
		}
		commandManager(command, command_trimmed);	//function that manages input command
	}

	return 0;
}
