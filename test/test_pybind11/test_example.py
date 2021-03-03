import example
import getopt
import sys
if __name__ == "__main__":
    if (len(sys.argv) >= 2) :
        for arg in sys.argv :
            if (arg == "--help") :
                help(example)
    else :
        try: 
            print(example.name)
            print(example.default)
            demo1 = example.Demo()
            demo2 = example.Demo(2, "wyw")
            print("demo1 -------------")
            print(str(demo1.num()))
            print(str(demo1.str()))
            demo1.set_num(1)
            print(str(demo1.num()))
            demo1.set_str('test')
            print(str(demo1.str()))
            print("demo2 -------------")
            print(str(demo2.num()))
            print(str(demo2.str()))
            demo2.set_num(3)
            print(str(demo2.num()))
            demo2.set_str('test2')
            print(str(demo2.str()))
            #使用def_property定义的方法，为name生成赋值和取值函数
            print(str(demo2.name))
            demo2.name = "demo2"
            print(str(demo2.name))
            
            print("Enum Numbers--------")
            print("Numbers::None = " + str(example.Num_None))
            print("Numbers::One = " + str(example.One))
            #print("compare with 1 : " + str(example.One == 1))
            #except : __eq__(): incompatible function arguments. The following argument types are supported:
            #   1. (self: example.Numbers, arg0: example.Numbers) -> bool

            test_enum = example.Tow
            print("Numbers::Tow = " + str(test_enum))
            print("compare with example.Tow : " + str(example.Tow == test_enum))
            test_enum = example.Three
            print("Numbers::Three = " + str(test_enum))
            print("compare with example::None : " + str(example.Num_None == test_enum))

            x = demo1.num()
            y = demo2.num()           
            print(str(x) + " + " + str(y) + " = " + str(example.add(x, y)))   
            #print(str(x) + " + " + str(y) + " = " + str(example.add())) 导出的函数参数需要全部满足
            #print(str(x) + " + " + str(y) + " = " + str(example.add1())) add1里面定义了arg1,arg2但是没有默认参数
            print(str(x) + " + " + str(y) + " = " + str(example.add1(x = 1, y = 2)))
            #print(str(x) + " + " + str(y) + " = " + str(example.add(x = 1, y = 2)))    add1定义了arg1,arg2，这里没有，所以不能这么用
            
            print(str(x) + " + " + str(y) + " = " + str(example.add2()))   
            #print(str(x) + " + " + str(y) + " = " + str(example.add3()))   
            
            print(str(x) + " - " + str(y) + " = " + str(example.sub(x, y)))    
            print(str(x) + " * " + str(y) + " = " + str(example.mul(example.B(), x, y)))    
            print(str(x) + " / " + str(y) + " = " + str(example.div(x, y)))  
            #print(str(x) + " / " + str(y) + " = " + str(example.div2(x, y)))  


            pydemo = example.PyDemo()
            pydemo.name = "pydemo"
            pydemo.set_num(4)  
            print(str(pydemo.name))
            pydemo.print()
        except Exception as identifier:
            print("except : " + str(identifier))
          
