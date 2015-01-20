package main

import (
	"fmt"
)


var increaseChannel = make(chan bool)
var decreaseChannel = make(chan bool)

var i int = 0
func main(){


	var done = make (chan bool)

	go variableManager()
	go thread1(done)
	go thread2(done)
	

	for j := 0; j < 2; j++ {
		<- done
	}
	
	fmt.Printf("i is now %v\n", i)
}

func variableManager(){
	for {
	    select {
	  	case <-increaseChannel:
	    	 i++
	    case <-decreaseChannel:
	    	 i--
		}
	}

}

func thread1(doneChannel chan<- bool){
	for j := 0; j < 10000; j++ {
	 
		increaseChannel <- true
	}

	doneChannel <- true	
}

func thread2(doneChannel chan<- bool){
	for j := 0; j < 10000; j++ {
		decreaseChannel <- true
	}

	doneChannel <- true
}
