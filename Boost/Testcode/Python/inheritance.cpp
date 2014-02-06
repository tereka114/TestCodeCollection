#include <boost/python.hpp>
#include <string>

using namespace std;
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
class Bar{
public:
	string temp2;
	void string_value(string a){
		 temp2 = a;
	}
};

class Baz:public Foo,public Bar{
public:
	string get_temp(){
		return temp2;
	}
};


BOOST_PYTHON_MODULE(inheritance){
	using namespace boost::python;
	class_<Foo>("Foo")
	.def("value",&Foo::value)
	.def("get_temp",&Foo::get_temp);

	class_<Bar>("Bar")
	.def("string_value",&Bar::string_value);

	class_<Baz,bases<Foo,Bar> >("Baz")
	.def("get_temp",&Baz::get_temp);
}