/*
pybind11初级使用demo
pybind11官方文档 https://pybind11.readthedocs.io/en/stable/advanced/functions.html
*/
#include "../../deps/pybind11/include/pybind11/pybind11.h"
#include <iostream>
namespace py = pybind11;

int sub(int x, int y)
{
    return x - y;
}

class A 
{
    int mul(int x, int y)
    {
        return x * y;
    }
};


class B
{
public:
    int mul(int x, int y)
    {
        return x * y;
    }
};

template <class T>
T div(T && x, T && y)
{
    return x / y;
}


class C
{
public:
    static int div(int x, int y)
    {
        return x / y;
    }
};

class Demo
{
public:
    Demo()
    {
        std::cout << "Demo::Demo()" << std::endl;
    }

    Demo(int n, std::string s) : _num(n), _str(s)
    {
        std::cout << "Demo::Demo()" << std::endl;
    }

    virtual ~Demo()
    {
        std::cout << "Demo::~Demo()" << std::endl;
    }

    const int & num() const
    {
        return _num;
    }

    void set_num(int n)
    {
        _num = n;
    }

    const std::string & str() const
    {
        return _str;
    }

    void set_str(const std::string & s)
    {
        _str = s;
    }

    void set_str(std::string && s)
    {
        _str = s;
    }

    virtual void print()
    {
        std::cout << "Demo::print " << "num : " << _num << "; str : " << _str << std::endl;     
    }

    const char * name() const
    {
        return _name;
    }

    void set_name(const char *s)
    {
        std::strncpy(_name, s, sizeof(_name));
    }
    char _p_test = 0;   //公有成员
private:
    int _num = 0;
    std::string _str = "";
    char _name[32] = "demo";
};

class PyDemo : public Demo
{
public :
    PyDemo() : Demo()
    {
        std::cout << "PyDemo::PyDemo()" << std::endl;
    }

    virtual ~PyDemo()
    {
        std::cout << "PyDemo::~PyDemo()" << std::endl;
    }

    void print() override
    {
        std::cout << "PyDemo::print " << "num : " << Demo::num() << "; str : " << Demo::str() << std::endl;
    }
};

enum class Numbers
{
    None = 0,
    One = 1,
    Two = 2,
    Three = 3,
};

PYBIND11_MODULE(example, m)
{
    m.doc() = "a example for pybind11";

    // 导出函数
    m.def("add", [] (int x, int y) {
        return x + y;
    });

    m.def("add1", [] (int x, int y) {
        return x + y;
    }, "a lambda function", py::arg("x"), py::arg("y"));

    m.def("add2", [] (int x, int y) {
        return x + y;
    }, "a lambda function", py::arg("x") = 1, py::arg("y") = 2);

    /*m.def("add3", [] (int x = 1, int y = 2) {
        return x + y;
    }, "a lambda function");
    */ //这种写法是错误的，参数需要绑定到py::arg

    m.def("sub", sub);

    //m.def("mul", &A::mul); 成员函数必须是public
   
    m.def("mul", &B::mul); //因为是成员函数，所以需要导出类声明
    py::class_<B>(m, "B")
        .def(py::init<>()); //默认构造函数需要显示导出

    //m.def("div", div); 不能绑定模板

    m.def("div", C::div);
    //静态函数的另一种绑定方式
    py::class_<C>(m, "C")
        .def_static("div2", &C::div);

    //定义变量
    m.attr("name") = "wyw";

    m.attr("default") = 0;

    //导出类
    py::class_<Demo>(m, "Demo")
        .def(py::init<>())
        .def(py::init<int, std::string>())
        .def("str", &Demo::str)
        .def("set_str", py::overload_cast<const std::string &>(&Demo::set_str))
        .def("set_str", py::overload_cast<std::string &&>(&Demo::set_str))    //其实右值函数没啥用，这里演示重载
        .def("num", &Demo::num)
        .def("set_num", &Demo::set_num)
        .def_property("name", &Demo::name, &Demo::set_name) //为name定义赋值和取值的方法
        .def("print", &Demo::print)
        .def_readwrite("_p_test", &Demo::_p_test) //导出公有成员变量
        ;

    //导出enum
    py::enum_<Numbers>(m , "Numbers")
        .value("One", Numbers::One)
        .value("Tow", Numbers::Two)
        .value("Three", Numbers::Three)
        .value("Num_None", Numbers::None)   //None是python关键字，同时演示导出的枚举名和原名称不一致的场景
        .export_values();


    // 继承，虚函数
    /*py::class_<PyDemo>(m, "PyDemo")
        .def(py::init<>())
        .def("print", &PyDemo::print)
        .def("num", &PyDemo::num)
        .def("set_num", &PyDemo::set_num)
        .def_property("name", &Demo::name, &Demo::set_name) //为name定义赋值和取值的方法
        ;
    */ //这样写可以使用，但是很麻烦，失去了继承性

    py::class_<PyDemo, Demo>(m, "PyDemo")
        .def(py::init<>());


}