#include <vector>
#include <map>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>

using namespace std;

class Foo{
public:
	vector<int> vect;
	map<string,int> mp;

	vector<int> get_list(){
		return vect;
	}

	void add(int a){
		vect.push_back(a);
	}

	void mp_add(string a,int b){
		mp[a] = b;
	}

	map<string,int> get_map(){
		return mp;
	}
};

BOOST_PYTHON_MODULE(basic_vector_suite){
	using namespace boost::python;

	class_<Foo>("Foo")
	.def("get_list",&Foo::get_list)
	.def("add",&Foo::add)
	.def("mp_add",&Foo::mp_add)
	.def("get_map",&Foo::get_map);

	class_<vector<int> >("vector<int>")
	.def(vector_indexing_suite<vector<int> >());

	class_<map<string,int> >("map<string,int>")
	.def(map_indexing_suite<map<string,int> >());
}