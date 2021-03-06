#define F (1<<14)

int int_to_fp(int n);
int fp_to_int_round(int x);
int fp_to_int(int x);
int add_mixed(int x, int n);
int sub_mixed(int x, int n);
int mult_fp(int x, int y);
int div_fp(int x, int y);


int int_to_fp(int n) {
	return n*F;
}

int fp_to_int_round(int x) {
	if(x>=0)
		return (x+F/2)/F;
	else
		return (x-F/2)/F;
}

int fp_to_int(int x) {
	return x/F;
}

int add_mixed(int x, int n) {
	return x+n*F;
}

int sub_mixed(int x, int n) {
	return x-n*F;
}

int mult_fp(int x, int y) {
	return ((int64_t)x)*y/F;
}

int div_fp(int x, int y) {
	return ((int64_t)x)*F/y;
}
