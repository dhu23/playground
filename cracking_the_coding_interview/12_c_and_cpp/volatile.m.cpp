// what is the significance of the keyword volatile in C

// The keyword volatile informs the compiler that the value of the value it is
// applied to can change from the outside, without any update done by the code.
// This may be done by the operating system, the hardware, or another thread. 
// Because the value can change unexpectedly, the compiler will therefore
// reload the value each time from memory.

int volatile volatileA;
volatile int volatileB;

volatile int* pointerToIntAToVolatile;
int volatile* pointerToIntBToVolatile;

int * volatile volatilePointerToIntA;
volatile int * volatile volatilePointerToVolatileIntA;

// compiler won't perform optimization on volatile variables
// can be very usefl when multi-threaded programs have global variables
