package main

import (
	"fmt"
	"runtime/cgo"
)

/*
#include <stdint.h>

extern void go_callback_int(uintptr_t h, int p1);
static inline void CallMyFunction(uintptr_t h) {
    go_callback_int(h, 5);
}
*/
import "C"

//export go_callback_int
func go_callback_int(h C.uintptr_t, p1 C.int) {
	fn := cgo.Handle(h).Value().(func(C.int))
	fn(p1)
}

type Test struct {
}

func (t *Test) MyCallback(x C.int) {
	fmt.Println("callback with", x)
}

func main() {
	t := &Test{}

	h := cgo.NewHandle(t.MyCallback)
	C.CallMyFunction(C.uintptr_t(h))
	h.Delete()
}
