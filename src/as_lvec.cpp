#include "ldat.h"
#include <cstring>
#include <stdexcept>

extern "C" {
  SEXP as_lvec(SEXP rv) {
    CPPRTRY
    if (cppr::is<cppr::numeric>(rv)) {
      double* v = REAL(rv);
      R_xlen_t l = cppr::numeric::length(rv);
      auto res = new ldat::lvec<double>(l);
      std::memcpy(res->data(), v, l * sizeof(double));
      return vec_to_sexp(res);
    } else if (cppr::is<cppr::integer>(rv)) {
      int* v = INTEGER(rv);
      R_xlen_t l = cppr::integer::length(rv);
      auto res = new ldat::lvec<int>(l);
      std::memcpy(res->data(), v, l * sizeof(int));
      return vec_to_sexp(res);
    } else if (cppr::is<cppr::logical>(rv)) {
      // create lvec
      cppr::rvec<cppr::logical> v{rv};
      auto res = new ldat::lvec<cppr::boolean>(v.length());
      for (ldat::vec::vecsize i = 0; i < v.length(); ++i) {
        int val = v[i];
        if (cppr::is_na(val)) res->set(i, cppr::na<cppr::boolean>());
        else res->set(i, val);
      }
      return vec_to_sexp(res);
    } else if (cppr::is<cppr::character>(rv)) {
      cppr::rvec<cppr::character> v{rv};
      // determine max string length
      int max_len = 0;
      for (ldat::vec::vecsize i = 0; i < v.length(); ++i) {
        std::string s = v[i];
        if (!cppr::is_na(s)) {
          int l = s.length();
          if (l > max_len) max_len = l;
        }
      }
      // the minimum string length is always 2 as we need to be able to store
      // missing values which have a length of 2.
      if (max_len < 2) max_len = 2;
      // create lvec
      auto res = new ldat::lvec<std::string>(v.length(), max_len);
      for (ldat::vec::vecsize i = 0; i < v.length(); ++i) {
        std::string str = v[i];
        res->set(i, str);
      }
      return vec_to_sexp(res);
    } else {
      throw std::runtime_error("Unknown type. Unable to transform to lvec.");
    }
    return R_NilValue;
    CPPRCATCH
  }
}
