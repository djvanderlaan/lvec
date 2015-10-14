#include "cppr.h"
#include "lvec.h"

static void vec_finalizer(SEXP rp) {
  if(!R_ExternalPtrAddr(rp)) return;
  void* p = R_ExternalPtrAddr(rp);
  ldat::vec* v = reinterpret_cast<ldat::vec*>(p);
  delete v;
  R_ClearExternalPtr(rp);
}

SEXP vec_to_sexp(ldat::vec* vec) {
  if (!vec) return R_NilValue;
  cppr::protSEXP res = R_MakeExternalPtr(vec, install("lvec"), R_NilValue);
  R_RegisterCFinalizerEx(res, vec_finalizer, TRUE);
  return res;
}

ldat::vec* sexp_to_vec(SEXP rvec) {
  if(!R_ExternalPtrAddr(rvec)) return 0;
  void* p = R_ExternalPtrAddr(rvec);
  return reinterpret_cast<ldat::vec*>(p);
}

extern "C" {
  SEXP new_lvec(SEXP rsize) {
    CPPRTRY
    cppr::rvec<cppr::integer> size(rsize);
    if (size.length() == 0) throw std::runtime_error("No size given.");
    ldat::vec* vec = new ldat::lvec<double>(size[0]);
    return vec_to_sexp(vec);
    CPPRCATCH
  }
}

