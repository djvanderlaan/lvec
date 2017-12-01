#ifndef ldat_h
#define ldat_h

#include "../inst/include/cppr.h"
#include "../inst/include/lvec.h"

//TODO: with rcpp sexp_to_vec and vec_to_sexp no longer necessry
ldat::vec* sexp_to_vec(SEXP rvec);
SEXP vec_to_sexp(ldat::vec* vec);

#endif
