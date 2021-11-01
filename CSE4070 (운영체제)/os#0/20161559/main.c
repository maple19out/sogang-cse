#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "hash.h"
#include "bitmap.h"

#define MAXCMD_LEN 100
#define MAXARGV_LEN 20

//comparison function of list
bool compare_list(const struct list_elem *a, const struct list_elem *b, void *aux) {
	struct list_item *A;
	A = list_entry(a, struct list_item, elem);
	struct list_item *B;
	B = list_entry(b, struct list_item, elem);

	if(A->data < B->data)
		return true;
	else
		return false;
}

//comparison function of hash
bool compare_hash(const struct hash_elem *a, const struct hash_elem *b, void *aux) {
	struct hash_item *A;
	A = hash_entry(a, struct hash_item, elem);
	struct hash_item *B;
	B = hash_entry(b, struct hash_item, elem);

	if(A->data < B->data)
		return true;
	else
		return false;
}

//computes and returns the hash value for hash element E
unsigned hash_func(const struct hash_elem *e, void *aux) {
	struct hash_item *E;
	E = hash_entry(e, struct hash_item, elem);

	return hash_int(E->data); 
}

//hash action function : squares data of hash element E
void square_func(struct hash_elem *e, void *aux) {
	struct hash_item *item;
	item = hash_entry(e, struct hash_item, elem);

	item->data *= item->data;
}

//hash action function : triples data of hash element E
void triple_func(struct hash_elem *e, void *aux) {
	struct hash_item *item;
	item = hash_entry(e, struct hash_item, elem);

	item->data *= (item->data)*(item->data);
}

//hash action function : deallocate heap area of hash element E
void destructor_func(struct hash_elem *e, void *aux) {
	free(e);
}


/*
main function below continuously gets input from console.
After getting input command, follows procedures below.
(1) get arguments from cmd string through parsing.
(2) call related function with arguments

loop continues until command "quit" is given.
*/
int main(void) {

	char cmd[MAXCMD_LEN];

	//Required data structures in the program : List, Hash, Bitmap
	struct list list[10];
	struct hash hash[10];
	void* bm[10];

	while(1) {
		fgets(cmd, MAXCMD_LEN, stdin);
		cmd[strlen(cmd)-1]='\0';
		int length = strlen(cmd);

		if(!strcmp(cmd, "quit"))
			break;

		else if(!strncmp(cmd, "create list ", 12)) {
			char argv[MAXARGV_LEN];

			for(int i=12; i<=length; i++)
				argv[i-12] = cmd[i];
			int list_idx = argv[strlen(argv)-1]-'0';

			list_init(&list[list_idx]);
		}

		else if(!strncmp(cmd, "dumpdata ", 9)) {
			char argv[MAXARGV_LEN];

			for(int i=9; i<=length; i++)
				argv[i-9] = cmd[i];

			if(!strncmp(argv, "list", 4)) {

				int list_idx = argv[strlen(argv)-1]-'0';
				struct list_elem* temp;

				for(temp = list_begin(&list[list_idx]); temp != list_end(&list[list_idx]); temp = list_next(temp)) {
					struct list_item * item = list_entry(temp, struct list_item, elem);
					printf("%d ", item->data);
				}

				if(temp!=list_begin(&list[list_idx]))
					puts("");
			}

			else if(!strncmp(argv, "hash", 4)) {

				int hash_idx = argv[strlen(argv)-1]-'0';
				struct hash_iterator iter;
				hash_first(&iter, &hash[hash_idx]);

				while(hash_next(&iter)) {
					struct hash_item *item = hash_entry(hash_cur(&iter), struct hash_item, elem);
					printf("%d ", item->data);
				}
				if(!hash_empty(&hash[hash_idx]))
					puts("");
			}

			else if(!strncmp(argv, "bm", 2)) {
				int bitmap_idx = argv[strlen(argv)-1]-'0';

				size_t bit_cnt = bitmap_size(bm[bitmap_idx]);
				for(size_t i=0; i<bit_cnt; i++) {
					bool result = bitmap_test(bm[bitmap_idx], i);
					if(result==false)
						printf("0");
					else
						printf("1");
				}
				if(bit_cnt!=0)
					puts("");
			}
		}

		else if(!strncmp(cmd, "list_push_front ", 16)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];

			int i;
			for(i=16; cmd[i]!=' '; i++)
				argv1[i-16] = cmd[i];
			argv1[i-16]='\0';
			int list_idx = argv1[strlen(argv1)-1]-'0';
			i++;

			for(int j=i; j<=length; j++)
				argv2[j-i] = cmd[j];

			struct list_elem* target = (struct list_elem*)malloc(sizeof(struct list_elem));
			list_push_front(&list[list_idx], target);

			struct list_item * item = list_entry(target, struct list_item, elem);
			item->data = atoi(argv2);
		}

		else if(!strncmp(cmd, "list_push_back ", 15)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];

			int i;
			for(i=15; cmd[i]!=' '; i++)
				argv1[i-15] = cmd[i];
			argv1[i-15]='\0';
			int list_idx = argv1[strlen(argv1)-1]-'0';
			i++;

			for(int j=i; j<=length; j++)
				argv2[j-i] = cmd[j];

			struct list_elem* target = (struct list_elem*)malloc(sizeof(struct list_elem));
			list_push_back(&list[list_idx], target);

			struct list_item * item = list_entry(target, struct list_item, elem);
			item->data = atoi(argv2);
		}

		else if(!strncmp(cmd, "list_front ", 11)) {
			char argv[MAXARGV_LEN];

			for(int i=11; i<=length; i++)
				argv[i-11] = cmd[i];
			int list_idx = argv[strlen(argv)-1]-'0';

			struct list_elem* temp;
			temp = list_front(&list[list_idx]);
			struct list_item * item = list_entry(temp, struct list_item, elem);

			printf("%d\n", item->data);
		}

		else if(!strncmp(cmd, "list_back ", 10)) {
			char argv[MAXARGV_LEN];

			for(int i=10; i<=length; i++)
				argv[i-10] = cmd[i];
			int list_idx = argv[strlen(argv)-1]-'0';

			struct list_elem* temp;
			temp = list_back(&list[list_idx]);
			struct list_item * item = list_entry(temp, struct list_item, elem);

			printf("%d\n", item->data);
		}

		else if(!strncmp(cmd, "list_pop_front ", 15)) {
			char argv[MAXARGV_LEN];

			for(int i=15; i<=length; i++)
				argv[i-15] = cmd[i];
			int list_idx = argv[strlen(argv)-1]-'0';

			struct list_elem* temp;
			temp = list_pop_front(&list[list_idx]);
			free(temp);
		}

		else if(!strncmp(cmd, "list_pop_back ", 14)) {
			char argv[MAXARGV_LEN];

			for(int i=14; i<=length; i++)
				argv[i-14] = cmd[i];
			int list_idx = argv[strlen(argv)-1]-'0';

			struct list_elem* temp;
			temp = list_pop_back(&list[list_idx]);
			free(temp);
		}

		else if(!strncmp(cmd, "delete ", 7)) {
			char argv[MAXARGV_LEN];

			for(int i=7; i<=length; i++)
				argv[i-7] = cmd[i];

			if(!strncmp(argv, "list", 4)) {
				int list_idx = argv[strlen(argv)-1]-'0';
			
				while(!list_empty(&list[list_idx])) {
					struct list_elem * e;
					e = list_pop_front(&list[list_idx]);
					free(e);
				}
			}

			else if(!strncmp(argv, "hash", 4)) {
				int hash_idx = argv[strlen(argv)-1]-'0';

				hash_destroy(&hash[hash_idx], destructor_func);
			}

			else if(!strncmp(argv, "bm", 2)) {
				int bitmap_idx = argv[strlen(argv)-1]-'0';

				bitmap_destroy(bm[bitmap_idx]);
			}
		}

		else if(!strncmp(cmd, "list_insert ", 12)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];
			char argv3[MAXARGV_LEN];

			int i;
			for(i=12; cmd[i]!=' '; i++)
				argv1[i-12] = cmd[i];
			argv1[i-12]='\0';
			int list_idx = argv1[strlen(argv1)-1]-'0';
			i++;

			int j;
			for(j=i; cmd[j]!=' '; j++)
				argv2[j-i] = cmd[j];
			argv2[j-i]='\0';
			j++;

			int k;
			for(k=j; k<=length; k++)
				argv3[k-j] = cmd[k];

			struct list_elem* target = (struct list_elem*)malloc(sizeof(struct list_elem));
			struct list_item * item = list_entry(target, struct list_item, elem);
			item->data = atoi(argv3);

			int idx=atoi(argv2);
			struct list_elem* temp;
			temp = list_begin(&list[list_idx]);
			for(int i=0; i<idx; i++)
				temp = list_next(temp);
			
			list_insert(temp, target);
		}

		else if(!strncmp(cmd, "list_empty ", 11)) {
			char argv[MAXARGV_LEN];

			for(int i=11; i<=length; i++)
				argv[i-11] = cmd[i];
			int list_idx = argv[strlen(argv)-1]-'0';

			if(list_empty(&list[list_idx]))
				puts("true");
			else
				puts("false");
		}

		else if(!strncmp(cmd, "list_size ", 10)) {
			char argv[MAXARGV_LEN];

			for(int i=10; i<=length; i++)
				argv[i-10] = cmd[i];
			int list_idx = argv[strlen(argv)-1]-'0';

			printf("%zu\n", list_size(&list[list_idx])); 
		}

		else if(!strncmp(cmd, "list_max ", 9)) {
			char argv[MAXARGV_LEN];

			for(int i=9; i<=length; i++)
				argv[i-9] = cmd[i];
			int list_idx = argv[strlen(argv)-1]-'0';

			struct list_elem * max_elem;
			max_elem = list_max(&list[list_idx], compare_list, NULL);
			struct list_item * max_item;
			max_item = list_entry(max_elem, struct list_item, elem);
			printf("%d\n", max_item->data);
		}

		else if(!strncmp(cmd, "list_min ", 9)) {
			char argv[MAXARGV_LEN];

			for(int i=9; i<=length; i++)
				argv[i-9] = cmd[i];
			int list_idx = argv[strlen(argv)-1]-'0';

			struct list_elem * min_elem;
			min_elem = list_min(&list[list_idx], compare_list, NULL);
			struct list_item * min_item;
			min_item = list_entry(min_elem, struct list_item, elem);
			printf("%d\n", min_item->data);
		}

		else if(!strncmp(cmd, "list_remove ", 12)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];

			int i;
			for(i=12; cmd[i]!=' '; i++)
				argv1[i-12] = cmd[i];
			argv1[i-12]='\0';
			int list_idx = argv1[strlen(argv1)-1]-'0';
			i++;

			for(int j=i; j<=length; j++)
				argv2[j-i] = cmd[j];
			int idx=atoi(argv2);
			struct list_elem* temp;
			temp = list_begin(&list[list_idx]);
			for(int i=0; i<idx; i++)
				temp = list_next(temp);
			list_remove(temp);
			free(temp);
		}

		else if(!strncmp(cmd, "list_reverse ", 13)) {
			char argv[MAXARGV_LEN];

			for(int i=13; i<=length; i++)
				argv[i-13] = cmd[i];
			int list_idx = argv[strlen(argv)-1]-'0';

			list_reverse(&list[list_idx]);
		}

		else if(!strncmp(cmd, "list_insert_ordered ", 20)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];

			int i;
			for(i=20; cmd[i]!=' '; i++)
				argv1[i-20] = cmd[i];
			argv1[i-20]='\0';
			int list_idx = argv1[strlen(argv1)-1]-'0';
			i++;

			for(int j=i; j<=length; j++)
				argv2[j-i] = cmd[j];
			int value=atoi(argv2);
			struct list_elem* temp = (struct list_elem*)malloc(sizeof(struct list_elem));
			struct list_item* item = list_entry(temp, struct list_item, elem);
			item->data = value;

			list_insert_ordered(&list[list_idx], temp, compare_list, NULL);
		}

		else if(!strncmp(cmd, "list_swap ", 10)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];
			char argv3[MAXARGV_LEN];

			int i;
			for(i=10; cmd[i]!=' '; i++)
				argv1[i-10] = cmd[i];
			argv1[i-10] = '\0';
			int list_idx = argv1[strlen(argv1)-1]-'0';
			i++;

			int j;
			for(j=i; cmd[j]!=' '; j++)
				argv2[j-i] = cmd[j];
			argv2[j-i]='\0';
			j++;

			int k;
			for(k=j; k<=length; k++)
				argv3[k-j] = cmd[k];

			int idx1 = atoi(argv2);
			int idx2 = atoi(argv3);

			struct list_elem* target1;
			struct list_elem* target2;

			target1 = list_begin(&list[list_idx]);
			target2 = list_begin(&list[list_idx]);
			for(int i=0; i<idx1; i++)
				target1 = list_next(target1);
			for(int j=0; j<idx2; j++)
				target2 = list_next(target2);

			list_swap(target1, target2);
		}

		else if(!strncmp(cmd, "list_sort ", 10)) {
			char argv[MAXARGV_LEN];

			for(int i=10; i<=length; i++)
				argv[i-10] = cmd[i];
			int list_idx = argv[strlen(argv)-1]-'0';

			list_sort(&list[list_idx], compare_list, NULL);
		}

		else if(!strncmp(cmd, "list_splice ", 12)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];
			char argv3[MAXARGV_LEN];
			char argv4[MAXARGV_LEN];
			char argv5[MAXARGV_LEN];

			int i, j, k, l, m;

			for(i=12; cmd[i]!=' '; i++)
				argv1[i-12] = cmd[i];
			argv1[i-12] = '\0';
			i++;
			int list_idx1 = argv1[strlen(argv1)-1]-'0';

			for(j=i; cmd[j]!=' '; j++)
				argv2[j-i] = cmd[j];
			argv2[j-i] = '\0';
			j++;
			int before = atoi(argv2);

			for(k=j; cmd[k]!=' '; k++)
				argv3[k-j] = cmd[k];
			argv3[k-j] = '\0';
			k++;
			int list_idx2 = argv3[strlen(argv3)-1]-'0';

			for(l=k; cmd[l]!=' '; l++)
				argv4[l-k] = cmd[l];
			argv4[l-k] = '\0';
			l++;
			int first = atoi(argv4);

			for(m=l; m<=length; m++)
				argv5[m-l] = cmd[m];
			int last = atoi(argv5);

			struct list_elem * before_e = list_begin(&list[list_idx1]);
			for(int i=0; i<before; i++)
				before_e = list_next(before_e);
			
			struct list_elem * first_e = list_begin(&list[list_idx2]);
			for(int i=0; i<first; i++)
				first_e = list_next(first_e);

			struct list_elem * last_e = list_begin(&list[list_idx2]);
			for(int i=0; i<last; i++)
				last_e = list_next(last_e);

			list_splice(before_e, first_e, last_e);
		}

		else if(!strncmp(cmd, "list_unique ", 12)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];

			int i, j;
			
			for(i=12; cmd[i]!=' ' && cmd[i]!='\0'; i++)
				argv1[i-12] = cmd[i];
			argv1[i-12] = '\0';
			int list_idx1 = argv1[strlen(argv1)-1]-'0';

			int list_idx2;
			if(cmd[i]=='\0') {
				list_idx2=-1;
				argv2[0]='\0';
			}
			else {
				i++;
				for(j=i; j<=length; j++)
					argv2[j-i] = cmd[j];
				list_idx2 = argv2[strlen(argv2)-1]-'0';
			}

			if(list_idx2!=-1)
				list_unique(&list[list_idx1], &list[list_idx2], compare_list, NULL);
			else
				list_unique(&list[list_idx1], NULL, compare_list, NULL);
		}

		else if(!strncmp(cmd, "list_shuffle ", 13)) {
			char argv[MAXARGV_LEN];

			for(int i=13; i<=length; i++)
				argv[i-13] = cmd[i];
			int list_idx = argv[strlen(argv)-1]-'0';

			list_shuffle(&list[list_idx]);
		}

		else if(!strncmp(cmd, "create hashtable ", 17)) {
			char argv[MAXARGV_LEN];

			for(int i=17; i<=length; i++)
				argv[i-17] = cmd[i];
			int hash_idx = argv[strlen(argv)-1]-'0';

			hash_init(&hash[hash_idx], hash_func, compare_hash, NULL);
		}

		else if(!strncmp(cmd, "hash_insert ", 12)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];

			int i, j;
			for(i=12; cmd[i]!= ' '; i++)
				argv1[i-12] = cmd[i];
			argv1[i-12] = '\0';
			i++;
			int hash_idx = argv1[strlen(argv1)-1]-'0';

			for(j=i; j<=length; j++)
				argv2[j-i] = cmd[j];
			int value = atoi(argv2);

			struct hash_elem *target = (struct hash_elem*)malloc(sizeof(struct hash_elem));
			struct hash_item *item = hash_entry(target, struct hash_item, elem);
			item->data = value;

			hash_insert(&hash[hash_idx], target);
		}

		else if(!strncmp(cmd, "hash_apply ", 11)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];

			int i, j;
			for(i=11; cmd[i]!=' '; i++)
				argv1[i-11] = cmd[i];
			argv1[i-11] = '\0';
			i++;
			int hash_idx = argv1[strlen(argv1)-1]-'0';

			for(j=i; j<=length; j++)
				argv2[j-i] = cmd[j];
			
			if(!strcmp(argv2, "square"))
				hash_apply(&hash[hash_idx], square_func);
			else if(!strcmp(argv2, "triple"))
				hash_apply(&hash[hash_idx], triple_func);
		}

		else if(!strncmp(cmd, "hash_delete ", 12)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];

			int i, j;
			for(i=12; cmd[i]!=' '; i++)
				argv1[i-12] = cmd[i];
			argv1[i-12] = '\0';
			i++;
			int hash_idx = argv1[strlen(argv1)-1]-'0';

			for(j=i; j<=length; j++)
				argv2[j-i] = cmd[j];
			int value = atoi(argv2);

			struct hash_elem * target = (struct hash_elem*)malloc(sizeof(struct hash_elem));
			struct hash_item * item = hash_entry(target, struct hash_item, elem);
			item->data = value;

			struct hash_elem * result = hash_delete(&hash[hash_idx], target);

			free(target);
			if(result!=NULL)
				free(result);
		}

		else if(!strncmp(cmd, "hash_find ", 10)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];
			
			int i, j;
			for(i=10; cmd[i]!=' '; i++)
				argv1[i-10] = cmd[i];
			argv1[i-10] = '\0';
			i++;
			int hash_idx = argv1[strlen(argv1)-1]-'0';

			for(j=i; j<=length; j++)
				argv2[j-i] = cmd[j];
			int value = atoi(argv2);

			struct hash_elem * target = (struct hash_elem*)malloc(sizeof(struct hash_elem));
			struct hash_item * item = hash_entry(target, struct hash_item, elem);
			item->data = value;

			struct hash_elem *result = hash_find(&hash[hash_idx], target);
			if(result!=NULL)
				printf("%d\n", item->data);
			
			free(target);
		}

		else if(!strncmp(cmd, "hash_replace ", 13)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];
			
			int i, j;
			for(i=13; cmd[i]!=' '; i++)
				argv1[i-13] = cmd[i];
			argv1[i-13] = '\0';
			i++;
			int hash_idx = argv1[strlen(argv1)-1]-'0';

			for(j=i; j<=length; j++)
				argv2[j-i] = cmd[j];
			int value = atoi(argv2);

			struct hash_elem * target = (struct hash_elem*)malloc(sizeof(struct hash_elem));
			struct hash_item * item = hash_entry(target, struct hash_item, elem);
			item->data = value;

			struct hash_elem *result = hash_replace(&hash[hash_idx], target);
			if(result!=NULL)
				free(result);
		}

		else if(!strncmp(cmd, "hash_empty ", 11)) {
			char argv[MAXARGV_LEN];

			for(int i=11; i<=length; i++)
				argv[i-11] = cmd[i];
			int hash_idx = argv[strlen(argv)-1]-'0';

			bool result = hash_empty(&hash[hash_idx]);
			if(result)
				puts("true");
			else
				puts("false");
		}

		else if(!strncmp(cmd, "hash_size ", 10)) {
			char argv[MAXARGV_LEN];

			for(int i=10; i<=length; i++)
				argv[i-10] = cmd[i];
			int hash_idx = argv[strlen(argv)-1]-'0';

			printf("%zu\n", hash_size(&hash[hash_idx]));
		}

		else if(!strncmp(cmd, "hash_clear ", 11)) {
			char argv[MAXARGV_LEN];

			for(int i=11; i<=length; i++)
				argv[i-11] = cmd[i];
			int hash_idx = argv[strlen(argv)-1]-'0';

			hash_clear(&hash[hash_idx], destructor_func);
		}

		else if(!strncmp(cmd, "create bitmap ", 14)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];

			int i, j;
			for(i=14; cmd[i]!=' '; i++)
				argv1[i-14] = cmd[i];
			argv1[i-14] = '\0';
			i++;
			int bitmap_idx = argv1[strlen(argv1)-1]-'0';

			for(j=i; j<=length; j++)
				argv2[j-i] = cmd[j];
			size_t bit_cnt = atoi(argv2);

			bm[bitmap_idx] = bitmap_create(bit_cnt);
		}

		else if(!strncmp(cmd, "bitmap_mark ", 12)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];

			int i, j;
			for(i=12; cmd[i]!=' '; i++)
				argv1[i-12] = cmd[i];
			argv1[i-12] = '\0';
			i++;
			int bitmap_idx = argv1[strlen(argv1)-1]-'0';

			for(j=i; j<=length; j++)
				argv2[j-i] = cmd[j];
			size_t bit_idx = atoi(argv2);

			bitmap_mark(bm[bitmap_idx], bit_idx);
		}

		else if(!strncmp(cmd, "bitmap_all ", 11)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];
			char argv3[MAXARGV_LEN];

			int i, j, k;
			for(i=11; cmd[i]!=' '; i++)
				argv1[i-11] = cmd[i];
			argv1[i-11] = '\0';
			i++;
			int bitmap_idx = argv1[strlen(argv1)-1]-'0';

			for(j=i; cmd[j]!=' '; j++)
				argv2[j-i] = cmd[j];
			argv2[j-i] = '\0';
			j++;
			size_t start = atoi(argv2);

			for(k=j; k<=length; k++)
				argv3[k-j] = cmd[k];
			size_t cnt = atoi(argv3);

			bool result = bitmap_all(bm[bitmap_idx], start, cnt);
			if(result)
				puts("true");
			else
				puts("false");
		}

		else if(!strncmp(cmd, "bitmap_any ", 11)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];
			char argv3[MAXARGV_LEN];

			int i, j, k;
			for(i=11; cmd[i]!=' '; i++)
				argv1[i-11] = cmd[i];
			argv1[i-11] = '\0';
			i++;
			int bitmap_idx = argv1[strlen(argv1)-1]-'0';

			for(j=i; cmd[j]!=' '; j++)
				argv2[j-i] = cmd[j];
			argv2[j-i] = '\0';
			j++;
			size_t start = atoi(argv2);

			for(k=j; k<=length; k++)
				argv3[k-j] = cmd[k];
			size_t cnt = atoi(argv3);

			bool result = bitmap_any(bm[bitmap_idx], start, cnt);
			if(result)
				puts("true");
			else
				puts("false");
		}

		else if(!strncmp(cmd, "bitmap_contains ", 16)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];
			char argv3[MAXARGV_LEN];
			char argv4[MAXARGV_LEN];
			
			int i, j, k, l;
			
			for(i=16; cmd[i]!=' '; i++)
				argv1[i-16] = cmd[i];
			argv1[i-16] = '\0';
			i++;
			int bitmap_idx = argv1[strlen(argv1)-1]-'0';

			for(j=i; cmd[j]!=' '; j++)
				argv2[j-i] = cmd[j];
			argv2[j-i] = '\0';
			j++;
			size_t start = atoi(argv2);

			for(k=j; cmd[k]!=' '; k++)
				argv3[k-j] = cmd[k];
			argv3[k-j] = '\0';
			k++;
			size_t cnt = atoi(argv3);

			for(l=k; l<=length; l++)
				argv4[l-k] = cmd[l];
			bool value;
			if(!strcmp(argv4, "true"))
				value = true;
			else if(!strcmp(argv4, "false"))
				value = false;

			bool result = bitmap_contains(bm[bitmap_idx], start, cnt, value);
			if(result)
				puts("true");
			else
				puts("false");
		}

		else if(!strncmp(cmd, "bitmap_count ", 13)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];
			char argv3[MAXARGV_LEN];
			char argv4[MAXARGV_LEN];
			
			int i, j, k, l;
			
			for(i=13; cmd[i]!=' '; i++)
				argv1[i-13] = cmd[i];
			argv1[i-13] = '\0';
			i++;
			int bitmap_idx = argv1[strlen(argv1)-1]-'0';

			for(j=i; cmd[j]!=' '; j++)
				argv2[j-i] = cmd[j];
			argv2[j-i] = '\0';
			j++;
			size_t start = atoi(argv2);

			for(k=j; cmd[k]!=' '; k++)
				argv3[k-j] = cmd[k];
			argv3[k-j] = '\0';
			k++;
			size_t cnt = atoi(argv3);

			for(l=k; l<=length; l++)
				argv4[l-k] = cmd[l];
			bool value;
			if(!strcmp(argv4, "true"))
				value = true;
			else if(!strcmp(argv4, "false"))
				value = false;
			
			size_t result = bitmap_count(bm[bitmap_idx], start, cnt, value);
			printf("%zu\n", result);
		}

		else if(!strncmp(cmd, "bitmap_dump ", 12)) {
			char argv[MAXARGV_LEN];

			int i;
			for(i=12; i<=length; i++)
				argv[i-12] = cmd[i];
			int bitmap_idx = argv[strlen(argv)-1]-'0';

			bitmap_dump(bm[bitmap_idx]);
		}

		else if(!strncmp(cmd, "bitmap_flip ", 12)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];

			int i, j;
			for(i=12; cmd[i]!=' '; i++)
				argv1[i-12] = cmd[i];
			argv1[i-12] = '\0';
			i++;
			int bitmap_idx = argv1[strlen(argv1)-1]-'0';

			for(j=i; j<=length; j++)
				argv2[j-i] = cmd[j];
			size_t bit_idx = atoi(argv2);

			bitmap_flip(bm[bitmap_idx], bit_idx);
		}

		else if(!strncmp(cmd, "bitmap_none ", 12)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];
			char argv3[MAXARGV_LEN];

			int i, j, k;
			for(i=12; cmd[i]!=' '; i++)
				argv1[i-12] = cmd[i];
			argv1[i-12] = '\0';
			i++;
			int bitmap_idx = argv1[strlen(argv1)-1]-'0';

			for(j=i; cmd[j]!=' '; j++)
				argv2[j-i] = cmd[j];
			argv2[j-i] = '\0';
			j++;
			size_t start = atoi(argv2);

			for(k=j; k<=length; k++)
				argv3[k-j] = cmd[k];
			size_t cnt = atoi(argv3);

			bool result = bitmap_none(bm[bitmap_idx], start, cnt);
			if(result)
				puts("true");
			else
				puts("false");
		}

		else if(!strncmp(cmd, "bitmap_reset ", 13)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];

			int i, j;
			for(i=13; cmd[i]!=' '; i++)
				argv1[i-13] = cmd[i];
			argv1[i-13] = '\0';
			i++;
			int bitmap_idx = argv1[strlen(argv1)-1]-'0';

			for(j=i; j<=length; j++)
				argv2[j-i] = cmd[j];
			size_t bit_idx = atoi(argv2);

			bitmap_reset(bm[bitmap_idx], bit_idx);
		}

		else if(!strncmp(cmd, "bitmap_scan_and_flip ", 21)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];
			char argv3[MAXARGV_LEN];
			char argv4[MAXARGV_LEN];
			
			int i, j, k, l;
			
			for(i=21; cmd[i]!=' '; i++)
				argv1[i-21] = cmd[i];
			argv1[i-21] = '\0';
			i++;
			int bitmap_idx = argv1[strlen(argv1)-1]-'0';

			for(j=i; cmd[j]!=' '; j++)
				argv2[j-i] = cmd[j];
			argv2[j-i] = '\0';
			j++;
			size_t start = atoi(argv2);

			for(k=j; cmd[k]!=' '; k++)
				argv3[k-j] = cmd[k];
			argv3[k-j] = '\0';
			k++;
			size_t cnt = atoi(argv3);

			for(l=k; l<=length; l++)
				argv4[l-k] = cmd[l];
			bool value;
			if(!strcmp(argv4, "true"))
				value = true;
			else if(!strcmp(argv4, "false"))
				value = false;

			printf("%zu\n", bitmap_scan_and_flip(bm[bitmap_idx], start, cnt, value));
		}

		else if(!strncmp(cmd, "bitmap_scan ", 12)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];
			char argv3[MAXARGV_LEN];
			char argv4[MAXARGV_LEN];
			
			int i, j, k, l;
			
			for(i=12; cmd[i]!=' '; i++)
				argv1[i-12] = cmd[i];
			argv1[i-12] = '\0';
			i++;
			int bitmap_idx = argv1[strlen(argv1)-1]-'0';

			for(j=i; cmd[j]!=' '; j++)
				argv2[j-i] = cmd[j];
			argv2[j-i] = '\0';
			j++;
			size_t start = atoi(argv2);

			for(k=j; cmd[k]!=' '; k++)
				argv3[k-j] = cmd[k];
			argv3[k-j] = '\0';
			k++;
			size_t cnt = atoi(argv3);

			for(l=k; l<=length; l++)
				argv4[l-k] = cmd[l];
			bool value;
			if(!strcmp(argv4, "true"))
				value = true;
			else if(!strcmp(argv4, "false"))
				value = false;

			printf("%zu\n", bitmap_scan(bm[bitmap_idx], start, cnt, value));
		}

		else if(!strncmp(cmd, "bitmap_set_all ", 15)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];

			int i, j;
			for(i=15; cmd[i]!=' '; i++)
				argv1[i-15] = cmd[i];
			argv1[i-15] = '\0';
			i++;
			int bitmap_idx = argv1[strlen(argv1)-1]-'0';

			for(j=i; j<=length; j++)
				argv2[j-i] = cmd[j];
			bool value;
			if(!strcmp(argv2, "true"))
					value = true;
			else if(!strcmp(argv2, "false"))
					value = false;

			bitmap_set_all(bm[bitmap_idx], value);
		}

		else if(!strncmp(cmd, "bitmap_set ", 11)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];
			char argv3[MAXARGV_LEN];

			int i, j, k;
			for(i=11; cmd[i]!=' '; i++)
				argv1[i-11] = cmd[i];
			argv1[i-11] = '\0';
			i++;
			int bitmap_idx = argv1[strlen(argv1)-1]-'0';

			for(j=i; cmd[j]!=' '; j++)
				argv2[j-i] = cmd[j];
			argv2[j-i] = '\0';
			j++;
			size_t idx = atoi(argv2);

			for(k=j; k<=length; k++)
				argv3[k-j] = cmd[k];
			bool value;
			if(!strcmp(argv3, "true"))
				value = true;
			else if(!strcmp(argv3, "false"))
				value = false;

			bitmap_set(bm[bitmap_idx], idx, value);
		}

		else if(!strncmp(cmd, "bitmap_set_multiple ", 20)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];
			char argv3[MAXARGV_LEN];
			char argv4[MAXARGV_LEN];
			
			int i, j, k, l;
			
			for(i=20; cmd[i]!=' '; i++)
				argv1[i-20] = cmd[i];
			argv1[i-20] = '\0';
			i++;
			int bitmap_idx = argv1[strlen(argv1)-1]-'0';

			for(j=i; cmd[j]!=' '; j++)
				argv2[j-i] = cmd[j];
			argv2[j-i] = '\0';
			j++;
			size_t start = atoi(argv2);

			for(k=j; cmd[k]!=' '; k++)
				argv3[k-j] = cmd[k];
			argv3[k-j] = '\0';
			k++;
			size_t cnt = atoi(argv3);

			for(l=k; l<=length; l++)
				argv4[l-k] = cmd[l];
			bool value;
			if(!strcmp(argv4, "true"))
				value = true;
			else if(!strcmp(argv4, "false"))
				value = false;

			bitmap_set_multiple(bm[bitmap_idx], start, cnt, value);
		}

		else if(!strncmp(cmd, "bitmap_size ", 12)) {
			char argv[MAXARGV_LEN];

			int i;
			for(i=12; i<=length; i++)
				argv[i-12] = cmd[i];
			int bitmap_idx = argv[strlen(argv)-1]-'0';

			printf("%zu\n", bitmap_size(bm[bitmap_idx]));
		}

		else if(!strncmp(cmd, "bitmap_test ", 12)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];

			int i, j;
			for(i=12; cmd[i]!=' '; i++)
				argv1[i-12] = cmd[i];
			argv1[i-12] = '\0';
			i++;
			int bitmap_idx = argv1[strlen(argv1)-1]-'0';

			for(j=i; j<=length; j++)
				argv2[j-i] = cmd[j];
			size_t idx = atoi(argv2);

			bool result = bitmap_test(bm[bitmap_idx], idx);
			if(result)
				puts("true");
			else
				puts("false");
		}

		else if(!strncmp(cmd, "bitmap_expand ", 14)) {
			char argv1[MAXARGV_LEN];
			char argv2[MAXARGV_LEN];

			int i, j;
			for(i=14; cmd[i]!=' '; i++)
				argv1[i-14] = cmd[i];
			argv1[i-14] = '\0';
			int bitmap_idx = argv1[strlen(argv2)-1]-'0';

			for(j=i; j<=length; j++)
				argv2[j-i] = cmd[j];
			int size = atoi(argv2);

			bm[bitmap_idx] = bitmap_expand(bm[bitmap_idx], size);
		}
	}

	return 0;
}
