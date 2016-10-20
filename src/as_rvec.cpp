#include "ldat.h"
#include <cstring>
#include <stdexcept>

class as_rvec_visitor : public ldat::lvec_visitor {
  public:
    as_rvec_visitor() : rvec_(0) {
    }

    void visit(ldat::lvec<double>& vec) {
      cppr::rvec<cppr::numeric> res(vec.size());
      for (ldat::vec::vecsize i = 0; i < vec.size(); ++i)
        res[i] = vec.get(i);
      rvec_ = PROTECT(res.sexp());
    }

    void visit(ldat::lvec<int>& vec) {
      cppr::rvec<cppr::integer> res(vec.size());
      for (ldat::vec::vecsize i = 0; i < vec.size(); ++i)
        res[i] = vec.get(i);
      rvec_ = PROTECT(res.sexp());
    }

    void visit(ldat::lvec<cppr::boolean>& vec) {
      cppr::rvec<cppr::logical> res(vec.size());
      for (ldat::vec::vecsize i = 0; i < vec.size(); ++i) {
        res[i] = cppr::cast_value<int>(vec.get(i));
      }
      rvec_ = PROTECT(res.sexp());
    }

    void visit(ldat::lvec<std::string>& vec) {
      cppr::rvec<cppr::character> res(vec.size());
      for (ldat::vec::vecsize i = 0; i < vec.size(); ++i) {
        std::string val = vec.get(i);
        // TODO: ifelse statement no longer nexessary
        if (cppr::is_na(val)) {
          res[i] = cppr::na<std::string>();
        } else {
          res[i] = val.c_str();
          // TODO: should be able to do res[i] = val; then the ifelse statement
          // is also no longer necessary
        }
      }
      rvec_ = PROTECT(res.sexp());
    }

    SEXP rvec() const {
      if (rvec_ == 0) throw std::runtime_error("Failed to convert.");
      UNPROTECT_PTR(rvec_);
      return rvec_;
    }

  private:
    SEXP rvec_;
};

extern "C" {
  SEXP as_rvec(SEXP rv) {
    CPPRTRY
    as_rvec_visitor visitor{};
    ldat::vec* v = sexp_to_vec(rv);
    v->visit(&visitor);
    return visitor.rvec();
    CPPRCATCH
  }
}
