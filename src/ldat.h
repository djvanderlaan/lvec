#ifndef ldat_h
#define ldat_h

#include "../inst/include/cppr.h"
#include "../inst/include/lvec.h"

ldat::vec* sexp_to_vec(SEXP rvec);
SEXP vec_to_sexp(ldat::vec* vec);

#endif
