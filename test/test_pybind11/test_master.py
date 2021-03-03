import master
import sys

class PyBase(master.Base) :
    def print2(self) :
        print("PyBase: " + str(super()._sum))

    def add(self, x, y) :
        return x + y
    
    def increment(self, n) :
        return super().increment(n * n)

class PyTestException(master.TestException) :
    def py_throw(self) :
        raise Exception("PyTestException::py_throw a exception") 

if __name__ == "__main__":
    if (len(sys.argv) >= 2) :
        for arg in sys.argv :
            if (arg == "--help") :
                help(master)
    else :
        try:
            pybase = PyBase()
            pybase.print2()
            print("1 + 2 = " + str(pybase.add(1, 2)))

            master.test_bind(pybase, 1, 2)
            master.test_bind(pybase, 2, 4)
            master.test_bind_ptr(pybase, 3, 6)

            pyex = PyTestException()
            try:
                pyex.cpp_throw()
            except Exception as e:
                print("pyex.cpp_throw(): " + str(e))
            
            try:
                pyex.py_throw_1()
            except Exception as e:
                print("pyex.py_throw_1(): " + str(e))
            
            try:
                pyex.py_throw_2()
            except Exception as e:
                print("pyex.py_throw_2(): " + str(e))

            master.test_bind_ex(pyex)
        except Exception as e:
            print("exception: " + str(e))