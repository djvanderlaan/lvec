


Currently the length of vectors is supplied to and from R using integers. R
supports long vectors (2^52 length). As far as I know, this is done using
doubles as indices, but this has to be looked up. This also means the indices
for assignent and indexing need to support double indices.

From http://adv-r.had.co.nz/C-interface.html

    As of R 3.0.0, R vectors can have length greater than 231 − 1. This means that
    vector lengths can no longer be reliably stored in an int and if you want your
    code to work with long vectors, you can’t write code like int n = length(x).
    Instead use the R_xlen_t type and the xlength() function, and write
    R_xlen_t n = xlength(x).



