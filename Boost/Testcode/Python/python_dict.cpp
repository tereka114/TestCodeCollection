#include <vector>
#include <map>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>

using namespace std;

template<class T1,class T2>
boost::python::dict get_dict(map<T1,T2> &map_){  
     boost::python::dict py_dict;
     typename map<T1,T2>::const_iterator it;

     for(it = map_.begin(); it != map_.end(); ++it)   
          py_dict[it->first]=it->second;        
     return py_dict;  
}

boost::python::dict test(){
	map<string,string> mp;
	mp["foo"] = "bar";
	mp["temp"] = "tenp2";
	boost::python::dict return_dict;
	return_dict = get_dict(mp);
	return return_dict;
}

BOOST_PYTHON_MODULE(python_dict){
	def("test", &test);
}