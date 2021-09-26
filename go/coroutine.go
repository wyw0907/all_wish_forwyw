package main
import "fmt"
import "time"
func print(x int){
    time.Sleep(time.Second *time.Duration(1))
    fmt.Printf("print %d\n", x)
}

func main(){
    for i:=0; i < 10; i++{
        go print(i)
    }
    time.Sleep(time.Second *time.Duration(3))
}