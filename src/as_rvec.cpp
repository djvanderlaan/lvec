#include "ldat.h"
#include "r_export.h"
#include <cstring>
#include <stdexcept>

class as_rvec_visitor : public ldat::lvec_visitor {
  public:
    as_rvec_visitor() : rvec_(0) {
    }

    void visit(ldat::lvec<double>& vec) {
      // TODO: possible optimisation: use memcpy
      cppr::rvec<cppr::numeric> res(vec.size());
      for (ldat::vec::vecsize i = 0; i < vec.size(); ++i)
        res[i] = vec.get(i);
      rvec_ = PROTECT(res.sexp());
    }

    void visit(ldat::lvec<int>& vec) {
      // TODO: possible optimisation: use memcpy
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
        const std::string val = vec.get(i);
        res[i] = val;
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
