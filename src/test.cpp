#include "cppr.h"
#include <iostream>

/*static void vec_finalizer(SEXP rp) {
  if(!R_ExternalPtrAddr(rp)) return;
  void* p = R_ExternalPtrAddr(rp);
  Vec* v = reinterpret_cast<Vec*>(p);
  delete v;
  R_ClearExternalPtr(rp);
}

SEXP vec_to_sexp(Vec* vec) {
  if (vec) {
    SEXP res = R_MakeExternalPtr(vec, install("lvec"), R_NilValue);
    PROTECT(res);
    R_RegisterCFinalizerEx(res, vec_finalizer, TRUE);
    UNPROTECT(1);
    return res;
  }
  return R_NilValue;
}

Vec* sexp_to_vec(SEXP rvec) {
  if(!R_ExternalPtrAddr(rvec)) return 0;
  void* p = R_ExternalPtrAddr(rvec);
  return reinterpret_cast<Vec*>(p);
}

extern "C" {

  SEXP new_lvec(SEXP rsize) {
    int* size = INTEGER(rsize);
    Vec* vec = new LVec<cppr::numeric>(size[0]);
    return vec_to_sexp(vec);
  }

  SEXP get_range(SEXP rvec, SEXP rrange) {
    cppr::rvec<cppr::integer> range{rrange};
    if (range.length() < 2) ; // TODO
    Vec* vec = sexp_to_vec(rvec);
    if (!vec) ; // TODO
    // subtract 1 from range indices as R indexing is 1 based
    Vec* res = vec->get_range(range[0]-1, range[1]-1);
    return vec_to_sexp(res);
  }

  SEXP to_vec(SEXP rvec) {
    cppr::rvec<cppr::numeric> vec{rvec};
    DoubleVec* res = new DoubleVec(vec.length());
    for (unsigned int i = 0; i < vec.length(); ++i) {
      res->set(i, vec[i]);
    }
    return vec_to_sexp(res);
  }

  SEXP to_vec2(SEXP rvec) {
    double* vec = REAL(rvec);
    unsigned int length = LENGTH(rvec);
    DoubleVec* res = new DoubleVec(length);
    std::memcpy(res->data(), vec, length * sizeof(double));
    return vec_to_sexp(res);
  }
}

*/
