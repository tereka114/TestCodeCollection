#include <iostream>
#include <vector>

int N = 1000000;
int length = 10000;

using namespace std;

void size_function(vector<int> &array){
	for(int i = 0; i < length; i++){
		array[i];
	}
}

void iterator_function(vector<int> &array){
	vector<int>::iterator it = array.begin();
	for(it; it != array.end(); it++){
		*it;
	}
}

int main(int argc, char const *argv[])
{
	vector<int> vect(length);
	for(int i = 0; i < length; i++){
		vect[i] = i;
	}

	void (*po[])(vector<int> &) = {size_function,iterator_function};

	for(int i = 0; i < 2; i++){

		clock_t start,end;
		start = clock();

		for(int j = 0; j < N; j++){
			(po[i])(vect);
		}

		end = clock();
		cout << (double)(end - start) / CLOCKS_PER_SEC << endl;
	}
	return 0;
}