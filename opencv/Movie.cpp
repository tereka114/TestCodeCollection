#include <iostream>
#include "./Movie.hpp"

using namespace std;

int main(int argc, char const *argv[]){
	cout << argv[1] << endl;
	Movie movie(argv[1]);
	movie.DataRead();
	movie.CenterExtract();

	return 0;
}