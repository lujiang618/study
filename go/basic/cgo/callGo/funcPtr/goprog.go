package main

/*
#cgo CFLAGS: -I .
#cgo LDFLAGS: -L . -lclibrary

#include "clibrary.h"

int callOnMeGo_cgo(int in); // Forward declaration.
*/
import "C"

import (
	"fmt"
	"unsafe"
)

//export callOnMeGo
func callOnMeGo(in int) int {
	fmt.Printf("go called with arg = %d\n", in)
	return in + 1
}

func main() {
	C.ON((C.callback_fcn)(unsafe.Pointer(C.callOnMeGo_cgo)))
}
