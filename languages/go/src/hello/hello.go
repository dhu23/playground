package main

import (
    "fmt"
)

// basic types:
// bool, string
// int, int8, int16, int32, int64
// uint, uint8, uint16, uint32, uint64, uintptr
// byte(unit8)
// rune(int32), like a char
// float32, float64
// complex64, complex128


// other types:
// Array
// Slice
// Struct
// Pointer
// Function
// Interface
// Map
// Channel

func main() {
    var message string
    message = "Hello Go World!"

    var a, b, c int = 1, 2, 3
    var e, d = 2, false
    f, g := true, "hello!"

    fmt.Println(message, a, b, c, e, d, f, g)
}
