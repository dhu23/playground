# Q language fundamentals and their Haskell equivalent

Q lanaguge and the kdb+ system that builds upon Q language is a popular and 
powerful tool used in financial industry. It is a descent of APL and
is built upon K language. However, with its esoteric syntex, it is not always
easy to decipher each line of the code for a beginner. Considering that it is 
also an array language with functional programming elements, it can be used to 
produce powerful and succinct code that is harder to get a hang of. The way
it introduces high-order functions by using the concept of verbs can seem 
confusing and arbitrary without deep understanding of functional programming.
Here I attempt to interpret Q fundamentals with Haskell language. 
The reason I choose Haskell for this purpose is, as a pure functional language,
it has become my default way of reasoning any functional code. In addition, 
I believe the powerful type system of Haskell and clear functional sematics was 
very helpful for me to get a better grip of Q. Even though the importance of 
this article will for sure decrease over time as I get better with Q, 
it still serves as a good exercise for Haskell. 

The interesting feature that Q provides as an array language are indexing at 
depth and index eliding. Similar features are also available in other Q 
competitors in analysis area, such as Python Numpy/Pandas and R language. 

Before we dive into the detailed comparison or reimplementation of Q features
by using Haskell, we look at some of the interesting features that Q offers:

A noticeable feature in `q` is the the atoms data and atomic functions.
Like in the ancient Greek physics, an atom is an irreduciable value of 
a specific data type. (see [the full list of atoms][atom-list]). Note that
q *operator*, *function* and *verb* are the same thing and are used
interchangeably generally, by purists. Operators are built-in functions used 
with in-fix notation. Verbs are higher order functions that modify function 
behavior. In Q, monadic functions take one argument and dyadic functions take
two arguments. The word *monaidic* is used differently in Haskell which is 
associated with *monad*. In newer standard of Q, monadic funcitons are
functions of rank 1 and dyadic functions are that of rank 2. 

## Atomic Function
### Monaidic Atomic Function
```q
/ neg is a monadic atomic function
neg 10    / -10
neg 10 20 30      / -10 -20 -30
neg (10; (20; 30))     
/ -10
/ -20 -30
neg `a`b`c!10 20 30     
/ produces
/ a| -10
/ b| -20
/ c| -30
neg `a`b!((1;2;(3;4));(11 12 13))
/ produces
/ a| -1  -2  -3 -4
/ b| -11 -12 -13
```

The take-away here is that, a monadic atomic function would recursively apply
to the bottom of the data structure until it reaches every single atoms. It 
somewhat resembles `fmap` to a recursive data structure in Haskell. This is 
also an excellent demonstration of the similarity between list and dictionary,
where a list can be seen as a dictionary with its indices as keys and a 
dictionary can be seen as a list with non-numeric indices. They share similar
interface and essentially both mathematical mappings. 

### Dyadic Atomic Function
There are two subcases for dyadic functions. 
- atomic in one of the argument. For example `?`, there are two overloaded 
cases:
    - `find`: find is atomic in only its second argument and it consumes
the first argument as the list to search. 
    - random: `?` takes a list as its second argument and generate the number
of random sample based on the first argument. 
```q
1 2 3?1 / 0, found at index=0
1 2 3?4 / 3, meaning not found
5?0 1 2 / 0 0 2 0 1
```
When functions like this, that take one atom and one list as the arguments, 
the case can be thought of as a monadic atomic function if you fix the 
non-atomic argument. In other words, projecting the function or partially 
applying the function so that it becomes a monadic atomic function. For 
example, function `in` with the second argument `0 1 2 3 4` can be thought 
as an atomic function that checks whether the input atom is in the given list.
```q
0 in 0 1 2 3 4 / 1b
0 1 10 in 0 1 2 3 4 / 110b
```
However this is a little tricky for funciton `?` as it is overloaded, 
demonstrated above. The massive overloading of everything/anything in Q is not
my favorite feature. With two lists as input, `?` still functions as `find`, 
finding each of the element in the second list in the first list. 
```q
1 2 3?1 2 3 4 5 / 0 1 2 3 3
```

- The other case is when both operands are atomic. For example `+`, `*` are of
this kind. In practice, this can bring about more subcases 
    - atom with atom
    - atom with list 
    - list with atom
    - list with list

The first case is dyadic atomic function of its basic form. The second and the
third can be demonstrated with the following examples, where they can be 
thought of as a monadic atomic function, created by projecting, recursively 
applying through the list. 
```q
/ list with atom 
10 20 30+1  / 11 21 31
/ atom with list
1+10 20 30  / 11 21 31
/ atom wiht list
1+(10;(20; 30))
/ produces
/ 11
/ 21 31
/ list with atom
(10;(20; 30))+1
/ produces 
/ 11
/ 21 31
```

The last case is specialy. The two lists much have the same shape and it is 
similar to `zip` in Haskell, instead of zipping through two Haskell lists, 
it can zip two functors of the same shape.
```q
1 2 3+4 5 6  / 5 7 9
(1;(2 3))+(4;(5 6)) 
/ produces
/ 5
/ 7 9
d1:`a`b!((1;2 3);4 5)
d2:`a`b!((11;22 33);44 55)
d1+d2
/ produces
/ a| 12 24 36
/ b| 48 60
```
## Adverb
### Monadic each
The classic example for this is `count`. It is not a monadic function as it takes
a list as its argument and produces one atomic value for the length. `each` would 
apply the function along the top level of the nested structure, instead of on the 
structure itself. This is similar to monadic atomic function extending to the 
whole structure, except for that the function itself is not atomic. 
```q
count 1 2 3    / 3
count (1 2 3; 4 5) / 2 as count is applied on the structure itself
count each (1 2 3; 4 5)  / 3 2 as it is applied along the top level
/ f each listdata is the general form
/ @[listdata;::;f] does the same thing through general application verb @
```
In Haskell, this can be implemented as `fmap` on the nested structure. 


### each-both '

### each-left \:

### each-right /:

### cross product

### over /

### scan \

### each-previous ':

```haskell
map :: (a -> b) -> [a] -> [b]
```
`map` maps a function over a list of elements of one type to a list of 
elements of another type. Conceptually if you map a function `f`  over a 
list `[x1, x2 ... xn]`, you end up with `[f x1, f x2 ... f xn]`. 
For example:

```haskell
map show [1, 2, 3, 4] -- produces ["1", "2", "3", "4"]
map (+1) [1, 2, 3, 4] -- produces [2, 3, 4, 5]
```
Note that `(+1)` is a partial application that is equivalent to this lambda
function `\ x -> x + 1`.

```haskell
zip :: [a] -> [b] -> [(a, b)]
zip3 :: [a] -> [b] -> [c] -> [(a, b, c)]
```
`zip` merges two lists to create a list of tuples, with the total length 
equal to the length of the shorter list. For example `[x1, x2 ... xn]` and
`[y1, y2 ... yn]` would be zipped up to form `[(x1, y1), (x2, y2) ... (xn, yn)]`
by `zip`. `zip3` would zip up three lists and produce a list of tuples in 
`[(x1, y1, z1), (x2, y2, z2) ... (xn, yn, zn)]` form. These two are provided 
in `Prelude` by default. `Data.List` module provides more general forms `zip4`,
`zip5`, `zip6` and `zip7`.

```haskell
zip [1, 2, 3] "ABC" -- produces [(1, 'A'), (2, 'B'), (3, 'C')]
zip3 [1..3] "ABC" [1.1, 2.2, 3,3] 
-- produces [(1, 'A', 1.1), (2, 'B', 2.2), (3, 'C', 3.3)]
```
Note that `zip` can also be achieved by using `ZipList` defined in 
`Control.Applicatve`. For example:

```haskell
import Control.Applicative

-- equivalent to zip
myZip :: [a] -> [b] -> [(a, b)]
myZip xs ys = getZipList $ (,) <$> ZipList xs <*> ZipList ys

-- equivalent to zip3
myZip3 :: [a] -> [b] -[c] -> [(a, b, c)]
myZip3 xs ys zs = getZipList $ (,,) <$> ZipList xs <*> ZipList ys <*> ZipList zs
```

`zipWith` merges two lists and applys a function 
```
zipWith :: (a -> b -> c) -> [a] -> [b] -> [c]

foldl :: Foldable t => (b -> a -> b) -> b -> t a -> b
```

A noticeable feature in `q` is the the atoms data and atomic functions.
Like in the ancient Greek physics, an atom is an irreduciable value of 
a specific data type. (see [the full list of atoms][atom-list]). Note that
q *operator*, *function* and *verb* are the same thing and are used
interchangeably generally, by purists. Operators are built-in functions used 
with in-fix notation. In Haskell it is different such that any function that
takes more than two arguments can be invoked in such way. For example:

```haskell
line :: Num a => a -> a -> a -> a
line a b x = a * x + b

f1 = line 1 2 -- creates function of x+2
f2 = 1 `line` 2 -- creates function of x+2

f1 1 -- prints 3
f2 1 -- prints 3
```
However this is not applicable in Q. 

Function application is generally done using bracket but in case of a 
monadic function `f`, meaning that it takes only one argument, it 
can be written as `f x` (similar to Haskell) rather than `f[x]`. 

An atomic funciton acts recursively on data structures. For example,
applying an atomic functions to a list is the same as applying it to 
each item in the list, in other words, it extends itself to the entire list. 
```q
20+1 / 21
20+1 2 3 / 21 22 23
neg 1 2 3 / -1 -2 -3

x:((1;(2 3));((4 5);6))
20+x / ((21;(22 23));((24 25);26))
neg x / ((-1;(-2 -3));((-4 -5);-6))
```

Equivalently this can be thought as a `fmap` of atomic funciton in a functor. 
In fact we can use the following Haskell snippet to reason this q feature.
```haskell
import Data.Char

-- Int, Char and Bool are considered Atom type
data Atom = I Int | C Char | B Bool deriving Show
-- A Q data type is either a simple type of itself or a list of Q types
data Q a = S a | L [Q a] deriving Show

instance Functor Q where
    fmap f (S a) = S (f a)
    fmap f (L qs) = L $ map (fmap f) qs

-- example code:
toIntQ = S . I
toCharQ = S . C

-- to simple Q list
toQ1 f = L . map f
-- to Q 2d list
toQ2 f = toQ1 (toQ1 f)

-- not yet a total function. only for demonstration
fromCtoI :: Atom -> Atom
fromCtoI (C c) = I $ digitToInt c

-- not yet a total function. only for demonstration
liftInt :: (Int -> Int) -> Atom -> Atom
liftInt f (I x) = I (f x)

-- not yet a total funciton. only for demonstration
neg :: Atom -> Atom
neg (I x) = I (negate x)

simpleList1 = toQ1 toIntQ [1, 2, 3]
complexList1 = toIntQ2 [[1, 2], [3, 4]]
complexList2 = L $ 
    [ L $ [toIntQ 1, toQ1 toIntQ [2, 3]]
    , L $ [toQ1 toIntQ [4, 5], toIntQ 6]
    ]

main = do
    print simpleList1 -- L [S (I 1), S (I 2), S (I 3)]
    print complexList1 -- L [L [S (I 1), S (I 2)], L [S (I 3), S (I 4)]]
    -- L [L [(S (I 1), L [S (I 2), S (I 3)]], [L [S (I 4), S (I 5)], S (I 6)]]
    print complexList2
    print $ fmap add20 simpleList1 -- L [S (I 21), S (I 22), S (I 23)]
    -- L [L [S (I 21), S (I 22)], L [S (I 23), S (I 24)]]
    print $ fmap add20 complexList1
    -- L [ L [S (I 21), L [S (I 22), S (I 23)]]
    --   , L [L [S (I 24), S (I 25)], S (I 26)]
    --   ]
    print $ fmap add20 complexList2
    -- L [ L [S (I (-1)), L [S (I (-2)), S (I (-3))]]
    --   , L [L [S (I (-4), S (I (-5))]], S (I (-6))]
    --   ]
    print $ fmap neg complexList2
    where add20 = liftInt (+20)
```

[atom-list]:https://code.kx.com/q4m3/2_Basic_Data_Types_Atoms/
