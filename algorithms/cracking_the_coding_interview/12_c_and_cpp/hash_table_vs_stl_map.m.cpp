// compare and contrast a hash table and an STL map. How is a hash table 
// implemented? If the number of inputs is small, which data structure options 
// can be used instead of a hash table?

// hash table has many differnet ways of implementation, one example could be
// using separate chaining, aka a vector of linked lists. There is also open
// addressing which is how Python implements it. 

// when data structure is small, a vector/list can be used. Search can be done
// either through binary search or linear search and its lightweight can make 
// up for the asymptoic complexity loss. In fact in small data structure case, 
// it could be a gain. 

// STL map is implemented using a Red-Black tree, guaranteeing logN complexity

// Hash Table points:
// 1) not sorted
// 2) O(1) lookup complexity
// 3) potential collision, handled by for example chaining
// 4) resizing to maintain a well spread out memory distribution

// STL map points:
// 1) binary search tree, in fact red-black tree
// 2) O(N logN) guaranteed
