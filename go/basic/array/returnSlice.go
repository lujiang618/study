package array

import (
	"fmt"
	"testing"
)

type A struct {
	A []int
}

// 0xc000018480
// 0xc000018480
// 返回结构体时，不会复制切片的值
func TestRead(t *testing.T) {
	a := abc()
	fmt.Printf("%p\n", a.A)
}

func abc() A {
	a := A{[]int{0, 1, 2, 3, 4, 5}}
	fmt.Printf("%p\n", a.A)

	return a
}
