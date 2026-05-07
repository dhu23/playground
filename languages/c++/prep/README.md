# Preparation Topics
## C++17 essentials
- `auto`, `decltype`, structured bindings
- `if constexpr`, fold expressions
- `std::optional`, `std::variant`, `std::any`

## Memory management & RAII
- Smart pointers (`unique_ptr`, `shared_ptr`, `weak_ptr`)
- Move sematics (`std::move`, `std::forward`)
- Rule of 5/3/0, object lifetimes

## Class design basics
- Constructors, destructors, copy/move assignments
- Const correctness, `mutable`, `explicit`, `constexpr`

## Templates & generics
- Function templates, class templates, template specialization
- `std::enable_if`, SFINAE, concepts (C++20)

## STL mastery
- Containers: `vector`, `map`, `unordered_map`, `deque`, `set`, `unordered_set`, `list`, `proiority_queue`
- Agorithms: `sort`, `lower_bound`, `upper_bound`, `find_if`, `transform`, `accumulate`, `partition`
- Iterators, range-based loops, `emplace` vs `push_back`

## C++ "Tricky" Topics
- References vs pointers
- Lambdas and captures (`[]`, `[=]`, `[&]`, `[this]`)
- Move semantics subtleties
- perfect forwarding
- allocator behavior
- Const correctness, `constexpr`, `if constexpr` usage
- Function overloading & operator overloading rules
- Undefined behavior traps(dangling pointers, iterator invalidation)
- Template metaprogramming / SFINAE edge cases (C++20 concept)

## Multithreading / Concurrency
- Threads, mutexes, condition variables
- `std::async`, `std::future`, thread-safe containers
- Thread pool, scheduler
- Lock-free structure and programming

## C++ polymorphism
- Runtime polymorphism
  - Modern improvements
    - always use `overrides`
    - always have a virtual destructor
    - use smart pointers
    - avoid raw ownership
  - When to use
    - Heterogeneous collections
    - plugin-style systems
    - you need runtime flexibility
  - Tradeoffs
    - Indirection (vtable)
    - Heap allocation often involved
    - Harder to optimize (a major cost in modern codebase) 
- Compile-time polymorphism (templates/concepts)
  - templates
  - C++20 concepts
  - Why is it modern?
    - no vtable overhead
    - fully inlined
    - type-safe constraints (concepts)
  - When to use?
    - Performance-critical code
    - Known types at compile time
    - Generic libraries
  - Tradeoffs
    - Code bloat (template instantiations)
    - cannot store different types easily in one container
- Type-erasure (modern alternative to virtual)
  - 

## C++ Problem Solving (Senior)
### Some selection from Cracking the Coding Interview
### Dynamic Programming modeling
#### Foundational 
- Coin Change
- Longest Increasing Subsequence
- Longest Common Subsequence
- Edit Distance
- House Robber I & II
#### Intermediate
- Partition Equal Subset Sum
- Target Sum
- Decode Ways
- Word Break
- Unique Paths II
- Boolean DP
- 0/1 knapsack pattern recognition
- Prefix decomposition thinking
#### Higher
- Burst Balloons
- Palindrome Partitioning II
- Longest Palindromic Substring
- Distinct Subsequences

### Backtracking state design
- Clean recursion
- state mutation control
- passing containers correctly
- avoid copies accidentally
#### Core
- Permutations
- Subsets
- Combination Sum
- N-Queues
- Word Search

### Fold/Prefix/Accumualtion-type
- Maximum Subarray (Kadane)
- Subarray Sum Equals K
- Product of Array Except self
- Trapping Rain Water
- Largest Rectangle in Histogram
- Sliding Window Maximum
- Daily Temperatures

### Tree/Graph
- Binary Tree Level Order
- Lowest Common Ancestor
- Validate BST
- Number of Islands
- Course Schedule
- Clean DFS/BFS implementation
- passing state correctly
- Avoiding point mistakes

### C++-Specific Drill List
#### Write These from Scratch
- LRU Cache (with list + unordered_map)
- Thread-safe queue
- Templated Matrix<T> class
- Custom comparator for priority_queue
- Implement `shared_ptr` (simplified)

#### C++ "Reflex" Exercises
- Perfect forwarding example
- `enable_if` example
- Lambda capture edge cases
- Rule-of-5 class
- Demonstrate iterator invalidation bug
