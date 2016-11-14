
checked:
  as_lvec.cpp
  as_rvec.cpp
  assign.cpp
  clone.cpp
  construct.cpp
  filenamefactory.cpp
  get_type.cpp
  indexing.cpp
  indexing_range.cpp
  order.cpp
  size.cpp
  sort.cpp




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



Vectors in R 2.x.y were limited to a length of 2^31 - 1 elements (about 2
billion), as the length is stored in the SEXPREC as a C int, and that type is
used extensively to record lengths and element numbers, including in packages.

Note that longer vectors are effectively impossible under 32-bit platforms
because of their address limit, so this section applies only on 64-bit
platforms. The internals are unchanged on a 32-bit build of R.

A single object with 2^31 or more elements will take up at least 8GB of memory
if integer or logical and 16GB if numeric or character, so routine use of such
objects is still some way off.

There is now some support for long vectors. This applies to raw, logical,
integer, numeric and character vectors, and lists and expression vectors.
(Elements of character vectors (CHARSXPs) remain limited to 2^31 - 1 bytes.)
Some considerations:

    This has been implemented by recording the length (and true length) as -1
    and recording the actual length as a 64-bit field at the beginning of the
    header. Because a fair amount of code in R uses a signed type for the
    length, the ‘long length’ is recorded using the signed C99 type ptrdiff_t,
    which is typedef-ed to R_xlen_t.

    These can in theory have 63-bit lengths, but note that current 64-bit OSes
    do not even theoretically offer 64-bit address spaces and there is currently
    a 52-bit limit (which exceeds the theoretical limit of current OSes and
    ensures that such lengths can be stored exactly in doubles).

    The serialization format has been changed to accommodate longer lengths, but
    vectors of lengths up to 2^31-1 are stored in the same way as before. Longer
    vectors have their length field set to -1 and followed by two 32-bit fields
    giving the upper and lower 32-bits of the actual length. There is currently
    a sanity check which limits lengths to 2^48 on unserialization.

    The type R_xlen_t is made available to packages in C header Rinternals.h:
    this should be fine in C code since C99 is required. People do try to use R
    internals in C++, but C++98 compilers are not required to support these
    types.

    Indexing can be done via the use of doubles. The internal indexing code used
    to work with positive integer indices (and negative, logical and matrix
    indices were all converted to positive integers): it now works with either
    INTSXP or REALSXP indices.

    R function length was documented to currently return an integer, possibly
    NA. A lot of code has been written that assumes that, and even code which
    calls as.integer(length(x)) before passing to .C/.Fortran rarely checks for
    an NA result.

    There is a new function xlength which works for long vectors and returns a
    double value if the length exceeds 2^31-1. At present length returns NA for
    long vectors, but it may be safer to make that an error.

