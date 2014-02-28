#include <iostream>
#include <string>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
using namespace std;

void current_directory(){
	namespace fs = boost::filesystem;
	fs::directory_iterator end;
	
	for( fs::directory_iterator it(fs::current_path()); it!=end; ++it )
	{
		if( fs::is_directory(*it) )
			cout << "D ";
		else
			cout << "F ";
		cout << it->path() << endl;
	}
}

void filepath(){
	namespace fs = boost::filesystem;
	fs::path dataset_dir("/Users/Tereka/Programing/Research/Research");
	fs::path m_dataset_dir("/Users/Tereka/Programing/Research/Research/youtube_movie");
	fs::directory_iterator first_end;

	for(fs::directory_iterator it(dataset_dir);it!=first_end; ++it){
		cout << it->path() << endl;
		cout << it->path().parent_path() << endl;

		fs::directory_iterator second_end;

		if(fs::is_directory(*it)){
			for(fs::directory_iterator its(*it);its!=second_end;++its){
				cout << m_dataset_dir / it->path().filename() / its->path().filename() << endl;
			}
		}
	}
}

int main(void){
	namespace fs = boost::filesystem;
	//カレントディレクトリの一覧を表示
	current_directory();
	filepath();
	return 0;
}