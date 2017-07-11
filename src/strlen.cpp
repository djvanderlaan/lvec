#include "cppr.h"
#include "ldat.h"
#include "lvec.h"
#include "r_export.h"

class strlen_visitor : public ldat::lvec_visitor {
  public: 

    strlen_visitor() : result_(cppr::na<int>()) {
    }

    void visit(ldat::lvec<double>& vec) {
      throw std::runtime_error("Vector is not a character vector.");
    }

    void visit(ldat::lvec<int>& vec) {
      throw std::runtime_error("Vector is not a character vector.");
    }

    void visit(ldat::lvec<cppr::boolean>& vec) {
      throw std::runtime_error("Vector is not a character vector.");
    }

    void visit(ldat::lvec<std::string>& vec) {
      result_ = vec.strlen();
    }

    int result() const {
      return result_;
    }

  private:
    int result_;
};

extern "C" {
  SEXP get_strlen(SEXP rv) {
    CPPRTRY
    strlen_visitor visitor{};
    ldat::vec* v = sexp_to_vec(rv);
    v->visit(&visitor);
    cppr::rvec<cppr::integer> result{1};
    result[0] = visitor.result();
    return result.sexp();
    CPPRCATCH
  }
}


