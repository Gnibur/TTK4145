package main

import (
	"fmt"
	"runtime"
	"time"
)

var i int = 0

func main(){
	runtime.GOMAXPROCS(runtime.NumCPU())
	go thread1()
	go thread2()
	time.Sleep(1000*time.Millisecond)
	fmt.Printf("i is now %v\n", i)
}

func thread1(){
	for j := 0; j < 1000000; j++ {
		i++	
	}
}

func thread2(){
	for j := 0; j < 1000000; j++ {
		i--
	}
}
