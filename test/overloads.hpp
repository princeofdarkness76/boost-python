#include <boost/python.hpp>

//
//  A bunch of overloads for boost.python testing 
//
std::string f(bool)                        { return "bool";            }
std::string f(short)                       { return "short";           }
std::string f(unsigned)                    { return "unsigned";        }
std::string f(int)                         { return "int";             }
std::string f(float)                       { return "float";           }
std::string f(double)                      { return "double";          }
std::string f(std::string)                 { return "std::string";     }
				           			     
std::string f(int, bool)                   { return "int,bool";        }
std::string f(int, short)                  { return "int,short";       }
std::string f(int, unsigned)               { return "int,unsigned";    }
std::string f(int, int)                    { return "int,int";         }
std::string f(int, float)                  { return "int,float";       }
std::string f(int, double)                 { return "int,double";      }
std::string f(int, std::string)            { return "int,std::string"; }

std::string f(float, bool)                 { return "float,bool";        }
std::string f(float, short)                { return "float,short";       }
std::string f(float, unsigned)             { return "float,unsigned";    }
std::string f(float, int)                  { return "float,int";         }
std::string f(float, float)                { return "float,float";       }
std::string f(float, double)               { return "float,double";      }
std::string f(float, std::string)          { return "float,std::string"; }

std::string f(float, float, bool)          { return "float,float,bool";        }
std::string f(float, float, short)         { return "float,float,short";       }
std::string f(float, float, unsigned)      { return "float,float,unsigned";    }
std::string f(float, float, int)           { return "float,float,int";         }
std::string f(float, float, float)         { return "float,float,float";       }
std::string f(float, float, double)        { return "float,float,double";      }
std::string f(float, float, std::string)   { return "float,float,std::string"; }

template <typename T>
void r(std::string name="f")
{
  boost::python::def(name.c_str(), (std::string(*)(T))&f);
}

template <typename T, typename U>
void r(std::string name="f")
{
  boost::python::def(name.c_str(), (std::string(*)(T,U))&f);
}

template <typename T, typename U, typename V>
void r(std::string name="f")
{
  boost::python::def(name.c_str(), (std::string(*)(T,U,V))&f);
}

