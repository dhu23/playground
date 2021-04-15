// what is the different between deep copy and shallow copy? Explain how you 
// would use each. 

// in class copy constructor or copy assignment operator, shallow copy simply
// copies each member by value while a deep copy does deep copy for each member
// Shallow copy ends up shared dynamically allocated memory between objects
// while deep copies own their own versions/copies. 


// however shared pointer can be considered a way to manage shallow copy, with
// better mechanism on the resource deletion. 
