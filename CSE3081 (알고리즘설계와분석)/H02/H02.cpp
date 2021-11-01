#include "Graph_adj_array.h"

void Read_Graph_adj_array (int Vnum, vertex V[], int Enum, edge E[]) {
	int front, rear;
	int temp;
	//initializing vertex f_hd, r_hd;
	for (int start = 0; start < Vnum; start++) {
		V[start].f_hd = NONE;
		V[start].r_hd = NONE;
		V[start].name = start;
		V[start].flag = false;
	}

	for (int start = 0; start < Enum; start++) {
		scanf_s("%d %d %d", &E[start].vf, &E[start].vr, &E[start].cost);
		E[start].cost = 1;
		E[start].flag = false;
		E[start].fp = NONE;
		E[start].rp = NONE;
		E[start].name = start;

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
	// read graph information
	// V와 E의 name field는 각각 자신의 인덱스를 저장한다
	// V[].flag과 E[].flag은 반드시 false로 설정해야 한다.
	
	// ***이 함수를 작성한다

}

int DFS_Tree_adj_array(
	int     src,   // source node index
	int     Vnum,  // number of vertices
	vertex *V,     // vertex structure array (starting index 0)
	int     Enum,  // number of edges
	edge   *E     // edge structure array (starting index 0) 
) {  
	int other_vertex;
	int result = 0;
	int temp;
	V[src].flag = true;
	temp = V[src].f_hd;

	while (temp != -1) {
		if (E[temp].flag == false) {
			if (E[temp].vf != src)
				other_vertex = E[temp].vf;
			else
				other_vertex = E[temp].vr;

			if (V[other_vertex].flag == false) {
				E[temp].flag = true;
				result += E[temp].cost + DFS_Tree_adj_array(other_vertex, Vnum, V, Enum, E);
			}
		}
		temp = E[temp].fp;
	}



	temp = V[src].r_hd;
	while (temp != -1) {
		if (E[temp].flag == false) {
			if (E[temp].vf != src)
				other_vertex = E[temp].vf;
			else
				other_vertex = E[temp].vr;

			if (V[other_vertex].flag == false) {
				E[temp].flag = true;
				result += E[temp].cost + DFS_Tree_adj_array(other_vertex, Vnum, V, Enum, E);
			}
		}
		temp = E[temp].rp;
	}





	return result;
	// DFS를 사용하여 DFS tree에 속하는 에지의 flag을 true로 설정한다.
	// DFS 시작 vertex는 입력 파일에서 지정된다(src).
	// DFS tree에 속한 에지의 cost 함을 return 한다(모든 각 edge cost가 1이면 unique)
	// recursive 함수로 작성해도 무방하다.

	// ***이 함수를 작성한다***

}
