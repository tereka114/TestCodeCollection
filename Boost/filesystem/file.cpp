#include <iostream>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
using namespace std;

int main()
{
	namespace fs = boost::filesystem;

	// カレントディレクトリのファイル一覧してみよう
	fs::directory_iterator end;
	for( fs::directory_iterator it(fs::current_path()); it!=end; ++it )
	{
		if( fs::is_directory(*it) )
			cout << "D ";
		else
			cout << "F ";
		// leaf() パス情報を切って、ファイルの名前部分のみ取り出し
		cout << it->path() << endl;
	}

	return 0;
}