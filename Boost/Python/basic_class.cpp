//basic_class.cpp
#include <boost/python.hpp>

class Foo{
public:
	int temp;
	void value(int a){
		temp = a;
	}
	int get_temp(){
		return temp;
	}

};

BOOST_PYTHON_MODULE(basic_class){
	using namespace boost::python;
	class_<Foo>("Foo")
	.def("value",&Foo::value)
	.def("get_temp",&Foo::get_temp);
}