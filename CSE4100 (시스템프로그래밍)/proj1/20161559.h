#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

//Structure that implements Linked List
typedef struct listNode *listPointer;
struct listNode{
	char cmd[30];
	listPointer link;
};

//Structure that implements Hash Table
typedef struct hashNode *hashPointer;
struct hashNode {
	char op[10];
	int opcode;
	char type[4];
	hashPointer next;
};

unsigned char* memory;	//Virtual memory
listPointer first=NULL;	//head part of linkedlist
listPointer trail=NULL;	//end part of linkedlist
int start_address=0;	//variable that stores end address when function "dump" is called
hashPointer hashTable[20];	//Hash Table

//Hashfunction 
int hashing(char *str) {
	int length=strlen(str);
	int result=0;

	for(int i=0; i<length; i++)
		result += (int)str[i];
	
	return result%20;
}

//function that removes left space of string
char* ltrim(char *str) {
	int i, j;
	int length=strlen(str);

	i=0;
	while(*(str+i)==' ')
		i++;
	for(j=0; j<length; j++)
		str[j] = str[j+i];
	
	return str;
}

//function that removes right space of string
char* rtrim(char *str) {
	char *temp = (char*)malloc(sizeof(char)*strlen(str));
	int i=strlen(str)-1;
	
	while(i>=0 && str[i]==' ')
		i--;
	temp[i+1]='\0';
	while(i>=0) {
		temp[i] = str[i];
		i--;
	}
	return temp;
}

//function that removes left, right space of string
char* trim(char *str) {
	return ltrim(rtrim(str));
}

//function that deallocates memories used in linkedlist
void history_cleaner(void) {
	listPointer temp=first;
	while(first!=NULL) {
		first = first->link;
		free(temp);
		temp = first;
	}
	return;
}

//function that deallocates memories used in hashtable
void hashTable_cleaner(void) {
	for(int i=0; i<20; i++) {
		hashPointer target=hashTable[i];
		while(target!=NULL) {
			hashPointer temp=target;
			target = target->next;
			free(temp);
		}
	}
	return;
}

//function that adds command to history(linkedlist)
void insert_command(char *cmd) {
	listPointer temp;
	temp=(listPointer)malloc(sizeof(struct listNode));
	strncpy(temp->cmd, cmd, strlen(cmd)+1);

	if(first==NULL)
		first = temp;
	else
		trail->link = temp;
	trail = temp;
	trail->link=NULL;
}

//function that prints all of the possible commands
void help(void) {
	puts("");
	puts("h[elp]");
	puts("d[ir]");
	puts("q[uit]");
	puts("hi[story]");
	puts("du[mp] [start, end]");
	puts("e[dit] address, value");
	puts("f[ill] start, end, value");
	puts("reset");
	puts("opcode mnemonic");
	puts("opcodelist");
	puts("");
}

//function that prints out directory and files in current directory
void dir(void) {
	DIR *pDir=NULL;
	struct dirent *pFile=NULL;
	struct stat buf;

	pDir=opendir(".");
	if(!pDir) {
		printf("ERROR\n");
		return;
	}

	while((pFile=readdir(pDir))!=NULL)
	{
		printf("\t%s", pFile->d_name);
		stat(pFile->d_name, &buf);
		if(S_ISDIR(buf.st_mode)) printf("/");	//if pFile is directory, then adds '/'symbol
		else if(buf.st_mode & S_IXUSR) printf("*");	//if pFile is executable file, then adds '*'symbol
	}
	closedir(pDir);
	puts("");

	return;
}

//function that prints out elements in linkedlist (prints out all of the commands executed)
void history(void) {
	listPointer temp=first;
	for(int i=1; temp!=NULL; temp=temp->link) {
		printf("\t%-3d ", i++);
		printf("%s\n", temp->cmd);
	}
	puts("");
}

//Shows contents of memories starting from start address to end address  
void dump(int start, int end) {

	int start_line=start-start%16;	//calculate address of starting line(line that involves start address)
	int end_line=end-end%16;	//calculate address of end line(line that involves end address)


	for(int i=start_line; i<=end_line; i+=16) {
		printf("%05X ", i);	//prints memory location
		for(int j=i; j<i+16; j++) {
			if(j<start || end<j)
				printf("   ");
			else
				printf("%02X ", memory[j]);	//prints element stored in memory in hexadecimal form
		}

		printf("; ");
		for(int j=i; j<i+16; j++) {
			if(0x20<=memory[j]&&memory[j]<=0x7E)
				printf("%c", memory[j]);	//prints element stored in memory in character form
			else
				printf(".");
		}
		puts("");
	}

	start_address = end+1>0xFFFFF ? 0 : end+1;	//if next address(end+1) exceeds 0xFFFFF, then set to 0

	return;
}

//set memory element to value
void edit(int address, int value) {
	memory[address] = value;
}

//set memory(from start address to end address)  element to value 
void fill(int start, int end, int value) {
	for(int i=start; i<=end; i++)
		memory[i] = value;
}

//set all of the memory elements to zero
void reset(void) {
	for(int i=0; i<=0xFFFFF; i++)
		memory[i]=0;
}

//show all of the opcode name in hashtable
void opcodelist(void) {
	//loop until variable temp reaches null in last element of 20th row of hashtable
	for(int i=0; i<20; i++) {
		hashPointer temp = hashTable[i];
		printf("%d : ", i);
		if(temp!=NULL) {
			while(temp!=NULL) {
				printf("[%s,%02X]", temp->op, temp->opcode);	//prints operation name and opcode

				if(temp->next!=NULL)
					printf(" -> ");
				temp = temp->next;
			}
		}
		puts("");
	}
	return;
}

//function that changes hex num stored in character arry in to integer
int parseHex(char *str, int start, int end) {
	//removes possible space (trimming)
	if(start>=end)
		return -1;
	int result=0;
	while(str[start]==' ')	//removing left space
		start++;
	while(str[end]==' ')	//removing right space
		end--;

	for(int i=start; i<=end; i++) {
		int temp_num=str[i];
		if(temp_num=='A' || temp_num=='a')
			result += 10;
		else if(temp_num=='B' || temp_num=='b')
			result += 11;
		else if(temp_num=='C' || temp_num=='c')
			result += 12;
		else if(temp_num=='D' || temp_num=='d')
			result += 13;
		else if(temp_num=='E' || temp_num=='e')
			result += 14;
		else if(temp_num=='F' || temp_num=='f')
			result += 15;
		else {
			temp_num-='0';
			if(temp_num<0 || temp_num>9)
				return -1;
			else
				result += temp_num;
		}
		if(i!=end)
			result *= 16;
	}
	return result;
}

//******Core of the program. Handles all of the commmands******
void commandManager(char *command, char *command_trimmed) {
	//if command "help" is called, add to history and call function help()
	if(strcmp(command_trimmed, "help")==0 || strcmp(command_trimmed, "h")==0) {
		insert_command(command);
		help();
	}

	//if command "dir" is called, add to history and call function dir()
	else if(strcmp(command_trimmed, "dir")==0 || strcmp(command_trimmed, "d")==0) {
		insert_command(command);
		dir();
	}

	//if command "history" is called, add to history and call function history()
	else if(strcmp(command_trimmed, "history")==0 || strcmp(command_trimmed, "hi")==0) {
		insert_command(command);
		history();
	}

	//if command "dump" is called, check further in order to examine whether it is valid command
	else if(strncmp(command_trimmed, "dump", 4)==0 || strncmp(command_trimmed, "du", 2)==0) {
		int boundary;
		if(strncmp(command_trimmed, "dump", 4)==0)
			boundary=4;	//case that command is "dump"
		else
			boundary=2;	//case that command is "du"

		//dump command with 1st form(without any argument)
		if(command_trimmed[boundary]=='\0') {
			insert_command(command);
			dump(start_address, start_address+159);
		}

		//dump command with other forms(with one or two arguments)
		else if(command_trimmed[boundary]==' ') {
			int start=parseHex(command_trimmed, boundary, strlen(command_trimmed)-1);
			//dump command with 2nd form(one argument)
			if(0<=start&&start<=0xFFFFF) {	//address boundary check
				insert_command(command);
				dump(start, start+159);
			}
			else {
				int temp;
				int length=strlen(command_trimmed);
				for(temp=boundary; temp<length; temp++) {
					if(command_trimmed[temp]==',')
						break;
				}
				int start=parseHex(command_trimmed, boundary, temp-1);
				int end=parseHex(command_trimmed, temp+1, strlen(command_trimmed)-1);
				if(start>end)	//address validity check
					puts("Start Address Bigger than End Address!");
				else if(start<0||0xFFFFF<start||end<0||0xFFFFF<end)
					puts("Invalid Address!");
				//dump command with 3rd form(two arguments)
				else if(0<=start&&start<=0xFFFFF&&0<=end&&end<=0xFFFFF&&start<=end) {	//address boundary check
					insert_command(command);
					dump(start, end);
				}
			}
		}
	}

	//if command "edit" is called, check further in order to examine whether it is valid command
	else if(strncmp(command_trimmed, "edit", 4)==0 || strncmp(command_trimmed, "e", 1)==0) {
		int boundary;
		if(strncmp(command_trimmed, "edit", 4)==0)
			boundary=4;	//case that command is "edit"
		else
			boundary=1;	//case that command is "e"

		if(command_trimmed[boundary]==' ') {
			int temp;
			int length=strlen(command_trimmed);
			for(temp=boundary; temp<length; temp++) {
				if(command_trimmed[temp]==',')
					break;
			}
			int address=parseHex(command_trimmed, boundary, temp-1);	//parsing address part from command
			int value=parseHex(command_trimmed, temp+1, strlen(command_trimmed)-1);	//parsign value part from command
			if(address<0||0xFFFFF<address||value<0||0xFF<value) {	//address & value validity check
				if(address<0||0xFFFFF<address)
					puts("Invalid Address!");
				if(value<0||0xFF<value)
					puts("Invalid Value!");
			}

			//if address and value are both valid, call edit function
			else if(0<=address&&address<=0xFFFFF&&0<=value&&value<=0xFF) {
				insert_command(command);
				edit(address, value);
			}
		}
	}

	//if command "fill" is called, check further to examine command 
	else if (strncmp(command_trimmed, "fill", 4)==0 || strncmp(command_trimmed, "f", 1)==0) {
		int boundary;
		if(strncmp(command_trimmed, "fill", 4)==0)
			boundary=4;	//case that command is "fill"
		else
			boundary=1;	//case that command is "f"

		if(command_trimmed[boundary]==' ') {
			int temp1, temp2;
			int length=strlen(command_trimmed);
			for(temp1=boundary; temp1<length; temp1++) {
				if(command_trimmed[temp1]==',')
					break;
			}

			for(temp2=temp1+1; temp2<length; temp2++) {
				if(command_trimmed[temp2]==',')
					break;
			}
			int start=parseHex(command_trimmed, boundary, temp1-1);	//parsing address(start) from command
			int end=parseHex(command_trimmed, temp1+1, temp2-1);	//parsing address(end) from command
			int value=parseHex(command_trimmed, temp2+1, strlen(command_trimmed)-1);	//parsing value from command
			if(start>end)	//wrong relationship between start and end
				puts("Start Address Bigger than End Address!");
			else if(start<0||0xFFFFF<start||end<0||0xFFFFF<end||value<0||0xFF<value) {	//arguments validy check
				if(start<0||0xFFFFF<start||end<0||0xFFFFF)
					 puts("Invalid Address!");
				if(value<0||0xFF<value)
					puts("Invalid Value!");
			}
			//if addresses and value are both valid, call fill function 
			else if(0<=start&&start<=0xFFFFF&&start<=end&&end<=0xFFFFF&&0<=value&&value<=0xFF) {
				insert_command(command);
				fill(start, end, value);
			}
		}
	}

	//if command is "reset" add to history and call reset function
	else if(strncmp(command_trimmed, "reset", 5)==0) {
		insert_command(command);
		reset();
	}

	//if command is "opcodelist", add to history and call opcodelist function
	else if(strncmp(command_trimmed, "opcodelist", 10)==0) {
		insert_command(command);
		opcodelist();
	}

	//if command is "opcode", check further to examine operation really exists
	else if(strncmp(command_trimmed, "opcode", 6)==0) {
		char temp_op[10];
		char* temp_op_trimmed;
		int length=strlen(command_trimmed);
		for(int i=6; i<length; i++)
			temp_op[i-6]=command_trimmed[i];
		temp_op[length-6]='\0';
		temp_op_trimmed=trim(temp_op);	//trim argument(operation name)
		int index=hashing(temp_op_trimmed);
		hashPointer temp=hashTable[index];
		//loop continues until there is a match between operation in hashtable and input command or reaches the end
		while(temp!=NULL) {
			if(strncmp(temp->op, temp_op_trimmed, strlen(temp->op)+1)==0) {
				insert_command(command);
				printf("opcode is %02X\n", temp->opcode);	//print in hex form
				break;
			}
			temp = temp->next;
		}
		free(temp_op_trimmed);
	}

	//case that input command is invalid
	else
		puts("Invalid Command!");

	free(command_trimmed);
}
