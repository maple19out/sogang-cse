#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <queue>

#define NONE -1
#define INF 1<<30	//infinite distance

typedef struct elm_edge {
	int  vf, vr;  // e = (vf, vr) where vf, vr are vertex array indices
	int  cost;    // edge cost
	bool flag;    // true if the edge is in the SP spanning tree, false otherwise 
	int  fp, rp;  // adj list ptr of forward and reverse (-1 if none)
} edge;

typedef struct elm_vertex {
	int  f_hd, r_hd;	// adj list header(-1 if none)
	int  distance;		// distance from src to this vertex
	int  heap_idx;		// minHeap index
	bool inS;			// true if SP length found(true when move from V-S to S) 
} vertex;


void heapify(int src, int Vnum, vertex *V, int Enum, edge *E, int *minHeap) {
	V[src].heap_idx = -1;	//src excluded
	minHeap[0] = -1;	//0 index excluded
	int i = 1;

	//this loop first insert the element at the end of minHeap, and move the element in appropriate position.
	for (int v_idx = 0; v_idx < Vnum; v_idx++) {
		if (v_idx != src) {
			minHeap[i] = v_idx;

			for (int j = i; j > 1; j /= 2) {
				if (V[minHeap[j / 2]].distance > V[minHeap[j]].distance) {
					int temp = minHeap[j / 2];
					minHeap[j / 2] = minHeap[j];
					minHeap[j] = temp;
				}
				else
					break;
			}
			i++;
		}
	}

	//cross reference
	for (int i = 1; i <= Vnum-1; i++)
		V[minHeap[i]].heap_idx = i;
}

void delete_minHeap(vertex *V, int heapsize, int *minHeap) {
	int temp;

	//first switch first element(element to be deleted) with last element
	temp = minHeap[1];
	minHeap[1] = minHeap[heapsize];
	minHeap[heapsize] = temp;

	V[minHeap[1]].heap_idx = 1;
	V[minHeap[heapsize]].heap_idx = -1;
	heapsize--;

	for (int i = 1; i <= heapsize; ) {
		if (i * 2 + 1 <= heapsize) {
			if (V[minHeap[i * 2]].distance < V[minHeap[i]].distance && V[minHeap[i * 2]].distance <= V[minHeap[i * 2 + 1]].distance) {
				temp = minHeap[i];
				minHeap[i] = minHeap[i * 2];
				minHeap[i * 2] = temp;

				V[minHeap[i]].heap_idx = i;
				V[minHeap[i * 2]].heap_idx = i * 2;

				i = i * 2;
			}
			else if (V[minHeap[i * 2 + 1]].distance < V[minHeap[i]].distance && V[minHeap[i * 2 + 1]].distance <= V[minHeap[i * 2]].distance) {
				temp = minHeap[i];
				minHeap[i] = minHeap[i * 2 + 1];
				minHeap[i * 2 + 1] = temp;

				V[minHeap[i]].heap_idx = i;
				V[minHeap[i * 2 + 1]].heap_idx = i * 2 + 1;

				i = i * 2 + 1;
			}

			else
				break;
		}

		else if (i * 2 <= heapsize) {
			if (V[minHeap[i * 2]].distance < V[minHeap[i]].distance) {
				temp = minHeap[i];
				minHeap[i] = minHeap[i * 2];
				minHeap[i * 2] = temp;

				V[minHeap[i]].heap_idx = i;
				V[minHeap[i * 2]].heap_idx = i * 2;

				i = i * 2;
			}
			else
				break;
		}

		else
			break;
		}
}

void adjust_minHeap(vertex *V, int w, int *minHeap) {
	int i = V[w].heap_idx;

	//compare with parent element and exchange position until Heap property is retrieved.
	while (i != 1) {
		if (V[minHeap[i]].distance < V[minHeap[i / 2]].distance) {
			int temp = minHeap[i];
			minHeap[i] = minHeap[i / 2];
			minHeap[i / 2] = temp;

			V[minHeap[i]].heap_idx = i;
			V[minHeap[i / 2]].heap_idx = i / 2;

			i = i / 2;
		}
		else
			break;		
	}
}



int SPT_Dijkstra(
	int src,	// source vertex index
	// graph structure array
	// 1. the adjacency list structure is the same as PHW02
	// 2. additional fields are added for Dijkstra's algorithm(see .h file)
	int Vnum, vertex *V,	// Vertex array size and the array
	int Enum, edge *E,		// Edge array size and the array

	int *minHeap	// array for min heap (array size = Vnum+1)
		// heap index range is 1 ~ (Vnum - 1) note: src must not in the initial heap
		// just arry is passed. must construct min-heap in this function

	// OUTPUT
	// 1. V[].distance : shortest path length from src to this vertex
	// 2. shortest path spanning tree : set E[].flag = true if the edge is in the tree
	// 3. return the sum of edge costs in the shortest path spanning tree.
	//    should be small as possible (think a way to make it small)
) {
	int treeCost = 0;
	// *** 이 함수를 작성하자 ***
	// 반드시 min-heap을 사용하여 O((n+m)logn) 알고리즘을 구현해야 한다(아니면 trivial한 프로그램임)
	// heap 연산 등 필요한 함수는 자유롭게 작성하여 추가한다.
	// 그러나 global 변수, dynamic array 등은 추가로 사용하지 않는다(실제로 필요 없다)

	//Initializing process (1): set src distance to be 0, other distance to be INF 
	for (int i = 0; i < Vnum; i++) {
		if (i == src) {
			V[i].inS = true;
			V[i].distance = 0;
		}
		else
			V[i].distance = INF;
	}
	//Initializing process (2): set vertices distance to be cost between src if there is any edge between them.
	for (int i = 0; i < Enum; i++) {
		if (E[i].vr == src)
			V[E[i].vf].distance = E[i].cost;
		else if (E[i].vf == src)
			V[E[i].vr].distance = E[i].cost;
	}
	//construct minHeap
	heapify(src, Vnum, V, Enum, E, minHeap);

	for (int i = 0; i < Vnum - 1; i++) {

		int u = minHeap[1];		
		V[u].inS = true;
		delete_minHeap(V, Vnum-i-1, minHeap);

		int flag = 0;	//variable flag : set 1 to indicate edge to be in SPT has been fonud.

		//two loops below has two functions : (1) : update vertex distance adjacent to u, (2) : construct SPT
		for (int e = V[u].f_hd; e != NONE; e = E[e].fp) {
			int w;
			if (E[e].vf == u)
				w = E[e].vr;
			else
				w = E[e].vf;
			//Updating vertex distance
			if (V[w].inS == false && V[u].distance + E[e].cost < V[w].distance) {
				V[w].distance = V[u].distance + E[e].cost;
				adjust_minHeap(V, w, minHeap);
			}
			//Constructing SPT
			if (flag == 0 && V[w].inS == true && V[u].distance - E[e].cost == V[w].distance) {
				if (E[e].flag == false) {
					E[e].flag = true;
					treeCost += E[e].cost;
					flag = 1;
				}
			}
		
		}
		for (int e = V[u].r_hd; e != NONE; e = E[e].rp) {
			int w;
			if (E[e].vf == u)
				w = E[e].vr;
			else
				w = E[e].vf;
			//Updating vertex distance
			if (V[w].inS == false && V[u].distance + E[e].cost < V[w].distance) {
				V[w].distance = V[u].distance + E[e].cost;
				adjust_minHeap(V, w, minHeap);
			}
			//Constructing SPT
			if (flag == 0 && V[w].inS == true && V[u].distance - E[e].cost == V[w].distance) {
				if (E[e].flag == false) {
					E[e].flag = true;
					treeCost += E[e].cost;
					flag = 1;
				}
			}
		}
	}
	return treeCost;
}

void Read_Graph(int Vnum, vertex *V, int Enum, edge *E) {
	// Graph 자료구조를 만드는 함수
	// *** 이 함수를 추가하자 ***
	// PHW02의 Read_Graph_adj_array()를 이 과제의 자료구조에 맞춰 살짝 수정하여 사용한다
	// 즉, Read_Graph_adj_array()에서 불필요한 내용을 제거한 후 사용.

	int front, rear;
	int temp;

	for (int start = 0; start < Vnum; start++) {
		V[start].f_hd = NONE;
		V[start].r_hd = NONE;
		V[start].distance = 0;
		V[start].heap_idx = NONE;
		V[start].inS = false;
	}

	for (int start = 0; start < Enum; start++) {
		scanf_s("%d %d %d", &E[start].vf, &E[start].vr, &E[start].cost);
		E[start].flag = false;
		E[start].fp = NONE;
		E[start].rp = NONE;

		front = E[start].vf;
		rear = E[start].vr;

		if (V[front].f_hd == NONE)
			V[front].f_hd = start;
		else {
			temp = V[front].f_hd;
			V[front].f_hd = start;
			E[start].fp = temp;
		}

		if (V[rear].r_hd == NONE)
			V[rear].r_hd = start;
		else {
			temp = V[rear].r_hd;
			V[rear].r_hd = start;
			E[start].rp = temp;
		}
	}
}

// the following functions are for testing if the submitted program is correct.
int  Tree_Check(int Vnum, vertex *V, int Enum, edge *E, int *visited);
bool SPT_test(int src, int Vnum, vertex *V, int Enum, edge *E, int *minHeap);
void Error_Exit(const char *s);

int main ( void ) {
	int		src;
	vertex *V;		int Vnum;
	edge   *E;		int Enum;
	int    *minHeap;
	int    Tree_cost;
	int    Tnum; 		// # of test cases
	clock_t start, finish;
	double cmpt;

	scanf_s("%d", &Tnum);		// read # of tests

	for (int t = 0; t < Tnum; t++ ) {
		scanf_s("%d %d %d", &Vnum, &Enum, &src);
		V = new vertex [Vnum];
		E = new edge [Enum];
		minHeap = new int[Vnum + 1];	// heap array allocation
		if ( V == NULL || E == NULL || minHeap == NULL ) {
			Error_Exit("Memory Allocation Error");
		}
		Read_Graph(Vnum, V, Enum, E);

		/**/start = clock();	// start timer

		Tree_cost = SPT_Dijkstra(src, Vnum, V, Enum, E, minHeap);	// code by students

		/**/finish = clock();	// stop timer
		cmpt = ((double)(finish - start)) / (double)CLK_TCK;

		// 아래 Tree_Check와 SPT_test 함수는 첨부한 SPT_test.obj 파일에 있다.
		// 이 테스트에서 오류로 인하여 프로그램이 정지하면 뭔가 잘못된 것이다(제출해도 0점)
        if (Tree_Check(Vnum, V, Enum, E, minHeap) == 0) {
			Error_Exit("   ERROR The result is not a spanning tree");
		}
		if (SPT_test(src, Vnum, V, Enum, E, minHeap) == false) {
			Error_Exit("** Something wrong in applying Dijkstra's");
		}
		if ( t != 0 ) 
			printf("\n");

	printf("**T%d (Dijkstra) (V = %d, E = %d, time = %.3f sec) Tree Cost = %d\n", 
			t+1, Vnum, Enum, cmpt, Tree_cost);

		delete minHeap; delete V; delete E;
	}
	return 0;
}

void Error_Exit (const char *s ) {
  printf("%s\n", s);
  exit(-1);
}