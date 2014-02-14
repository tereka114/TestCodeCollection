#include <iostream>
#include <vector>

using namespace std;

int N = 1000000;

void argument(vector<int> array){
	int a = 10;
}
void pointer_argument(vector<int> &array){
	int b = 10;
}

int main(void){
	int size = 1000000;
	vector<int> array(size);
	for(int i = 0; i < size; i++){
		array[i] = i;
	}
	clock_t start,end;
	start = clock();
	for(int i = 0; i < N; i++){
		argument(array);
	}
	end = clock();
	cout << (double)(end-start)/CLOCKS_PER_SEC << endl;

	start = clock();
	for(int i = 0; i < N; i++){
		pointer_argument(array);
	}
	end = clock();
	cout << (double)(end-start)/CLOCKS_PER_SEC << endl;
}