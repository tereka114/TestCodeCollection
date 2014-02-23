#include <iostream>
#include <vector>

using namespace std;

int main(void){
	int bit = 3;
	int cnt = 1 << bit;

	vector<vector<double> > bitrate;

	for(int i = 0; i < cnt; i++){
		vector<double> temp;
		for(int j = 0; j < bit; j++){
			int a = ((i & (1 << j)) != 0) ? 1 : 0;
			if((i & (1 << j)) != 0){
				temp.push_back(1);
			}else{
				temp.push_back(0);
			}
			cout << temp[j];
		}

		int flag = temp[0];
		for(int i = 1; i < temp.size(); i++){
			flag = flag ^ (int)temp[i];
		}
	}
	return 0;
}