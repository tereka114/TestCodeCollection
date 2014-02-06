#include <vector>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

using namespace std;

class Foo{
public:
	vector<int> vect;

	vector<int> get_list(){
		return vect;
	}

	void add(int a){
		vect.push_back(a);
	}
};

BOOST_PYTHON_MODULE(basic_vector){
	using namespace boost::python;

	class_<Foo>("Foo")
	.def("get_list",&Foo::get_list)
	.def("add",&Foo::add);


	class_<vector<int> >("vector<int>")
	.def("__getitem__", (int const&(vector<int>::*)(vector<int>::size_type) const)&vector<int>::at, return_value_policy<copy_const_reference>())
	.def("__len__", &vector<int>::size)
	;
}