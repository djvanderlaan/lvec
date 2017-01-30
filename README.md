
lvec: out of memory vectors in R
================================

Core functionality for working with vectors (numeric, integer, logical and
character) that are too large to keep in memory. The vectors are kept
(partially) on disk using memory mapping. This package contains the basic
functionality for working with these memory mapped vectors (e.g.  creating,
indexing, ordering and sorting) and provides C++ headers which can be used by
other packages to extend the functionality provided in this package.




Functionality
-------------

### Creating and converting to and from lvecs

The core function to create an lvec is `lvec`. It accepts two arguments: an
length (`size`) and a type (`type`) which can have the following values:
`numeric`, `integer`, `logical` and `character`. This will create an lvec with
the given length of the given type. In case of `character` you will also have to
specify a maximum string length (`strlen`) as strings are stored with a fixed
width to speed up access. When assigning to a character lvec strings longer than
the maximum string length are truncated.

Create an integer vector of length 100

```r
x <- lvec(100, type = "integer")
```
Get the first 10 values. Values are initialised to 0 by default.

```r
lget(x, 1:10)
```

```
## integer lvec of length 10:
##  [1] 0 0 0 0 0 0 0 0 0 0
```
Set the first 10 values to 11:20

```r
lset(x, 1:10, 11:20)
```

```
## integer lvec of length 100:
##    [1] 11 12 13 14 15 16 17 18 19 20  0  0  0  0  0  0  0  0  0  0 
##   ... 
##   [81]  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
```
Set maximum length of the string to 1, strings longer than that get truncated.
However, minimum value of strlen is 2 (this is needed to be able to store
missing values).

```r
x <- lvec(10, type = "character", strlen = 1)
lset(x, 1:3, c("a", "foo", NA))
```

```
## character lvec of length 10:
##  [1] "a"  "fo" NA   ""   ""   ""   ""   ""   ""   ""
```

```r
lget(x, 1:3)
```

```
## character lvec of length 3:
## [1] "a"  "fo" NA
```

An other function for creating lvecs, is `as_lvec` which will try to convert a
R-vector (e.g. a numeric, integer, logical or character vector) to an lvec of
the corresponding type. In case of a character vector the maximum string length
is set equal to the maximum string length found in the input. 

Convert a character vector to lvec:

```r
x <- as_lvec(letters)
lget(x, 1:26)
```

```
## character lvec of length 26:
##   [1] a b c d e f g h i j k l m n o p q r s t 
##  ... 
##   [7] g h i j k l m n o p q r s t u v w x y z
```

Finally, most functions operating on lvecs will return an lvec. In order to work
directly in R with a result, this result needs to be converted to a regular
R-vector. For this the function `as_rvec` can be used, which will take an lvec
and return a vector of type `numeric`, `integer`, `logical` or `character`.

Convert `x`, defined above, back to an R-vector:

```r
as_rvec(x)
```

```
##  [1] "a" "b" "c" "d" "e" "f" "g" "h" "i" "j" "k" "l" "m" "n" "o" "p" "q"
## [18] "r" "s" "t" "u" "v" "w" "x" "y" "z"
```


### Reading from and writing to lvecs

Subsets of an lvec can be selected using `lget`. Three types of selection are
possible: using numeric indices and logical indices (as one would index an
reguler R-vector), or using a range index (using the `range` argument). A range
index is a numeric vector of length two giving the lower and upper bound of the
range one selects (inclusive).

Getting the first 3 elements of an lvec

```r
x <- as_lvec(1:5)
lget(x, 1:3)
```

```
## integer lvec of length 3:
## [1] 1 2 3
```

```r
lget(x, c(TRUE, TRUE, TRUE, FALSE, FALSE))
```

```
## integer lvec of length 3:
## [1] 1 2 3
```

```r
lget(x, range = c(1,3))
```

```
## integer lvec of length 3:
## [1] 1 2 3
```
`lget` always returns an lvec. When a regular R vector is needed `as_rvec` can
be used to convert the lvec.

Values can be written (assigned) to an lvec using `lset`. The first parameter is
the lvec, the second the (numeric or logical) indices and the last the new
values. This function should behave as regular assignent in R (e.g. 
`x[1:3] <- 4` should behave as `lset(x, 1:3, 4)`).

Changing the first three elements:

```r
lset(x, 1:3, 33:31)
```

```
## integer lvec of length 5:
## [1] 33 32 31  4  5
```
When indexing with a numeric index, values are recycled:

```r
lset(x, 1:3, 100)
```

```
## integer lvec of length 5:
## [1] 100 100 100   4   5
```
When indexing with a logical index, the index is also recycled (to the length of
the lvec) For example, setting the odd elements of an lvec to 1:

```r
lset(x, c(TRUE, FALSE), 1)
```

```
## integer lvec of length 5:
## [1]   1 100   1   4   1
```

For both `lset` and `lget` the index and values can also be other lvecs. When
they are not lvecs they are first converted to lvecs using `as_lvec`. 

### Working with lvecs

The length of an lvec can be obtained and changed using `length`:

```r
x <- as_lvec(1:5)
length(x)
```

```
## [1] 5
```

```r
length(x) <- 3
print(x)
```

```
## integer lvec of length 3:
## [1] 1 2 3
```

```r
length(x) <- 5
print(x)
```

```
## integer lvec of length 5:
## [1] 1 2 3 4 5
```

The maximum string length of an character lvec can be obtained and changed using
`strlen`. When changing the maximum string length to a smaller value, string
longer than the maximum string length are truncated. 

```r
x <- as_lvec(c("foo", "bar", "foobar"))
strlen(x)
```

```
## [1] 6
```

```r
strlen(x) <- 3
print(x)
```

```
## character lvec of length 3:
## [1] "foo" "bar" "foo"
```

`is_lvec` can be used to check if an object is an lvec. With `lvec_type` the
exact type of the lvec can be obtained: `numeric`, `integer`, `logical` or
`character`. 

Lvec objects can be seen as references to a piece of memory. Therefore, when an
lvec is copied only the reference is copied and not the actual data; the copy
refers to the same memory as the original. The advantage of this is that copying
is fast (no data needs to be copied); the disadvantage is that modifying the
copy will also modify the original. This behaviour is different from that of
other R objects. This behaviour is demonstrated below:

```r
introduce_missing_values <- function(x) {
  lset(x, c(TRUE, FALSE), NA)
  x
}

a <- as_lvec(1:5)
print(a)
```

```
## integer lvec of length 5:
## [1] 1 2 3 4 5
```

```r
b <- introduce_missing_values(a)
print(b)
```

```
## integer lvec of length 5:
## [1] NA  2 NA  4 NA
```

```r
print(a)
```

```
## integer lvec of length 5:
## [1] NA  2 NA  4 NA
```
With the `clone` function a true copy can be made of an lvec, it will create a
new lvec and copy the data from the original lvec. Using `clone` the function in
the example above can be given behaviour that is more in line with the behaviour
of regular R functions:

```r
introduce_missing_values <- function(x) {
  res <- clone(x)
  lset(res, c(TRUE, FALSE), NA)
  res
}

a <- as_lvec(1:5)
print(a)
```

```
## integer lvec of length 5:
## [1] 1 2 3 4 5
```

```r
b <- introduce_missing_values(a)
print(b)
```

```
## integer lvec of length 5:
## [1] NA  2 NA  4 NA
```

```r
print(a)
```

```
## integer lvec of length 5:
## [1] 1 2 3 4 5
```
When speed is an issue, you could introduce a `clone` argument with a default
value of `TRUE`. This will allow a user to skip the clone when it is not
needed:

```r
introduce_missing_values <- function(x, clone = TRUE) {
  if (clone) x - clone(x)
  lset(x, c(TRUE, FALSE), NA)
  x
}
a <- as_lvec(1:100)
a <- introduce_missing_values(a, clone = FALSE)
```

### Ordering and sorting

When working with large vectors (that don't fit into memory) ordering and
sorting are important operations. Therefore, `order` and `sort` are also
implemented for lvecs. These functions should behave as the reguler R functions.
However, sorting of character lvecs is implemented in C++, and will therefore
sorts using the local C-locale, which might be different from the locale used in
R. 


```r
x <- as_lvec(rnorm(10))
order(x)
```

```
## numeric lvec of length 10:
##  [1]  8  9  6  5  2  1  4 10  3  7
```

```r
lget(x, order(x))
```

```
## numeric lvec of length 10:
##  [1] -1.98799452 -0.72938604 -0.36763236 -0.21464091 -0.08568923
##  [6]  0.08462956  0.35395664  0.38670618  1.16447178  1.53825021
```

```r
sort(x)
```

```
## numeric lvec of length 10:
##  [1] -1.98799452 -0.72938604 -0.36763236 -0.21464091 -0.08568923
##  [6]  0.08462956  0.35395664  0.38670618  1.16447178  1.53825021
```
### Working with attributes

In order to have support for factor and POSIXct (dates and times) objects, lvec
tries to keep track of the original R attributes. A factor is internally stores
as an integer vector with a `levels` and `class` attribute. A POSIXct object is
a numeric vector with a `class` attribute. The attributes of the original R
objects can be obtained and modifier using the `rattr` functions. When
converting an lvec to an R vector, the attributes stored in rattr are applied to
the R object.


```r
x <- as_lvec(factor(sample(letters[1:3], 10, replace=TRUE)))
print(x)
```

```
## integer lvec of length 10:
##  [1] a c b b a b b b a a
## Levels: a b c
```

```r
rattr(x)
```

```
## $levels
## [1] "a" "b" "c"
## 
## $class
## [1] "factor"
```

```r
rattr(x, "class")
```

```
## [1] "factor"
```

```r
as_rvec(x)
```

```
##  [1] a c b b a b b b a a
## Levels: a b c
```

```r
x <- as_lvec(as.POSIXct("2016-01-01", "2015-03-22"))
print(x)
```

```
## numeric lvec of length 1:
## [1] "2016-01-01"
```

```r
rattr(x)
```

```
## $class
## [1] "POSIXct" "POSIXt" 
## 
## $tzone
## [1] "2015-03-22"
```

```r
rattr(x, "class")
```

```
## [1] "POSIXct" "POSIXt"
```

```r
as_rvec(x)
```

```
## [1] "2016-01-01"
```

```r
x <- as_lvec(1:3)
rattr(x, "class") <- "factor"
rattr(x, "levels") <- c("a", "b", "c")
print(x)
```

```
## integer lvec of length 3:
## [1] a b c
## Levels: a b c
```

### Reading and writing lvecs to disk

lvec objects can be written to disk and read back using `lsave` and `lread`. The
lvec is divided into a number of chunks. Each chunk is written to an separate
RDS file (see `saveRDS`). Finally, the names of the files, size of the lvec and
some additional information to correctly read back the lvec is written to an RDS
file. The filenames of the chunks have the following format:
`<filename>.00001.RDS`, `<filename>.00002.RDS` etc.; and the file containing the
background information has the name: `<filename>.RDS`. When the filename given
to `lsave` has the extension `RDS` this extension is first removed. The lvec can
be read back into memory using `lread` passing it the file containing the
background information (`<filename>.RDS`).


```r
x <- as_lvec(1:10)
lsave(x, "tmp.RDS")
list.files(pattern = "*.RDS")
```

```
## [1] "tmp.00001.RDS" "tmp.RDS"
```

```r
y <- lload("tmp.RDS")
print(y)
```

```
## integer lvec of length 10:
##  [1]  1  2  3  4  5  6  7  8  9 10
```


Linking to lvec from another R package
---------------------------------------

The lvec package contains only the basic functionality for working with lvec
objects. The goal is that other packages can extend the functionality of the
lvec package. Although most extensions can probably be built directly in R code
using the functionality provided in lvec, sometimes it will be necessary to work
directly with the C++ objects behing the lvec object. To support this most
headers and some C++ functions are exported. Other packages can use these by
having 

```
LinkingTo:
    lvec
```
in their `DESCRIPTION` file. This makes the header files available for use by
the package. They can the include all headers using:
```
#include <lvec_interface.h>
```
in their C++ code. 


