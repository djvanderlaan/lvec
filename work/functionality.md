
Essential routines to be implemented (partially) in C++
-------------------------------------------------------

####Creating an lvec
Destruction is done automatically by the garbage collector of R.

####Indexing using lvec returning lvec
- Indexing using logical vector
- Indexing using numeric/integer vector

####Range indexing returning lvec

####Assignment of an lvec to an lvec indexed by an lvec
- Indexing can be with a logical and numeric lvec
- Assignment to an character lvec should give warnings when strings are
  truncated. 
- Assignment without index (`a[] <- b`) can be translated in R by an assignment
  with logical index (`a[TRUE] <- b`). 

####Cloning

####Converting r-vector to lvec
Needed because all other functions return or expect lvecs. 

####Convert lvec to r-vector
Needed because all other functions return or expect lvecs. 

####Sorting
Needed? Order is used much more often. Partial sort can be useful for
calculating quantiles.

####Calculate order


Essential routines to be implemented completely in R
----------------------------------------------------

####Casting between different types of lvecs
Can be implemented using `creating an lvec` combined with `assignment of
an lvec to an lvec indexed by an lvec`.

####Change string length of character lvec
Or can this also be done using `creating an lvec` combined with `assignment of
an lvec to an lvec indexed by an lvec`? Actually this is an variant of `casting
between different types of lvecs`. 

####Binary and unary operators


Open questions
----------------------------------------------------

####Do we implement different storage types?
For logical lvecs it might be a option to immediately implement using bytes as
storage types. Converting to and from r-vectors will be slower. Operations using
lvecs will be faster. 

