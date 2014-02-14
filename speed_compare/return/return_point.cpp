#include <iostream>
#include <vector>

using namespace std;

int N = 1000000;

void argument(vector<int> &array1,vector<int> &array2){
	int size = array1.size();
	for(int i = 0; i < size; i++){
		array2[i] = i;
	}
}

vector<int> return_argument(vector<int> &array1,vector<int> &array2){
	int size = array1.size();
	for(int i = 0; i < size; i++){
		array2[i] = i;
	}
	return array2;
}

int main(void){
	int size = 10000;
	vector<int> array1(size);
	vector<int> array2(size);
	for(int i = 0; i < size; i++){
		array1[i] = i;
	}
	clock_t start,end;
	start = clock();
	for(int i = 0; i < N; i++){
		argument(array1,array2);
	}
	end = clock();
	cout << (double)(end-start)/CLOCKS_PER_SEC << endl;

	start = clock();
	for(int i = 0; i < N; i++){
		return_argument(array1,array2);
	}
	end = clock();
	cout << (double)(end-start)/CLOCKS_PER_SEC << endl;
}