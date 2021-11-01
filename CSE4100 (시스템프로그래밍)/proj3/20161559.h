#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>


//***User-defined data structure
//Structure that implements Linked List
typedef struct listNode *listPointer;
struct listNode{
	char cmd[50];
	listPointer link;
};

//Structure that stores BreakPoints
typedef struct listNode2 *listPointer2;
struct listNode2{
	int locctr;
	listPointer2 link;
};

//Structure that implements linked list of file (for load operation)
typedef struct file_listNode *file_listPointer;
struct file_listNode {
	char filename[10];
	file_listPointer link;
};

//Structure that implements Hash Table
typedef struct hashNode *hashPointer;
struct hashNode {
	char op[10];
	int opcode;
	char type[4];
	hashPointer next;
};

//Structure that stores external-symbol
typedef struct hashNode2 *hashPointer2;
struct hashNode2 {
	char control_section[10];
	char symbol_name[10];
	int address;
	int program_length;
	hashPointer2 next;
};

//Structure that stores Record-Reference information
typedef struct {
	char symbol[7];
	int used_flag;
} Reference;



//Structure that implements Binary tree
typedef struct treeNode *treePointer;
struct treeNode {
	int LOCCTR;
	char label[100];
	treePointer left, right;
};

//Structure that implements Symbol Table
typedef struct {
	int flag;	//Stores line number where error occcurs
	char error_message[50];
	int starting_address;	//Stores starting address of a program
	int program_length;	//Stores length of a program 
	treePointer top;
} Symbol;


//***Global Variables
unsigned char* memory;	//Virtual memory (Size of 1MB)
listPointer first=NULL;	//Head part of linkedlist
listPointer trail=NULL;	//End part of linkedlist
listPointer2 first_bp=NULL;	//Head part of breakpoint
listPointer2 trail_bp=NULL;	//End part of breakpoint

file_listPointer first_file=NULL;	//Head part of file_list
file_listPointer trail_file=NULL;	//End part of file_list
int start_address=0;	//Variable that stores end address when function "dump" is called
int start_progaddr=0;	//Variable that stores starting address of program when loaded
hashPointer hashTable[20];	//Hash Table
hashPointer2 ESTable[10];	//External-Symbol Table
Symbol symbolTable;	//Symbol Table
treePointer temp_symbol=NULL;	//temp symbol table to store contents(symbols) of successfully assembled file previously
int program_lines;	//Number of lines in .asm file
int total_length;	//total length of loaded program
int bp_status[0xFFFFF];
int A, X, L, PC, B, S, T;	//Registers
int length;
int locctr;
char CC='\0';	//condition code



//***Primary functions : Functions that perform main part of the program (controls input command)
void help(void);	//prints out possible commands
void dir(void);	//prints out list of files and directories located in current directory
void history(void);	//prints out history of input command
void insert_command(char*);	//insert effective commands to history
void dump(int, int);	//prints out elements stored in Virtual memory
void edit(int, int);	//edit elements in Virtual memory
void fill(int, int, int);	//edit elements in Virtual memory by given value and range
void reset(void);	//reset all of the elements stored in Virtual memory by '0'
void opcodelist(void);	//prints out possible operation and its opcode in hash table form
void type(char*);	//prints out contents of input file
void assemble(char*, char*);	//assemble .asm file and generate .lst file / .obj file return 0 if there's any error. Other wire returns 1
int pass1(char*);	//pass1 algorithm of assemble function. returns 0 if there's any error. Otherwise returns 1
int pass2(char*);	//pass2 algorithm of assemble function. returns 0 if there's any error. Otherwise returns 1
void print_symbol(treePointer);	//prints out stored symbol and its location in alphabetic order
void commandManager(char*, char*);	//manages input command and calls other primary functions
void loader(char*);	//loader function
int loader_pass1(void);	//pass1 algorithm of load function.
int loader_pass2(void);	//pass2 algorithm of load function.
void run(void);	//program run function.

//***Secondary functions : Functions that support Primary functions(parsing, searching, deleting, etc)
treePointer search(treePointer, char*);	//binary search for treePointer that has corresponding string value
void insert_node(treePointer*, int, char*);	//insert treePointer to binary tree
void insert_bp(int);	//add break point
void insert_file(char*);	//insert file_name to file_list
void tree_cleaner(treePointer);	//free all of the elements in tree

int hashing(char*);	//returns hash value of input string
void hashTable_cleaner(void);	//free all of the elements in Hash table
void hashTable2_cleaner(void); //free all of the elements in Hash table2(external-symbol table)

char* ltrim(char*);	//remove left space of input string
char* rtrim(char*);	//remove right space of input string
char* trim(char*);	//remove left and right space of input string(By calling ltrim and rtrim);

void history_cleaner(void);	//free all of the elements in history(linked list)
void bp_cleaner(void);	//free all of the breakpoints
void file_list_cleaner(void); //free all of the file_list in file_list(linked list)

void tokenize(char* agv1, char* agv2, char* agv3, char* target);	//tokenize input string(target) by space and store results in strings(agv1, agv2, agv3)
void numChange(char*, unsigned int, int length);	//change unsigned integer in string form. string length is set as "length"
int parseNum(char*, int, int, int);	//change number in character to integer. returns -1 if string can't be converted to integer.



treePointer search(treePointer node, char* str) {
	while(node) {
		int compare=strcmp(str, node->label);

		if(compare==0)	//Case that corresponding treePointer found
			return node;
		else if(compare > 0) 	//Case that input string is bigger than node label
			node = node->right;	
		else 	//Case that input string is maller than node label
			node = node->left;
	}

	return NULL;
}


void insert_node(treePointer *root, int LOCCTR, char str[]) {
	treePointer p, t;
	treePointer n;
	t = *root;
	p = NULL;

	while(t) {
		p = t;
		if(strcmp(str, p->label)<0)
			t = p->left;
		else
			t = p->right;
	}

	n = (treePointer)malloc(sizeof(struct treeNode));
	n->left=NULL;
	n->right=NULL;
	n->LOCCTR=LOCCTR;
	strcpy(n->label, str);

	if(p) {
		if(strcmp(str, p->label)<0)
			p->left=n;
		else
			p->right=n;
	}
	else
		*root = n;
}


void tree_cleaner(treePointer top) {
	if(top) {
		if(top->left)
			tree_cleaner(top->left);
		if(top->right)
			tree_cleaner(top->right);
		free(top);
	}
}


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
}

void bp_cleaner(void) {
	listPointer2 temp=first_bp;

	while(first_bp!=NULL) {
		first_bp = first_bp->link;
		bp_status[temp->locctr]=0;
		free(temp);
		temp = first_bp;
	}
}

//function that deallocates memories used in file_list
void file_list_cleaner(void) {
	file_listPointer temp=first_file;

	while(first_file!=NULL) {
		first_file = first_file->link;
		free(temp);
		temp = first_file;
	}
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
}

//function that deallocates memories used in external-symbol table
void hashTable2_cleaner(void) {
	for(int i=0; i<10; i++) {
		hashPointer2 target=ESTable[i];
		while(target!=NULL) {
			hashPointer2 temp=target;
			target = target->next;
			free(temp);
		}
	}

	for(int i=0; i<10; i++)
		ESTable[i]=NULL;
}


//function that adds command to history(linkedlist)
void insert_command(char *cmd) {
	listPointer temp;
	temp=(listPointer)malloc(sizeof(struct listNode));
	strcpy(temp->cmd, cmd);

	if(first==NULL)
		first = temp;
	else
		trail->link = temp;
	trail = temp;
	trail->link=NULL;
}

void insert_bp(int locctr) {
	listPointer2 temp;
	temp=(listPointer2)malloc(sizeof(struct listNode2));
	temp->locctr=locctr;

	if(first_bp==NULL)
		first_bp = temp;
	else
		trail_bp->link = temp;
	trail_bp = temp;
	trail_bp->link=NULL;
}

//function that adds file to file_list(linkedlist)
void insert_file(char *filename) {
	file_listPointer temp;
	temp=(file_listPointer)malloc(sizeof(struct file_listNode));
	strcpy(temp->filename, filename);

	if(first_file==NULL)
		first_file = temp;
	else
		trail_file->link = temp;
	trail_file=temp;
	trail_file->link=NULL;
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
	puts("assemble filename");
	puts("type filename");
	puts("symbol");
	puts("progaddr address");
	puts("loader [object filename1] [object filename2] [...]");
	puts("bp [address]");
	puts("bp clear");
	puts("bp");
	puts("run");
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
			if(0x20<=memory[j]&&memory[j]<=0x7E && (start<=j && j<=end))
				printf("%c", memory[j]);	//prints element stored in memory in character form
			else
				printf(".");
		}
		puts("");
	}

	start_address = end+1>0xFFFFF ? 0 : end+1;	//if next address(end+1) exceeds 0xFFFFF, then set to 0
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
}


//show contents of filename
void type(char* filename) {
	FILE *pFile=NULL;
	pFile=fopen(filename, "r");
	char text;

	while(1) {	//loop until end of file
		text=fgetc(pFile);	//read from file
		if(text==EOF)
			break;
		else
			putchar(text);	//print out directly
	}
	fclose(pFile);
}


/*tokenize target string by character ' '(space) and save each elements in s1, s2, s3
This function  tokenize input line from .asm file as "LABEL", "OPCODE", "OPERAND"*/
void tokenize(char s1[], char s2[], char s3[], char target[]) {
	int length=strlen(target);
	int index=0;
	int i;

	//initialize s1, s2, s3 as null string
	s1[0]='\0';
	s2[0]='\0';
	s3[0]='\0';

	//copying target to s1 until space appears
	i=0;
	while(target[index]!=' ' && index<length)
		s1[i++]=target[index++];
	s1[i]='\0';
	if(i==0) {
		s1[0]='!';
		s1[1]='\0';
	}
	//removing space
	while(target[index]==' ' && index<length)
		index++;

	//copying target to s2 until space appears
	i=0;
	while(target[index]!=' ' && index<length)
		s2[i++]=target[index++];
	s2[i]='\0';
	if(i==0) {
		s2[0]='!';
		s2[1]='\0';
	}
	//removing space
	while(target[index]==' ' && index<length)
		index++;

	//copying target to s3 until the end
	//this case is different from upper cases because there can be space in "OPERAND" part (i.e. "BUFFER, X")
	i=0;
	while(index<length) {
		if(target[index]==' ') {
			index++;
			continue;
		}
		s3[i++]=target[index++];
	}
	s3[i]='\0';
	//update s3 as "!" to indicate no contents in "OPERAND" field
	if(i==0) {
		s3[i]='!';
		s3[i+1]='\0';
	}
}


//change number(instruction code) in character format
void numChange(char str[], unsigned int x, int length) {
	for(int i=length-1; i>=0; i--) {
		int remainder=x%16;
		if(remainder==10)
			str[i]='A';
		else if(remainder==11)
			str[i]='B';
		else if(remainder==12)
			str[i]='C';
		else if(remainder==13)
			str[i]='D';
		else if(remainder==14)
			str[i]='E';
		else if(remainder==15)
			str[i]='F';
		else
			str[i]=remainder+'0';
		x/=16;
	}

	str[length]='\0';
}


//pass1 algorithm of function "assemble" returns 1 if function ends successfully, otherwise returns 0
int pass1(char* filename) {
	int LOCCTR;
	int PC;
	int line_num=5;	//current line number
	FILE *pFile_input=NULL;	//file pointer that points input file
	FILE *pFile_intermediate=NULL;	//file pointer that points temp file named as "intermediate.txt"
	char input_line[200];	//read line-by-line from input file
	char agv1[100];	//character array of "LABEL" part
	char agv2[10];	//character array of "OPCODE" part
	char agv3[90];	//character array of "OPERAND" part

	pFile_input=fopen(filename, "r");
	pFile_intermediate=fopen("intermediate.txt", "w");

	//initialize contents in symbolTable
	tree_cleaner(symbolTable.top);
	symbolTable.top=NULL;
	symbolTable.flag=0;
	symbolTable.starting_address=0;
	symbolTable.program_length=0;

	fgets(input_line, sizeof(input_line), pFile_input);	//read first input line
	input_line[strlen(input_line)-1]='\0';
	tokenize(agv1, agv2, agv3, input_line);	//tokenize elements in input line

	//if opcode=="START", then save operand as starting address and initialize LOCCTR as starting address, otherwise initialize as 0
	if(strcmp(agv2, "START")==0) {
		symbolTable.starting_address=parseNum(agv3, 0, strlen(agv3)-1, 16);
		LOCCTR=symbolTable.starting_address;
	}
	else
		LOCCTR=0;
	PC=LOCCTR;

	//Write current line_num, LABEL, OPCODE, OPERAND to intermediate.txt
	fprintf(pFile_intermediate, "%d\n", line_num);
	fprintf(pFile_intermediate, "%s\n", agv1);
	fprintf(pFile_intermediate, "%s\n", agv2);
	fprintf(pFile_intermediate, "%s\n", agv3);
	fprintf(pFile_intermediate, "%04X\n", PC);
	fprintf(pFile_intermediate, "%04X\n", LOCCTR);


	//Read lines until OPCODE part == "END"	
	while(1) {
		fgets(input_line, sizeof(input_line), pFile_input);
		if(feof(pFile_input))	
			break;

		line_num+=5;	//increase line_num by 5 after reading new line from input file
		input_line[strlen(input_line)-1]='\0';
		tokenize(agv1, agv2, agv3, input_line);
		if(strcmp(agv2, "END")==0)
			break;

		if(agv1[0]!='.') {	//if this is not a comment line
			if(agv1[0]!='!') {	//if there is a symbol in the LABEL field
				if(search(symbolTable.top, agv1)) {	//begin search symbol table for LABEL and if found, set error flag and return 0
					symbolTable.flag=line_num;
					strcpy(symbolTable.error_message, "Error! duplicate symbol");
					fclose(pFile_input);
					fclose(pFile_intermediate);
					tree_cleaner(symbolTable.top);
					symbolTable.top=NULL;
					remove("intermediate.txt");
					printf("line %d : %s\n", symbolTable.flag, symbolTable.error_message);
					return 0;
				}
				else	//if there is no LABEL in symbol field, then insert (LABEL, LOCCTR) into symbol table  
					insert_node(&(symbolTable.top), LOCCTR, agv1);				
			}


			//Some opcode contains speical character such as '+' indicate special format.
			//Extract pure opcode parts from agv2
			char op[10];
			for(int i=0; i<10; i++)
				op[i]='\0';

			if(agv2[0]=='+') {
				int i;
				for(i=1; agv2[i]!='\0'; i++)
					op[i-1]=agv2[i];
				op[i]='\0';
			}
			else
				strcpy(op, agv2);
			int index=hashing(op);
			hashPointer temp=hashTable[index];

			while(temp!=NULL) {	//if there is a op in opcode table
				if(strcmp(temp->op, op)==0) {
					if(strcmp(temp->type, "1")==0)	//if op is format 1, then increase LOCCTR by 1	 
						LOCCTR+=1;
					else if(strcmp(temp->type, "2")==0)	//if op is format 2, then increase LOCCTR by 2
						LOCCTR+=2;
					else {
						if(agv2[0]!='+')	//if op is format 3, then increase LOCCTR by 3
							LOCCTR+=3;
						else 	//if op is format 4, then increase LOCCTR by 4
							LOCCTR+=4;
					}
					break;
				}
				temp=temp->next;
			}

			if(temp==NULL && agv2[0]!='!') {	//if there is no op in opcode table
				if(strcmp(agv2, "WORD")==0)	//if agv2=="WORD", then increase LOCCTR by 3
					LOCCTR+=3;
				else if(strcmp(agv2, "RESW")==0) {	//if agv2=="RESW", then increase LOCCTR by 3*OPERAND
					int size=parseNum(agv3, 0, strlen(agv3)-1, 10);
					LOCCTR+=(3*size);
				}
				else if(strcmp(agv2, "RESB")==0)	//if agv2=="RESB", then increase LOCCTR by OPERAND
					LOCCTR+=parseNum(agv3, 0, strlen(agv3)-1, 10);
				else if(strcmp(agv2, "BYTE")==0) {	//if agv2=="BYTE", then increase LOCCTR by length of constant in bytes
					if(agv3[0]=='C')
						LOCCTR+=(strlen(agv3)-3);
					else
						LOCCTR+=((strlen(agv3)-3)/2);
				}
				else if(strcmp(agv2, "BASE")!=0 && strcmp(agv2, "NOBASE")!=0){	//if agv2 is neither strings previously nor assembly directives, set error flag and return 0
					printf("agv2 ???: %s\n", agv2);
					printf("strlen(agv2)? : %d\n", (int)strlen(agv2));
					print_symbol(symbolTable.top);
					symbolTable.flag=line_num;
					strcpy(symbolTable.error_message, "Error! invalid operation code");
					fclose(pFile_input);
					fclose(pFile_intermediate);
					tree_cleaner(symbolTable.top);
					symbolTable.top=NULL;
					remove("intermediate.txt");
					printf("line %d : %s\n", symbolTable.flag, symbolTable.error_message);
					return 0;
				}
			}
		}
		else {	//if input_line is comment line, then just copy input line to agv1
			if(agv2[0]!='!') {
				strcpy(agv1, input_line);
				agv2[0]='!';
				agv2[1]='\0';
				agv3[0]='!';
				agv3[1]='\0';
			}
		}

		//Write information generated from algorithm to "intermediate.txt"
		fprintf(pFile_intermediate, "%d\n", line_num);
		fprintf(pFile_intermediate, "%s\n", agv1);
		fprintf(pFile_intermediate, "%s\n", agv2);
		fprintf(pFile_intermediate, "%s\n", agv3);
		fprintf(pFile_intermediate, "%04X\n", PC);
		fprintf(pFile_intermediate, "%04X\n", LOCCTR);
		PC=LOCCTR;
	}

	program_lines=(line_num-10)/5;
	fprintf(pFile_intermediate, "%s\n", input_line);
	symbolTable.program_length=LOCCTR-symbolTable.starting_address;
	fclose(pFile_input);
	fclose(pFile_intermediate);
	return 1;
}


//pass2 algorithm of function "assemble" returns 1 if function ends successfully, otherwise returns 0
int pass2(char filename[]) {
	FILE *pFile_lst;	//file pointer of .lst file
	FILE *pFile_obj;	//file pointer of .obj file
	FILE *pFile_intermediate;	//file pointer of intermediate.txt
	
	int line_num=0;	
	char comment[100];
	char label[100];
	char opcode[10];
	char operand[90];
	int LOCCTR;
	int BASE;
	int PC;

	int start_LOCCTR=-1;

	unsigned int instruction_code;
	char instruction_str[20];

	int* modification_LOC=(int*)malloc(sizeof(int)*program_lines);	//stores LOC value where modification happens
	int modification_index=0;

	char text_record[70];	//character array that is written to .obj file
	text_record[0]='T';
	int	text_record_length=0;
	int text_record_index=1;
	
	char filename_lst[30];	//filename.lst
	char filename_obj[30];	//filename.obj

	int length=strlen(filename);

	strcpy(filename_lst, filename);
	filename_lst[length-3]='l';
	filename_lst[length-2]='s';
	filename_lst[length-1]='t';

	strcpy(filename_obj, filename);
	filename_obj[length-3]='o';
	filename_obj[length-2]='b';
	filename_obj[length-1]='j';
	
	pFile_lst=fopen(filename_lst, "w");
	pFile_obj=fopen(filename_obj, "w");
	pFile_intermediate=fopen("intermediate.txt", "r");

	//Read first 6 elements from "intermediate.txt"
	fscanf(pFile_intermediate, "%d", &line_num);
	fscanf(pFile_intermediate, "%s", label);
	fscanf(pFile_intermediate, "%s", opcode);
	fscanf(pFile_intermediate, "%s", operand);
	fscanf(pFile_intermediate, "%X", &LOCCTR);
	fscanf(pFile_intermediate, "%X", &PC);

	if(strcmp(opcode, "START")==0) {	//if OPCODE=="START"
		fprintf(pFile_lst, "%-6d\t%04X\t%-6s\t\t%-8s\t%s\n", line_num, LOCCTR, label, opcode, operand);	//write listing line
		fprintf(pFile_obj, "H%-6s%06X%06X\n", label, symbolTable.starting_address, symbolTable.program_length);	//write first oine of .obj file

		//Read next 6 elements from "intermediate.txt"
		fscanf(pFile_intermediate, "%d", &line_num);
		fscanf(pFile_intermediate, "%s", label);
		if(label[0]=='.') {
			fgets(comment, sizeof(comment), pFile_intermediate);
			comment[strlen(comment)-1]='\0';
		}
		fscanf(pFile_intermediate, "%s", opcode);
		fscanf(pFile_intermediate, "%s", operand);
		fscanf(pFile_intermediate, "%X", &LOCCTR);
		fscanf(pFile_intermediate, "%X", &PC);
	}

	while(strcmp(label, "END")!=0) {	//Read until label=="END"
		instruction_str[0]='\0';
		if(strcmp(opcode, "BASE")==0) {	//if opcode part=="BASE", tell assembler the value of OPERAND, print out message if error exists
			treePointer temp=search(symbolTable.top, operand);
			if(temp==NULL) {
				symbolTable.flag=line_num;
				strcpy(symbolTable.error_message, "Error! undefined symbol for BASE");
				fclose(pFile_lst);
				fclose(pFile_obj);
				fclose(pFile_intermediate);
				tree_cleaner(symbolTable.top);
				symbolTable.top=NULL;
				free(modification_LOC);
				remove("intermediate.txt");
				remove(filename_obj);
				remove(filename_lst);
				printf("line %d : %s\n", symbolTable.flag, symbolTable.error_message);
				return 0;
			}

			BASE=temp->LOCCTR;
			instruction_str[0]='\0';
		}

		//if opcode=="RESW" or opcode=="RESB", obj file must be written in next line
		if((strcmp(opcode, "RESW")==0 || strcmp(opcode, "RESB")==0) && text_record_length!=0) {
			char text_length_str[3];
			numChange(text_length_str, text_record_length/2, 2);
			text_record[7]=text_length_str[0];
			text_record[8]=text_length_str[1];

			fprintf(pFile_obj, "%s\n", text_record);

			text_record_index=9;
			text_record_length =0 ;
			text_record[text_record_index]='\0';
		}


		instruction_code=0;	//initialize instruction code(opcode)
		int format=0;	//initialize format of opcode
		int flag_x=0;	//variable that stores data whether the opcode uses X ((i.e.)"BUFFER, X")
		char mode='0';	//variable that stores addressing mode

		//if this is not a comment line
		if(label[0]!='.') {
			if(opcode[0]=='+') {	//if opcode part starts with '+' this means the opcode is format 4.
				format=4;
				for(int i=1; opcode[i]!='\0'; i++)	//rearrange character string to extract opcode part
					opcode[i-1]=opcode[i];
				opcode[strlen(opcode)-1]='\0';
			}
			int index=hashing(opcode);
			hashPointer temp=hashTable[index];
			while(temp!=NULL) {	//Search until there is a match between opcode and operation table(hashtable) element
				if(strcmp(opcode, temp->op)==0)
					break;
				temp=temp->next;
			}
			if(temp!=NULL) {	//if there is a opcode in operation table
				if(strcmp(temp->type, "1")==0) {	//if opcode is format 1
					format=1;
					instruction_code=temp->opcode;
				}
				else if(strcmp(temp->type, "2")==0) {	//if opcode is format 2
					format=2;
					instruction_code=temp->opcode<<8;
				}
				else {
					if(format==0) {	//if opcode is format 3
						instruction_code=temp->opcode<<16;
						format=3;
					}
					else	//if opcode is format 4
						instruction_code=temp->opcode<<24;
				}

				//checks for symbol existence in OPERAND field
				int flag=1;	//flag that shows whether OPERAND part is "symbol". 1 if symbol, otherwise 0
				int length=strlen(operand);
				if(operand[0]=='#' || operand[0]=='@') {	//set addressing mode and rearrange character array 'operand'
					mode=operand[0];
					for(int i=0; i<length; i++)
						operand[i]=operand[i+1];
				}

				if(operand[length-2]==',' && operand[length-1]=='X') {	//set register X and rearrange character array 'operand'
					mode='0';
					operand[length-2]='\0';
					flag_x=1;
				}

				length=strlen(operand);
				for(int i=0; i<length; i++) {
					if((operand[i]<'0' || operand[i]>'9') && (operand[i]<'a' || operand[i]>'z') && (operand[i]<'A' || operand[i]>'Z')) {
						flag*=0;
						break;
					}
				}

				if(flag && (format==3 || format==4)) {	//if there is a symbol in OPERAND field
					if(search(symbolTable.top, operand)) {	//search symbol table for OPERAND
						if(format==4) {	//if format==4, just store symbol value to operand address
							instruction_code |= (49<<20);
							instruction_code |= search(symbolTable.top, operand)->LOCCTR;
							modification_LOC[modification_index++]=LOCCTR+1;


							//setting n, i in nixbpe part of opcode
							if(mode=='0')	//n==1, i==1(simple addressing)
								instruction_code |= 3<<24;
							else if(mode=='#')	//n==0, i==1(immediate addressing)
								instruction_code |= 1<<24;
							else	//n==1, i==0(indirect addressing)
								instruction_code |= 2<<24;
				
							if(flag_x)	//x=1(indexed addressing mode)
								instruction_code |= (1<<23);
						}
						else {	//if format==3, update address part of operand according to addressing mode. Use base addressing if PC addressing is not availabe
							int destination=search(symbolTable.top, operand)->LOCCTR;
							int source=PC;
							int displacement = destination-source;
							displacement &= ((1<<12)-1);

							if(-2048<=destination-source&&destination-source<=2047) {	//Using PC relative
								instruction_code |= (1<<13);
								instruction_code |= displacement;
							}
							else {	//Using BASE relative
								instruction_code |= (1<<14);
								displacement=destination-BASE;
								displacement &= ((1<<12)-1);
								instruction_code |= displacement;
							}
								
							//setting n, i in nixbpe part of opcode
							if(mode=='0')	//n==1, i==1(simple addressing)
								instruction_code |= 3<<16;
							else if(mode=='#')	//n==0, i==1(immediate addressing)
								instruction_code |= 1<<16;
							else	//n==1, i==0(indirect addressing)
								instruction_code |= 2<<16;
				
							if(flag_x)	//x=1(indexed addressing mode)
								instruction_code |= (1<<15);
						}

						//Change instruction(unsigned integer) to charcter array
						numChange(instruction_str, instruction_code, format*2);
					}


					else {	//if there is no symbol for OPERAND
						int num=parseNum(operand, 0, strlen(operand)-1, 10);
						if(num!=-1) {	//if operand part is just a number
							instruction_code |= num;
							if(format==4) {
								instruction_code |= 1<<24;
								instruction_code |= 1<<20;
							}
							else
								instruction_code |= 1<<16;

							if(flag_x)
								instruction_code |= (1<<15);
							numChange(instruction_str, instruction_code, format*2);
						}

						else if(num==-1 && (format==3 || format==4)) {	//if operand part is neither operand nor number(error)
							symbolTable.flag=line_num;
							strcpy(symbolTable.error_message, "Error! undefined symbol");
							fclose(pFile_lst);
							fclose(pFile_obj);
							fclose(pFile_intermediate);
							tree_cleaner(symbolTable.top);
							symbolTable.top=NULL;
							free(modification_LOC);
							remove("intermediate.txt");
							remove(filename_obj);
							remove(filename_lst);
							printf("line %d : %s\n", symbolTable.flag, symbolTable.error_message);
							return 0;
						}
					}
				}

				if(operand[0]=='!') {	//if there is no operand, just operand is 0 (just convert instruction code)
					if(format==4)
						instruction_code |= (49<<20);
					else if(format==3)
						instruction_code |= (48<<12);
					numChange(instruction_str, instruction_code, format*2);
				}

				else if(format==2) {	//if opcode is format 2, make opcode according to register it uses
					if(operand[0]=='X')
						instruction_code |= 1<<4;
					else if(operand[0]=='L')
						instruction_code |= 2<<4;
					else if(operand[0]=='B')
						instruction_code |= 3<<4;
					else if(operand[0]=='S')
						instruction_code |= 4<<4;
					else if(operand[0]=='T')
						instruction_code |= 5<<4;
					else if(operand[0]=='F')
						instruction_code |= 6<<4;

					if(operand[1]==',') {
						if(operand[2]=='X')
							instruction_code |= 1;
						else if(operand[2]=='L')
							instruction_code |= 2;
						else if(operand[2]=='B')
							instruction_code |= 3;
						else if(operand[2]=='S')
							instruction_code |= 4;
						else if(operand[2]=='T')
							instruction_code |= 5;
						else if(operand[2]=='F')
							instruction_code |= 6;
					}
					numChange(instruction_str, instruction_code, format*2);
				}

			}
			else if(strcmp(opcode, "BYTE")==0 || strcmp(opcode, "WORD")) {	//if OPCODE=="BYTE" or OPCODE=="WORD"
				if(!strcmp(opcode, "WORD")) {
					instruction_code=0;
					instruction_code |= parseNum(operand, 0, strlen(operand)-1, 16);
					numChange(instruction_str, instruction_code, strlen(operand));
				}
				else if(operand[0]=='C') {
					instruction_code=0;
					int length=strlen(operand);
					for(int i=2; i<length-1; i++) {
						instruction_code+=(int)operand[i];
						instruction_code<<=8;						
					}
					instruction_code>>=8;
					numChange(instruction_str, instruction_code, 2*(length-3));
				}

				else if(operand[0]=='X') {
					instruction_code = parseNum(operand, 2, strlen(operand)-2, 16);
					numChange(instruction_str, instruction_code, strlen(operand)-3);
				}
			}

		}

		//Below part write lst file.
		fprintf(pFile_lst, "%-6d\t", line_num);
		if(label[0]=='.') {	//if this is a comment line
			fprintf(pFile_lst, "%4s\t", "    ");
			if(strlen(comment)==0)
				fprintf(pFile_lst, "%c", label[0]);
			else {
				fprintf(pFile_lst, "%c    \t\t", label[0]);
				int start=0;
				while(comment[start]==' ')
					start++;
				int length=strlen(comment);
				for(int i=0; comment[i+start]!='\0'; i++)
					comment[i] = comment[i+start];
				comment[length-start]='\0';
				fprintf(pFile_lst, "%s", comment);
			}
		}
		else { 	//if this is not a commment line
			fprintf(pFile_lst, "%04X\t", LOCCTR);
			if(label[0]=='!')
				fprintf(pFile_lst, "%-6s\t\t", " ");
			else
				fprintf(pFile_lst, "%-6s\t\t", label);

			if(opcode[0]=='!')
				fprintf(pFile_lst, "%-8s\t\t", "        ");
			else {
				if(format==4)
					fprintf(pFile_lst, "+%-7s\t", opcode);
				else
					fprintf(pFile_lst, "%-8s\t", opcode);
			}

			if(operand[0]=='!')
				fprintf(pFile_lst, "%-8s\t\t", "        ");
			else {
				if(flag_x) {
					int temp=strlen(operand);
					operand[temp]=',';
					operand[temp+1]=' ';
					operand[temp+2]='X';
					operand[temp+3]='\0';
				}
	
			if(mode!='0')
				fprintf(pFile_lst, "%c%-7s", mode, operand);
			else
				fprintf(pFile_lst, "%-8s", operand);

			fprintf(pFile_lst, "%s", "\t\t");
			}
		}
	
		//below part writes text_record and writes to .obj file
		if(strlen(instruction_str)) {
			if(start_LOCCTR==-1)
				start_LOCCTR=LOCCTR;
			int instruction_length=strlen(instruction_str);
			char text_length_str[3];
			fprintf(pFile_lst, "%s", instruction_str);
			if(text_record_length==0) {	//if it is first time to write text record
				char LOC[7];
				numChange(LOC, LOCCTR, 6);
				for(int i=0; i<6; i++)
					text_record[1+i]=LOC[i];
				text_record_index=9;
				for(int i=0; i<instruction_length; i++)
					text_record[text_record_index+i] = instruction_str[i];
				text_record_index += instruction_length;
				text_record_length = instruction_length;
				numChange(text_length_str, text_record_length/2, 2);
				text_record[7]=text_length_str[0];
				text_record[8]=text_length_str[1];


			}

			else {
				if(text_record_length + instruction_length <= 60) {	//if text length does not exceeds maximum
					for(int i=0; i<instruction_length; i++)
						text_record[text_record_index+i] = instruction_str[i];
					text_record_index += instruction_length;
					text_record_length += instruction_length;
					text_record[text_record_index]='\0';
					numChange(text_length_str, text_record_length/2, 2);
					text_record[7]=text_length_str[0];
					text_record[8]=text_length_str[1];


				}

				else {	//if text length exceedes maximum length (write to next line)
					numChange(text_length_str, text_record_length/2, 2);
					text_record[7]=text_length_str[0];
					text_record[8]=text_length_str[1];

					fprintf(pFile_obj, "%s\n", text_record);


					char LOC[7];
					numChange(LOC, LOCCTR, 6);
					for(int i=0; i<6; i++)
						text_record[1+i]=LOC[i];
					text_record_index=9;
					for(int i=0; i<instruction_length; i++)
						text_record[text_record_index+i] = instruction_str[i];
					text_record_index += instruction_length;
					text_record_length = instruction_length;
					text_record[text_record_index]='\0';

				}	

			}

		}
		fprintf(pFile_lst, "%c", '\n');

		fscanf(pFile_intermediate, "%d", &line_num);
		fscanf(pFile_intermediate, "%s", label);
		if(label[0]=='.') {
			fgets(comment, sizeof(comment), pFile_intermediate);
			comment[strlen(comment)-1]='\0';
		}
		fscanf(pFile_intermediate, "%s", opcode);
		fscanf(pFile_intermediate, "%s", operand);
		fscanf(pFile_intermediate, "%X", &LOCCTR);
		fscanf(pFile_intermediate, "%X", &PC);

	}
	
	fprintf(pFile_obj, "%s\n", text_record);
	for(int i=0; i<modification_index; i++)
		fprintf(pFile_obj, "M%06X05\n", modification_LOC[i]);
	fprintf(pFile_obj, "E%06X\n", start_LOCCTR); 
	fprintf(pFile_lst, "%-6d\t%s\t%-6s\t\t%-8s\t%s\n", line_num+5, "    ", "      ", label, opcode); 

	remove("intermediate.txt");
	free(modification_LOC);
	fclose(pFile_lst);
	fclose(pFile_obj);
	fclose(pFile_intermediate);

	return 1;
}

void assemble(char* filename, char* cmd) {
	char extension[]=".asm";
	int flag=1;
	FILE* fp;
	int filename_length=strlen(filename);

	//Checks if file exists
	fp=fopen(filename, "r");
	if(!fp) {
		printf("Error! File does not exist\n");
		return;
	}

	//Checks if filename ends with ".asm"
	for(int i=0; i<4; i++)
		if(filename[filename_length-4+i]!=extension[i])
			flag*=0;

	if(flag) {	//Case that filename ends with ".asm"
		if(pass1(filename)) {
			if(pass2(filename)) {
				printf("\033[0;32mSuccessfully\033[0m assemble %s\n", filename);
				if(temp_symbol!=NULL) {
					tree_cleaner(temp_symbol);
					temp_symbol=NULL;
				}
				temp_symbol=symbolTable.top;			
				symbolTable.top=NULL;
				insert_command(cmd);
			}
		}
	}
	else	//Case that filename does not end with ".asm"
		printf("Error! File extension format is not .asm\n");
	
	fclose(fp);
}

int loader_pass1(void) {
	int csaddr=start_progaddr;
	file_listPointer temp=first_file;


	int idx=0;
	while(temp!=NULL) {
		FILE* fp;
		fp = fopen(temp->filename, "r");
		//if file does not exist, deallocate related data and return 0
		if(fp==NULL) {
			printf("file does not exist!\n");
			hashTable2_cleaner();
			file_list_cleaner();
			return 0;
		}
		char file_str[100];
		fgets(file_str, sizeof(file_str), fp);
		file_str[strlen(file_str)-1]='\0';

		char control_section_name[10];
		int i;
		for(i=1; i<=6 && file_str[i]!=' '; i++)
			control_section_name[i-1]=file_str[i];
		while(control_section_name[i]==' ')
			i--;
		control_section_name[i]='\0';

		char cslth_str[7];
		for(i=13; i<=18; i++)
			cslth_str[i-13]=file_str[i];
		cslth_str[6]='\0';
		int cslth=parseNum(cslth_str, 0, 5, 16);

		//search ESTAB for control section name. If found, then set error flag and return 0
		for(i=0; i<idx; i++) {
			if(strcmp(control_section_name, ESTable[i]->control_section)==0) {
				printf("Error! duplicate external symbol: %s\n", control_section_name);
				fclose(fp);
				hashTable2_cleaner();
				file_list_cleaner();
				return 0;
			}
		}

		//if control section name not found, insert to ESTable
		hashPointer2 est = (hashPointer2)malloc(sizeof(struct hashNode2));
		est->next=NULL;
		strcpy(est->control_section, control_section_name);
		strcpy(est->symbol_name, control_section_name);
		est->address=csaddr;
		est->program_length=cslth;
		ESTable[idx]=est;


		while(file_str[0]!='E') {
			fgets(file_str, sizeof(file_str), fp);
			if(file_str[0]=='E')
				break;
			file_str[strlen(file_str)-1]='\0';


			//If there is a definition record, for each symbol search for symbol table. if found, set error flag otherwise insert to ESTable
			if(file_str[0]=='D') {
				int start_idx=1;
				//parsing symbol from D line
				while(file_str[start_idx]!='\0') {
					char symbol_name[7];
					int i;
					for(i=0; i<6 && file_str[start_idx+i]!=' '; i++)
						symbol_name[i]=file_str[start_idx+i];
					symbol_name[i]='\0';

					start_idx+=6;

					//search for symbol in ESTAB
					hashPointer2 est;
					for(int i=0; i<=idx; i++) {
						est = ESTable[i];
						//case that symbol found in ESTAB
						while(est!=NULL) {
							if(strcmp(symbol_name, est->symbol_name)==0) {
								printf("Error! duplicate external symbol!: %s\n", symbol_name);
								fclose(fp);
								hashTable2_cleaner();
								file_list_cleaner();
								return 0;
							}
							else
								est = est->next;
						}
					}


					//if symbol not found, insert into ESTable
					if(est==NULL) {
						char address_str[7];
						for(int i=0; i<6; i++)
							address_str[i]=file_str[start_idx++];
						address_str[6]='\0';
						int address=parseNum(address_str, 0, 5, 16);

						hashPointer2 new_est = (hashPointer2)malloc(sizeof(struct hashNode2));
						new_est->next=NULL;
						strcpy(new_est->control_section, ESTable[idx]->control_section);
						strcpy(new_est->symbol_name, symbol_name);
						new_est->address = address + csaddr;
						new_est->program_length=0;

						hashPointer2 trace = ESTable[idx];
						while(trace->next!=NULL)
							trace = trace->next;
						trace->next = new_est;
					}
				}
			}
		}


		fclose(fp);
		temp=temp->link;
		csaddr+=cslth;
		idx++;	

	}

	//return 1 : indicates pass1 algorithm successfully ended
	return 1;
}

int loader_pass2(void) {
	//set CSADDR, EXECADDR to PROGADDR
	int csaddr=start_progaddr;
	int execaddr=start_progaddr;

	int idx=0;
	//temp_memory is used to store previous data in memory. If pass2 algorithm fails, recover data from temp_memory.
	unsigned char temp_memory[0xFFFFF];
	for(int i=0; i<0xFFFFF; i++)
		temp_memory[i]=memory[i];

	file_listPointer temp_file=first_file;

	//open file and read data one-by-one
	while(temp_file!=NULL) {
		FILE *fp;
		fp = fopen(temp_file->filename, "r");
		
		Reference r[3];	//used to implement Reference number(i.e. LISTA->01)
		char file_str[100];

		fgets(file_str, sizeof(file_str), fp);
		int cslth=ESTable[idx]->program_length;
		csaddr=ESTable[idx]->address;
		strcpy(r[0].symbol, ESTable[idx]->control_section);
		r[0].used_flag=ESTable[idx]->address;

		while(file_str[0]!='E') {
			fgets(file_str, sizeof(file_str), fp);
			if(file_str[0]=='E')
				break;
			if(file_str[0]=='R') {
				int start=1;
				//parsing reference data from file. initialize external-reference symbols
				while(start<strlen(file_str) && file_str[start]!='\n' && file_str[start]!='\0') {
					char symbol_str[7];
					int symbol_idx=parseNum(file_str, start, start+1, 10);
					start+=2;
					int i;

					for(i=0; i<6 && file_str[start]!='\n'; i++)
						symbol_str[i] = file_str[start++];

					while(symbol_str[i-1]==' ' || symbol_str[i-1]=='\n')
						i--;

					symbol_str[i]='\0';
					strcpy(r[symbol_idx-1].symbol, symbol_str);
					r[symbol_idx-1].used_flag=-1;

				}
			}

			//if object code is in character form, convert into internal representation
			else if(file_str[0]=='T') {
				file_str[strlen(file_str)-1]='\0';
				//parsing from obj file
				int text_length=parseNum(file_str, 7, 8, 16);
				int specified_addr=parseNum(file_str, 1, 6, 16);
				int addr_trace=0;
				for(int i=0; i<text_length; i++) {
					int value=parseNum(file_str, 9+2*i, 9+2*i+1, 16);

					//load data to memory
					memory[csaddr+specified_addr+addr_trace]=value;
					addr_trace++;
				}
			}

			//search ESTAB for modifying symbol name
			else if(file_str[0]=='M') {
				int index=0;
				while(file_str[index]!='+' && file_str[index]!='-')
					index++;
				int symbol_idx=parseNum(file_str, index+1, index+2, 10);

				//search for symbol
				//flag==-1 in this part means symbol is not yet searched
				if(r[symbol_idx-1].used_flag==-1) {
					hashPointer2 est = ESTable[0];
					for(int i=0; ESTable[i]!=NULL; i++) {
						est = ESTable[i];
						while(est!=NULL) {

							if((strcmp(r[symbol_idx-1].symbol, est->symbol_name)==0)) {
								r[symbol_idx-1].used_flag=est->address;
								int location=parseNum(file_str, 1, 6, 16);
								location += csaddr;
								int modification_amount=parseNum(file_str, 7, 8, 10);
								int addr_trace=location;

								int original_data=0;
								for(int i=0; i<(modification_amount+1)/2; i++) {
									original_data+=memory[location+i];
									original_data*=(16*16);
								}
								original_data/=(16*16);
								//case that modification is for 5-half byte
								if(modification_amount==5) {
									if(file_str[index]=='+')
										original_data+=(r[symbol_idx-1].used_flag);
									else {
										int com=r[symbol_idx-1].used_flag;
										com = ~com;
										com +=1;
										com &= (1<<20)-1;
										com += (original_data & ((1<<20)-1));
										com &= (1<<20)-1;
										original_data &= ((1<<24)-1 - ((1<<20)-1));
										original_data+=com;

									}
								}

								//case that modification is for 6-half byte
								else if(modification_amount==6) {
									if(file_str[index]=='+')
										original_data+=(r[symbol_idx-1].used_flag);
									else {
										int com=r[symbol_idx-1].used_flag;
										com = ~com;
										com +=1;
										com &= (1<<24)-1;
										original_data+=com;
									}
								}

								memory[addr_trace++]=((original_data>>16)&((1<<8)-1));
								memory[addr_trace++]=((original_data>>8)&((1<<8)-1));
								memory[addr_trace++]=original_data%256;								
								break;
							}
							else
								est = est->next;
						}
						//match between symbol and reference number
						if(r[symbol_idx-1].used_flag!=-1)
							break;
					}


					//flag==-1 in this part means that external symbol is not found
					if(r[symbol_idx-1].used_flag==-1) {
						printf("file?: %s\n", temp_file->filename);
						printf("Error! undefined external symbol!: %s\n", r[symbol_idx-1].symbol);
						for(int i=0; i<0xFFFFF; i++)
							memory[i]=temp_memory[i];
						fclose(fp);
						hashTable2_cleaner();
						file_list_cleaner();
						return 0;
					}
				}
			
				//add or subtract symbol value at location
				else {
					int location=parseNum(file_str, 1, 6, 16);
					location += csaddr;
					int modification_amount=parseNum(file_str, 7, 8, 10);
					int addr_trace=location;

					int original_data=0;
					for(int i=0; i<(modification_amount+1)/2; i++) {
						original_data+=memory[location+i];
						original_data*=(256);
					}
					original_data/=256;
					//case that modification is for 5-half bytes
					if(modification_amount==5) {
						if(file_str[index]=='+')
							original_data+=(r[symbol_idx-1].used_flag);
						else {
							int com=r[symbol_idx-1].used_flag;
							com = ~com;
							com +=1;
							com &= (1<<20)-1;
							com += (original_data & ((1<<20)-1));
							com &= (1<<20)-1;
							original_data &= ((1<<24)-1   - ((1<<20)-1));
							original_data|=com;
						}
					}
					//case that modification is for 6-half bytes
					else if(modification_amount==6) {
						if(file_str[index]=='+')
							original_data+=(r[symbol_idx-1].used_flag);
						else {
							int com=r[symbol_idx-1].used_flag;
							com = ~com;
							com +=1;
							com &= (1<<24) -1;
							original_data +=com;
						}
					}

					memory[addr_trace++]=((original_data>>16)&((1<<8)-1));
					memory[addr_trace++]=((original_data>>8)&((1<<8)-1));
					memory[addr_trace++]=original_data%256;			
				}
			}
		
		}
		//if an address is specified in End record, then set EXECADDR to CSADDR + specified address
		if(file_str[0]=='E'&&(strlen(file_str)!=1)) 
			execaddr=parseNum(file_str, 1, 6, 16) + csaddr;

		csaddr+=cslth;
		temp_file = temp_file->link;
		idx++;
	}

	start_progaddr=execaddr;

	//loader 2 function successfully completed : return 1
	return 1;
}

void loader(char* cmd) {
	//first call loader_pass1 function. if succeded, then call loader_pass2 function
	if(loader_pass1()) {
		if(loader_pass2()) {
			//Case that successfully called loader_pass1, loader_pass2 function. print out related information (load map)
			printf("%s\t%s\t%s\t%s\n", "control", "symbol", "address", "length");
			printf("%s\t%s\n", "section", "name");
			printf("---------------------------------\n");

			for(int i=0; i<10 && ESTable[i]!=NULL; i++) {
				printf("%s\t%s\t%04X\t%04X\n", ESTable[i]->control_section, "\0", ESTable[i]->address, ESTable[i]->program_length);
				hashPointer2 temp=ESTable[i]->next;
				while(temp!=NULL) {
					printf("\t%s\t%04X\t\n", temp->symbol_name, temp->address);
					temp = temp->next;
				}
			}

			total_length=0;
			for(int i=0; i<10 && ESTable[i]!=NULL; i++)
				total_length+=ESTable[i]->program_length;
			printf("---------------------------------\n");
			printf("\ttotal length %04X\n\n", total_length);

			A=0; X=0; L=total_length; PC=start_progaddr; B=0; S=0; T=0;

			//free unused data (data that will not be used after loading)
			hashTable2_cleaner();
			file_list_cleaner();
			insert_command(cmd);
			length=L;
		}
	}
}

void run(void) {

	while(PC<start_progaddr+length) {

		//value for break point
		int temp_A=A; 
		int temp_X=X;
		int temp_L=L;
		int temp_PC=PC;
		int temp_B=B;
		int temp_S=S;
		int temp_T=T;

		int opcode=memory[PC];

		//operation "CLEAR r1"
		if(opcode==0xB4) {
			PC += 2;
			int operand=memory[PC-1];
			operand>>=4;

			if(operand==0)
				A=0;
			else if(operand==1)
				X=0;
			else if(operand==2)
				L=0;
			else if(operand==3)
				B=0;
			else if(operand==4)
				S=0;
			else if(operand==5)
				T=0;
			else if(operand==8)
				PC=0;

		}

		//operation "COMP m"
		else if(opcode==0xA0) {
			PC += 2;
			int operand1=memory[PC-1];
			operand1>>=4;
			int operand2=memory[PC-1];
			operand2 &= ((1<<4)-1);

			if(operand1==0)
				operand1=A;
			else if(operand1==1)
				operand1=X;
			else if(operand1==2)
				operand1=L;
			else if(operand1==3)
				operand1=B;
			else if(operand1==4)
				operand1=S;
			else if(operand1==5)
				operand1=T;
			else if(operand1==8)
				operand1=PC;

			if(operand2==0)
				operand2=A;
			else if(operand2==1)
				operand2=X;
			else if(operand2==2)
				operand2=L;
			else if(operand2==3)
				operand2=B;
			else if(operand2==4)
				operand2=S;
			else if(operand2==5)
				operand2=T;
			else if(operand2==8)
				operand2=PC;

			if(operand1==operand2)
				CC='=';
			else if(operand1>operand2)
				CC='>';
			else
				CC='<';

		}

		//operation "TIXR r1"
		else if(opcode==0xB8) {
			PC += 2;

			int operand=memory[locctr+1];
			operand>>=4;

			if(operand==0)
				operand=A;
			else if(operand==1)
				operand=X;
			else if(operand==2)
				operand=L;
			else if(operand==3)
				operand=B;
			else if(operand==4)
				operand=S;
			else if(operand==5)
				operand=T;
			else if(operand==8)
				operand=PC;

			X++;

			if(X==operand)
				CC='=';
			else if(X<operand)
				CC='<';
			else
				CC='>';

		}

		//foramt3 or foramt4 operation
		else {
			opcode&=((1<<8)-1) - ((1<<2)-1);

			int nixbpe=memory[locctr];
			nixbpe&=((1<<2)-1);
			nixbpe<<=8;
			nixbpe += memory[locctr+1];
			nixbpe>>=4;

			int nixbpe_arr[6];
			for(int i=5; i>=0; i--) {
				nixbpe_arr[i]=nixbpe&1;
				nixbpe>>=1;
			}


			int disp_address;
			//format 4
			if(nixbpe_arr[5]==1) {
				PC += 4;
				disp_address=memory[locctr+1];
				disp_address<<=8;
				disp_address+=memory[locctr+2];
				disp_address<<=8;
				disp_address+=memory[locctr+3];
				disp_address&=((1<<20)-1);
			}

			//foramt 3
			else {
				PC += 3;
				disp_address=memory[locctr+1];
				disp_address<<=8;
				disp_address+=memory[locctr+2];
				disp_address&=((1<<12)-1);

				if(disp_address & (1<<11))
					disp_address |= (-(1<<12));
			}
	
			//Determine using PC-relative / Base-relative
			if(nixbpe_arr[4]==1)
				disp_address+=PC;
			else if(nixbpe_arr[3]==1)
				disp_address+=B;

			//Determine using X register
			if(nixbpe_arr[2]==1)
				disp_address+=X;

			int m;

			//immediate addressing
			if(nixbpe_arr[0]==0&&nixbpe_arr[1]==1)
				m=disp_address;

			//simple addressing
			else if(nixbpe_arr[0]==1 && nixbpe_arr[1]==1)
				m=disp_address;

			//indirect addressing
			else if(nixbpe_arr[0]==1 && nixbpe_arr[1]==0) {
				int value=memory[disp_address];
				value<<=8;
				value += memory[disp_address+1];
				value<<=8;
				value += memory[disp_address+2];
				m = value;
			}

			
	

			//operation "COMP m"
			if(opcode==0x28) {
				if(A==m)
					CC='=';
				else if(A<m)
					CC='<';
				else
					CC='>';
			}

			//operation "J m"
			else if(opcode==0x3C) {
				PC=m;
				locctr=PC;
			}

			//operation "JEQ m"
			else if(opcode==0x30) {
				if(CC=='=') {
					PC=m;
					locctr=PC;
				}
			}

			//operation "JLT m"
			else if(opcode==0x38) {
				if(CC=='<') {
					PC=m;
					locctr=PC;
				}
			}

			//operation "JSUB m"
			else if(opcode==0x48) {
				L=PC;
				PC=m;
			}

			//operation "LDA m"
			else if(opcode==0x00) {
				if(nixbpe_arr[0]==1 && nixbpe_arr[1]==1) {
					A=memory[m];
					A<<=8;
					A+=memory[m+1];
					A<<=8;
					A+=memory[m+2];
				}

				else if(nixbpe_arr[0]==0 && nixbpe_arr[1]==1)
					A=m;

			}

			//operation "LDB m"
			else if(opcode==0x68) {
				B=m;
			}

			//operation "LDCH m"
			else if(opcode==0x50) {
				A=memory[m];
			}

			//operation "LDT m"
			else if(opcode==0x74) {
				if(nixbpe_arr[0]==1 && nixbpe_arr[1]==1) {
					T=memory[m];
					T<<=8;
					T+=memory[m+1];
					T<<=8;
					T+=memory[m+2];
				}

				else if(nixbpe_arr[0]==0 && nixbpe_arr[1]==1)
					T=m;
			}

			//operation "RD m"
			else if(opcode==0xD8) {
				A=0;
			}

			//operation "RSUB"
			else if(opcode==0x4C) {
				PC=L;
			}

			//operation "STA m"
			else if(opcode==0x0C) {
				int temp=A;
				char value3=temp&((1<<8)-1);
				temp>>=8;
				char value2=temp&((1<<8)-1);
				temp>>=8;
				char value1=temp&((1<<8)-1);

				memory[m] = value1;
				memory[m+1] = value2;
				memory[m+2] = value3;
			}

			//operation "STCH m"
			else if(opcode==0x54) {
				memory[m]=(A&((1<<8)-1));
			}


			//operation "STL m"
			else if(opcode==0x14) {
				int temp=L;
				char value3=temp&((1<<8)-1);
				temp>>=8;
				char value2=temp&((1<<8)-1);
				temp>>=8;
				char value1=temp&((1<<8)-1);

				memory[m]=value1;
				memory[m+1]=value2;
				memory[m+2]=value3;
			}


			//operation "STX m"
			else if(opcode==0x10) {
				int temp=X;
				char value3=temp&((1<<8)-1);
				temp>>=8;
				char value2=temp&((1<<8)-1);
				temp>>=8;
				char value1=temp&((1<<8)-1);

				memory[m]=value1;
				memory[m+1]=value2;
				memory[m+2]=value3;

			}

			//operation "TD m"
			else if(opcode==0xE0) {
				CC='<';
			}
		}
		locctr=PC;

		//if there is a break point, print out inforamtion and return
		if(bp_status[temp_PC]==1) {
			printf("A : %06X\t X : %06X\n", temp_A, temp_X);
			printf("L : %06X\tPC : %06X\n", temp_L, temp_PC);
			printf("B : %06X\t S : %06X\n", temp_B, temp_S);
			printf("T : %06X\n", temp_T);
			printf("\t\t Stop at checkpoint[%X]\n", temp_PC);
			return;
		}

	}

	//end of the program
	printf("A : %06X\t X : %06X\n", A, X);
	printf("L : %06X\tPC : %06X\n", L, PC);
	printf("B : %06X\t S : %06X\n", B, S);
	printf("T : %06X\n", T);
	printf("\t\t End Program\n");
	
	//reset value of register
	PC=start_progaddr;
	A=0;
	X=0;
	L=length;
	B=0;
	S=0;
	T=0;
}

//function that changes hex num stored in character arry in to integer
int parseNum(char *str, int start, int end, int standard) {
	//removes possible space (trimming)
	if(start>end)
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
			result *= standard;
	}
	return result;
}

void print_symbol(treePointer head) {
	if(head) {
		if(head->left)
			print_symbol(head->left);
		printf("\t%s\t%04X\n", head->label, head->LOCCTR);
		if(head->right)
			print_symbol(head->right);
	}
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
			int start=parseNum(command_trimmed, boundary, strlen(command_trimmed)-1, 16);
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
				int start=parseNum(command_trimmed, boundary, temp-1, 16);
				int end=parseNum(command_trimmed, temp+1, strlen(command_trimmed)-1, 16);
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
			int address=parseNum(command_trimmed, boundary, temp-1, 16);	//parsing address part from command
			int value=parseNum(command_trimmed, temp+1, strlen(command_trimmed)-1, 16);	//parsign value part from command
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
			int start=parseNum(command_trimmed, boundary, temp1-1, 16);	//parsing address(start) from command
			int end=parseNum(command_trimmed, temp1+1, temp2-1, 16);	//parsing address(end) from command
			int value=parseNum(command_trimmed, temp2+1, strlen(command_trimmed)-1, 16);	//parsing value from command
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
	else if(strncmp(command_trimmed, "opcode ", 7)==0) {
		char temp_op[10];
		char* temp_op_trimmed;
		int length=strlen(command_trimmed);
		for(int i=7; i<length; i++)
			temp_op[i-7]=command_trimmed[i];
		temp_op[length-7]='\0';
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

	else if(strncmp(command_trimmed, "type ", 5)==0) {
		char filename[30];
		char* filename_trimmed;	//stores filename
		int length=strlen(command_trimmed);


		for(int i=5; i<length; i++)
			filename[i-5]=command_trimmed[i];
		filename[length-5]='\0';
		filename_trimmed=trim(filename);

		DIR *pDir=NULL;
		struct dirent *pFile=NULL;
		struct stat buf;
		int flag=0;

		//return if error occurs when opening the file
		pDir=opendir(".");
		if(!pDir) {
			printf("ERROR\n");
			return;
		}


		//Read list of file one-by-one
		while((pFile=readdir(pDir))!=NULL)
		{
			//break the loop if correspoding file detected
			if(strcmp(filename_trimmed, pFile->d_name)==0) {
				flag=1;
				break;
			}
		}
		closedir(pDir);

		if(flag==0)	//Case if there is no corresponding file
			printf("Error : file does not exist!\n");
		else {
			stat(pFile->d_name, &buf);
			if(S_ISDIR(buf.st_mode))	//Case if file is directory
				printf("Error : %s is a directory!\n", filename_trimmed);
			else {	//Case that there is a corresponding file -> calls function "type"
				type(filename_trimmed);
				insert_command(command);
			}
		}

		free(filename_trimmed);
	}

	else if(strncmp(command_trimmed, "assemble ", 9)==0) {
		char filename[30];
		char* filename_trimmed;
		int length=strlen(command_trimmed);

		for(int i=9; i<length; i++)
			filename[i-9]=command_trimmed[i];
		filename[length-9]='\0';
		filename_trimmed=trim(filename);
		assemble(filename_trimmed, command);
		free(filename_trimmed);
	}

	else if(strncmp(command_trimmed, "symbol", 6)==0) {
		insert_command(command);
		if(temp_symbol)
			print_symbol(temp_symbol);
		else
			printf("Symbol Table is empty");
		puts("");
	}

	//setting starting address of program
	else if(strncmp(command_trimmed, "progaddr ", 9)==0) {
		int target_addr=parseNum(command_trimmed, 9, strlen(command_trimmed)-1, 16);
		
		//target in range
		if(0<=target_addr&&target_addr<0xFFFFF) {
			start_progaddr=target_addr;
			insert_command(command);
		}
		
		//target out of range
		else
			puts("program address out of Range!");
	}

	//loading program to progaddr
	else if(strncmp(command_trimmed, "loader ", 7)==0) {
		int length=strlen(command_trimmed);
		int boundary=7;
		char filename[20];

		int start=boundary;
		int end=boundary;

		//parsing filename from command
		while(start<length && command_trimmed[start]!='\0') {
			while(command_trimmed[start]==' ')
				start++;
			end=start;
			while(command_trimmed[end]!=' ' && command_trimmed[end]!='\0')
				end++;
			for(int i=start; i<=end; i++)
				filename[i-start]=command_trimmed[i];
			filename[end-start]='\0';
			int filename_length=strlen(filename);

			if(filename[filename_length-4]=='.' && filename[filename_length-3]=='o' && filename[filename_length-2]=='b' && filename[filename_length-1]=='j')
				insert_file(filename);
			else {
				puts("filename format Error!");
				file_list_cleaner();
				break;
			}

			start=end+1;
			for(int i=0; i<10; i++)
				filename[i]='\0';
		}
		

		//If there is file to load, call loader function. (valid command)
		if(first_file!=NULL)
			loader(command);
	}

	//clear all of the break points set
	else if(strcmp(command_trimmed, "bp clear")==0) {
		bp_cleaner();
		printf("\t\t[\033[0;32mok\033[0m] clear all breakpoints\n");
		insert_command(command);
	}


	//set bp [address]
	else if(strncmp(command_trimmed, "bp ", 3)==0) {
		int length=strlen(command_trimmed);
		int locctr=parseNum(command_trimmed, 3, length-1, 16);

		if(0<=locctr && locctr<0xFFFFF) {
			insert_bp(locctr);
			insert_command(command);
			printf("\t\t[\033[0;32mok\033[0m] create breakpoint %x\n", locctr);
			bp_status[locctr]=1;
		}
		else
			puts("Error! Breakpoint out of range");

	}

	//print out all of the break point set
	else if(strcmp(command_trimmed, "bp")==0) {
		printf("\t\tbreakpoint\n");
		printf("\t\t----------\n");
		listPointer2 temp=first_bp;
		while(temp!=NULL) {
			printf("\t\t%X\n", temp->locctr);
			temp = temp->link;
		}

		insert_command(command);

	}

	//call run function
	else if(strcmp(command_trimmed, "run")==0) {
		run();
		insert_command(command);
	}


	//case that input command is invalid
	else
		puts("Invalid Command!");

	free(command_trimmed);
}
