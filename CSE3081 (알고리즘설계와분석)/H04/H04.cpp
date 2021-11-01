#include <math.h>

void mergesort(double *X, unsigned *Xid, unsigned *TMP, int low, int high);	//mergesort function
void merge(double *X, unsigned *Xid, unsigned *TMP, int low, int mid, int high);	//merge function

void sortXid(double *X, unsigned *Xid, unsigned *TMP, unsigned N) {
	mergesort(X, Xid, TMP, 0, N - 1);
}

//mergesort Xid with X
void mergesort(double *X, unsigned *Xid, unsigned *TMP, int low, int high) {
	int mid;
	if (low < high) {
		mid = (low + high) / 2;
		mergesort(X, Xid, TMP, low, mid);
		mergesort(X, Xid, TMP, mid + 1, high);
		merge(X, Xid, TMP, low, mid, high);
	}
}

//merge Xid with X
void merge(double *X, unsigned *Xid, unsigned *TMP, int low, int mid, int high) {
	int i, j, k;
	i = low; j = mid + 1; k = low;
	while (i <= mid && j <= high) {
		if (X[Xid[i]] < X[Xid[j]])
			TMP[k++] = Xid[i++];
		else
			TMP[k++] = Xid[j++];
	}

	if (i > mid) {
		while (j <= high)
			TMP[k++] = Xid[j++];
	}
	else {
		while (i <= mid)
			TMP[k++] = Xid[i++];
	}
	k--;

	while (k>=low) {
		Xid[k] = TMP[k];
		k--;
	}
}

void selectionsort(double *Y, unsigned *Yid, int low, int high) {	//sorting function in O(N^2) Time
	int min, temp;
	for (int i = low; i < high; i++) {
		min = i;
		for (int j = i + 1; j <= high; j++) {
			if (Y[Yid[j]] < Y[Yid[min]])
				min = j;
		}
		temp = Yid[i];
		Yid[i] = Yid[min];
		Yid[min] = temp;
	}
}

double closestPairDC(
	unsigned L, unsigned R,	  // current leftmost and rightmost indices
	unsigned *pt1, unsigned *pt2, // closest pair points indices
	double *X, double *Y,         // (x,y) positions array(input)
	unsigned *Xid,  // point index array(sorted by x coordinates)
	unsigned *Yid,  // point index array(gradually sorted by y coordinates)
	unsigned *TMP,  // temporal index array
	unsigned THR	// threshold value
) {
	double d_L, d_R;	//d_L : closest distance in Left side, d_R : cloest distance in Right side
	double d_LR;	//min(d_L, d_R)
	double d;	//possible distance shorter than d_LR

	double middle;
	double left, right;
	unsigned left_index, right_index;

	if (R - L + 1 <= THR ) {	//Brute-force Method when less or equal than THR(Threshold)

		double size = -1;	//*exception handling : if returns -1, L==R which implies there is only one point in range (L, R)
		(*pt1) = L; (*pt2) = R;

		//compare all of the possible distance of points in range (L, R)
		for(unsigned i = L; i <= R-1; i++)
			for (unsigned j = i + 1; j <= R; j++) {
				if (size == -1 || (size > sqrt((X[j] - X[i])*(X[j] - X[i]) + (Y[j] - Y[i])*(Y[j] - Y[i])))) {
					size = sqrt((X[j] - X[i])*(X[j] - X[i]) + (Y[j] - Y[i])*(Y[j] - Y[i]));
					*(pt1) = i;
					*(pt2) = j;
				}
			}

		//copy index of Xid to Yid
		for (unsigned i = L; i <= R; i++)
			Yid[i] = Xid[i];
		//sort Yid using selectionsort function(to merge in linear time)
		selectionsort(Y, Yid, L, R);

		return size;
	}

	/*After closestPairDC function is called, index of *(pt1), *(pt2) is changed.
	In case d_L is shorter than d_R, *(pt1) and *(pt2) needs to be changed to closest pair of index of d_L.
	temp1, temp2 is used to store *(pt1) and *(pt2).
	*/
	d_L = closestPairDC(L, (L+R)/2, pt1, pt2, X, Y, Xid, Yid, TMP, THR);
	int temp1 = (*pt1);
	int temp2 = (*pt2);
	d_R = closestPairDC((L+R)/2 + 1, R, pt1, pt2, X, Y, Xid, Yid, TMP, THR);
	merge(Y, Yid, TMP, L, (L + R) / 2, R);
	if (d_L < d_R) {
		(*pt1) = temp1;
		(*pt2) = temp2;
		d_LR = d_L;
	}
	else
		d_LR = d_R;
	
	/*Exception handling : When there was only one point in Brute-force method (d_LR == -1)
	This indicates there were two points(or three points) before the area is divided, so just set d_LR as distance between two points if there are two points
	and set (*pt1) = L, (*pt2) = R. if there are three points in the LR area, set d_L as d_LR(2 points in left half, 1 point in right half)
	**There is no case that there is one point in left side and two points in right side as left side has more or equal numbers of points than right side.
	*/
	if (d_L==-1 && d_R==-1) {	//both divided case contains only one point -> exception occurs in both side in brute-force case
		(*pt1) = L;
		(*pt2) = R;
		d_LR = sqrt((X[L] - X[R])*(X[L] - X[R]) + (Y[L] - Y[R])*(Y[L] - Y[R]));
	}

	else if (d_R == -1) {	//left side has two points(no exception), right side has one point -> exception occurs in right side in brute-force case
		(*pt1) = temp1;
		(*pt2) = temp2;		
		d_LR = d_L;
	}

	//narrowing search area using d_LR
	middle = X[Xid[(L + R) / 2]];

	left = middle - d_LR;	//left boundary of search area
	unsigned s;
	for (s = 0; (L + R) / 2 - s >= L && X[Xid[(L + R) / 2 - s]] >= left; s++) {	//find index of which X[index] >= left
		if ((L + R) / 2 - s == 0)	//exception handling (unsigned variable can't have negative value)
			break;
	}
	if ((L + R) / 2 == s)
		left_index = L;
	else
		left_index = (L + R) / 2 - s + 1;
	
	right = middle + d_LR;	//right boundary of search area
	unsigned t;
	for (t = 0; (L + R) / 2 + t <= R && X[Xid[(L + R) / 2 + t]] <= right; t++);	//find index of which X[index] <= right
	t--;
	right_index = (L + R) / 2 + t;
	
	//search through narrowed area
	for (unsigned i = left_index; i <= right_index - 1; i++)
		for (unsigned j = i + 1; j <= right_index; j++) {
			if (Y[Yid[j]] - Y[Yid[i]] >= d_LR) break;
			d = sqrt((X[Yid[i]] - X[Yid[j]])*(X[Yid[i]] - X[Yid[j]]) + (Y[Yid[i]] - Y[Yid[j]])*(Y[Yid[i]] - Y[Yid[j]]));
			if (d < d_LR) {
				d_LR = d;
				(*pt1) = Yid[i];
				(*pt2) = Yid[j];
			}
		}

	return d_LR;	
}