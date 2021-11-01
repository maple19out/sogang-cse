#include "Graph_adj_list.h"

extern void errorExit(const char *s);

SLListD pool;
unsigned long UsedMemoryForArray = 0;

void Read_Graph_adj_list(int Vnum, vertex *V, int Enum, edge *E) {
	for (int start = 0; start < Enum; start++) {
		scanf_s("%d %d %d", &E[start].vf, &E[start].vr, &E[start].cost);
		E[start].flag = false;
		E[start].name = start;

		SLL *front;
		front = pool.allocSLL();
		front->i = start;

		SLL *rear;
		rear = pool.allocSLL();
		rear->i = start;

		//stack push
		V[E[start].vf].flag = false;
		V[E[start].vf].name = E[start].vf;
		V[E[start].vf].S.push(front);

		//stack push
		V[E[start].vr].flag = false;
		V[E[start].vr].name = E[start].vr;
		V[E[start].vr].S.push(rear);
	}


	// ���Ͽ��� �׷��� ������ �о� V�� E �迭�� �����Ѵ�.
	// V[ ].name�� E[ ].name�� ������ �ʿ䰡 ������ ���ǻ� �� �迭������
	//   index�� �����Ѵ�.
	// V[ ].S�� adjacent list header�� ���� ����ε� ���ȴ�.
	// ���� edge (i,j)�� ������ V[i].S�� edge index k�� push�ϰ�
	//  ���������� V[j].S�� k�� push�Ѵ�.
	// V[].flag�� E[].flag�� �ݵ�� false�� �����ؾ� �Ѵ�.

	// ***�� �Լ��� �ۼ��Ѵ�
	
}

void Free_Graph_adj_list(int Vnum, vertex *V) {
	for (int start = 0; start < Vnum; start++) {
		while (!V[start].S.empty()) 
			pool.freeSLL(V[start].S.pop());
	}
	// V �迭�� adjacency list�� ���Ҹ� pop()�� ����Ͽ�
	// ���  pool�� �ǵ��� �ش�
	// ***�� �Լ��� �ۼ��Ѵ�
}

int DFS_Tree_adj_list (
	int     src,   // source node index
	int     Vnum,  // number of vertices
	vertex *V,     // vertex structure array (starting index 0)
	int     Enum,  // number of edges
	edge   *E     // edge structure array (starting index 0)
) {
	int other_vertex;
	int result = 0;
	V[src].flag = true;
	SLL *temp = V[src].S.top();

	while (temp != NULL) {
		if (E[temp->i].flag == false) {
			if (E[temp->i].vf != src)
				other_vertex = E[temp->i].vf;
			else
				other_vertex = E[temp->i].vr;

			if (V[other_vertex].flag == false) {
				E[temp->i].flag = true;
				result +=  (E[temp->i].cost + DFS_Tree_adj_list(other_vertex, Vnum, V, Enum, E));
			}
		}

		temp = temp->p;
	}

	return result;
	

	// DFS�� ����Ͽ� DFS tree�� ���ϴ� ������ flag�� true�� �����Ѵ�.
	// DFS ���� vertex�� �Է� ���Ͽ��� �����ȴ�(src).
	// V[]�� adjacent list Ž���� .top() ��� �Լ��� ����Ͽ�
	//  adj list ����� ��� �̸� ���󰡸� ���� vertex�� Ž���Ѵ�.
	// DFS tree�� ���� ������ cost ���� return �Ѵ�
	//   (��� �� edge cost�� 1�̸� unique)
	// recursive �Լ��� �ۼ��ص� �����ϴ�.

	// ***�� �Լ��� �ۼ��Ѵ�
}

// SLL management functions
// �Ʒ� �Լ����� 02_04_CSE3081_CPrg&DataStructure_�߰��ڷ�.pdf��
//   �����Ͽ� �� �Լ����� ä�� ����.
// ���ø� �ʿ��ϹǷ� ť�� ��� �Լ��� �Ʒ� �Լ� ����Ʈ�� ����

// stack member fuctions
void sllStack::push(SLL *p) {
	// insert p to the top of stack ST
	p->p = ST;
	ST = p;
}

SLL *sllStack::pop(void) {
	// remove and return p at the top of ST
	SLL *p = ST;

	if (ST->p == NULL)
		ST = NULL;
	else
		ST = ST->p;
	return p;
}

SLL *sllStack::top(void) {
	// return p at the top of ST
	return ST;
}

bool sllStack::empty(void) {
	// return true if empty
	if (ST == NULL)
		return true;
	else
		return false;
}

// SLListD member functions
SLL *SLListD::allocSLL(void) {		// allocate an SLL element
	SLL *p;
	if (SLL_pool == NULL) {
		p = (SLL *)malloc(sizeof(SLL));
		if (p == NULL)
			errorExit("Error in Alloc_SLL");
		UsedMemoryForSLLs += sizeof(SLL);
		p->i = NONE;
	}
	else {
		p = SLL_pool;
		SLL_pool = p->p;
	}
	p->p = NULL;
	++SLL_cnt;
	return(p);
}

void SLListD::freeSLL(SLL *p) {		// free *p (move it to SLL_pool
	if (p->i == NONE) {
		errorExit("This is already freed(freeSLL).");
	}
	p->i = NONE;
	p->p = SLL_pool;
	SLL_pool = p;
	--SLL_cnt;
}

void SLListD::freeSLL_pool(void) {	// clear SLL_pool
	SLL *p = SLL_pool;

	while (p != NULL) {
		SLL_pool = p->p;
		free(p);
		UsedMemoryForSLLs -= sizeof(SLL);
		p = SLL_pool;
	}

	if (SLL_cnt != 0) {
		errorExit("Non-zero SLL_cnt after cleanup.");
	}
}