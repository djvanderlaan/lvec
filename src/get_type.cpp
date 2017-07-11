#include "cppr.h"
#include "ldat.h"
#include "lvec.h"
#include "r_export.h"

class type_visitor : public ldat::lvec_visitor {
  public: 

    type_visitor() : result_(cppr::na<std::string>()) {
    }

    void visit(ldat::lvec<double>& vec) {
      result_ = "numeric";
    }

    void visit(ldat::lvec<int>& vec) {
      result_ = "integer";
    }

    void visit(ldat::lvec<cppr::boolean>& vec) {
      result_ = "logical";
    }

    void visit(ldat::lvec<std::string>& vec) {
      result_ = "character";
    }

    const std::string& result() const {
      return result_;
    }

  private:
    std::string result_;
};


extern "C" {
  SEXP get_type(SEXP rv) {
    CPPRTRY
    type_visitor visitor{};
    ldat::vec* v = sexp_to_vec(rv);
    v->visit(&visitor);
    cppr::rvec<cppr::character> result{1};
    result[0] = visitor.result();
    return result.sexp();
    CPPRCATCH
  }
}


