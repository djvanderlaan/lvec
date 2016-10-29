
### Unify classes

We currently have a `cppr::logical` and a `cppr::boolean`; a `cppr::rstring`
and `std::string`; a `cppr::integer` and a `int`; a `cppr::numeric` and
a `double`. Try to unify this: one of each type. This might also reduce the
code in `as_rvec` for example. 

For cppr::rstring this has been resolved: std::string is now used and
cppr::rstring no longer exists. 

### Add Boost headers and source files to project

Currently a globally installed Boost installation is needed. Add the necessary
files to the src directory. I have already tried this with boost 1.62. However,
this resulted in compilation errors. Using the old Boost files from ldat, there
are warnings but no errors. It seems bcp doesn't copy all neccessary files. 

# Support long vectors

Currently the length of vectors is supplied to and from R using integers. R
supports long vectors (2^52 length). As far as I know, this is done using
doubles as indices, but this has to be looked up. This also means the indices
for assignent and indexing need to support double indices. 

(http://adv-r.had.co.nz/C-interface.html)[http://adv-r.had.co.nz/C-interface.html]
As of R 3.0.0, R vectors can have length greater than 231 − 1. This means that
vector lengths can no longer be reliably stored in an int and if you want your
code to work with long vectors, you can’t write code like int n = length(x).
Instead use the `R_xlen_t` type and the `xlength()` function, and write
`R_xlen_t n = xlength(x)`.

# Set and get size of vectors

length.lvec has been written. length<-.lvev not yet. 

# Rename `as.lvec` to `as_lvec` etc.
