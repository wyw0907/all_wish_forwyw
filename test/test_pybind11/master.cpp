/*
pybind11高级使用demo
pybind11官方文档 https://pybind11.readthedocs.io/en/stable/advanced/classes.html
*/


#include "../../deps/pybind11/include/pybind11/pybind11.h"
#include <iostream>
#include "core/log.h"
namespace py = pybind11;

class Base 
{
public :
    virtual void print() const = 0;

    virtual int add(int x, int y) = 0;

    virtual int increment(int n)
    {
        _sum += n;
        return _sum; 
    }
public:
    int _sum = 0;
};

class PyBase : public Base
{
public:
    using Base::Base;
    void print() const override
    {
        PYBIND11_OVERLOAD_PURE_NAME(void, Base,  "print2", print);
    }

    int add(int x, int y) override
    {
        PYBIND11_OVERLOAD_PURE_NAME(int , Base, "add", add, x, y);
    }

    int increment(int n) override
    {
        PYBIND11_OVERLOAD_NAME(int , Base, "increment", increment, n);
    }
};

class TestException
{
public:
    void cpp_throw()
    {
        throw std::runtime_error("test cpp_throw");
    }

    void py_throw_1()
    {
        //不处理异常
        py_throw();
    }
    void py_throw_2()
    {
        try {
            py_throw();
        }
        catch(const std::exception &e)
        {
            LOG_ERR("TestException::py_throw_2 catch : %s", e.what());
        }
    }
    virtual void py_throw() = 0;
};

class PyTestException : public TestException
{
public:
    virtual void py_throw() override
    {
        PYBIND11_OVERLOAD_PURE(void, TestException, py_throw);
    }
};

PYBIND11_MODULE(master, m)
{
    //在python中重写虚函数
    py::class_<Base, PyBase, std::shared_ptr<Base> /*用例2，如果需要使用智能指针必须传入该模板*/>(m, "Base")
        .def(py::init<>())
        .def("print2", &Base::print)
        .def("add", &Base::add)
        .def("increment", &Base::increment)
        .def_readonly("_sum",  &Base::_sum)
        ;

    m.def("test_bind", [] (Base *base, int x, int y) {
        base->increment(base->add(x, y));
        base->print();
    });

    //智能指针
    m.def("test_bind_ptr", [] (std::shared_ptr<Base> base, int x, int y) {
        base->increment(base->add(x, y));
        base->print();
    });

    //异常
    py::class_<TestException, PyTestException>(m, "TestException")
        .def(py::init<>())
        .def("cpp_throw", &TestException::cpp_throw)
        .def("py_throw_1", &TestException::py_throw_1)
        .def("py_throw_2", &TestException::py_throw_2)
        .def("py_throw", &TestException::py_throw)
        ;
    m.def("test_bind_ex", [] (TestException *ex) {
        try 
        {
            ex->py_throw_1();
        }
        catch (const std::exception &e) 
        {
            LOG_ERR("test_bind_ex:py_throw_1 catch : %s", e.what());
        }

        try 
        {
            ex->py_throw_2();
        }
        catch (const std::exception &e) 
        {
            LOG_ERR("test_bind_ex:py_throw_2 catch : %s", e.what());
        } 
    });
    
}

