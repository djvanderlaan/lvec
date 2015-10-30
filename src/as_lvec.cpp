#include "ldat.h"
#include <cstring>
#include <stdexcept>

extern "C" {
  SEXP as_lvec(SEXP rv) {
    CPPRTRY
    if (cppr::is<cppr::numeric>(rv)) {
      double* v = REAL(rv);
      int l = LENGTH(rv);
      auto res = new ldat::lvec<double>(l);
      std::memcpy(res->data(), v, l * sizeof(double));
      return vec_to_sexp(res);
    } else if (cppr::is<cppr::integer>(rv)) {
      int* v = INTEGER(rv);
      int l = LENGTH(rv);
      auto res = new ldat::lvec<int>(l);
      std::memcpy(res->data(), v, l * sizeof(int));
      return vec_to_sexp(res);
    } else if (cppr::is<cppr::logical>(rv)) {
      throw std::runtime_error("Unsupported type: logical. Unable to transform to lvec.");
    } else if (cppr::is<cppr::character>(rv)) {
      cppr::rvec<cppr::character> v{rv};
      // determine max string length
      int max_len = 0;
      for (int i = 0; i < v.length(); ++i) {
        cppr::rstring s = v[i];
        int l = s.length();
        if (l > max_len) max_len = l;
      }
      if (max_len < 2) max_len = 2;
      // create lvec
      auto res = new ldat::lvec<std::string>(v.length(), max_len);
      for (int i = 0; i < v.length(); ++i) {
        cppr::rstring str = v[i];
        if (cppr::is_na(str)) res->set(i, cppr::na<std::string>());
        else res->set(i, str);
      }
      return vec_to_sexp(res);
    } else {
      throw std::runtime_error("Unknown type. Unable to transform to lvec.");
    }
    return R_NilValue;
    CPPRCATCH
  }
}

// TODO strlen with missing values
