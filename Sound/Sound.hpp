#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>

using namespace boost::numeric;
using namespace boost::filesystem;
using namespace std;

class Sound{
public:
	std::vector<ublas::vector<double> > mfcc;
	Sound(){}
	//SoundRead();
	void FileReadMFCC(string path);
};

void Sound::FileReadMFCC(string path){
	std::ifstream ifs(path);
	string str;
	ublas::vector<double> vect(36);

	while(getline(ifs,str)){
		std::vector<string> v;

		boost::algorithm::split(v, str, boost::is_any_of(" "));
		for(int i = 0; i < v.size();i++){
			std::stringstream ss;
			double temp;
			ss << v[i];
			ss >> temp;
			vect[i] = temp;
		}
		mfcc.push_back(vect);
	}
}