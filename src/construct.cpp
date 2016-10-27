#include "ldat.h"

static void vec_finalizer(SEXP rp) {
  // TODO: use exp_to_vec
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
  if(!R_ExternalPtrAddr(rvec))
    throw std::runtime_error("Object is not an external pointer; failed to convert to lvec.");
  // TODO check if rvec is an externalptr of type lvec
  void* p = R_ExternalPtrAddr(rvec);
  return reinterpret_cast<ldat::vec*>(p);
}

extern "C" {
  SEXP new_lvec(SEXP rsize, SEXP rtype, SEXP rstrlen) {
    CPPRTRY
    cppr::rvec<cppr::integer> size{rsize};
    if (size.length() == 0) throw std::runtime_error("Size is empty.");
    cppr::rvec<cppr::character> type{rtype};
    if (type.length() == 0) throw std::runtime_error("Type is empty.");

    ldat::vec* vec = 0;
    std::string t = type[0];
    if (t == "numeric") {
      vec = new ldat::lvec<double>(size[0]);
    } else if (t == "integer") {
      vec = new ldat::lvec<int>(size[0]);
    } else if (t == "logical") {
      vec = new ldat::lvec<cppr::boolean>(size[0]);
    } else if (t == "character") {
      cppr::rvec<cppr::integer> strlen{rstrlen};
      if (strlen.length() == 0) throw std::runtime_error("Strlen is empty.");
      vec = new ldat::lvec<std::string>(size[0], strlen[0]);
    }
    return vec_to_sexp(vec);
    CPPRCATCH
  }
}
