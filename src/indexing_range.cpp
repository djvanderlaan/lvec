#include "ldat.h"
#include "lvec.h"
#include <memory>

class range_indexing_visitor : public ldat::lvec_visitor {
  public: 
    range_indexing_visitor(ldat::vec& index) : index_(index), result_(0) { }

    template<typename T>
    void visit_template(ldat::lvec<T>& vec) {
    }

    void visit(ldat::lvec<double>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::lvec<int>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::lvec<cppr::boolean>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::lvec<std::string>& vec) {
      return visit_template(vec);
    }

    ldat::vec* result() { 
      return result_;
    }

  private:
    ldat::vec& index_;
    ldat::vec* result_;
};

// TODO: range index
extern "C" {
  SEXP get_range(SEXP rv, SEXP rindex) {
    CPPRTRY
    cppr::rvec<cppr::numeric> index{rindex};
    if (index.length() != 2)
      throw std::runtime_error("Expecting vector of length 2 for range index.");
    if (index[1] < index[0])
      throw std::runtime_error("Top of range is smaller than bottom.");
    return R_NilValue;

    //ldat::vec* index = sexp_to_vec(rindex);
    //indexing_visitor visitor{*index};
    //ldat::vec* v = sexp_to_vec(rv);
    //v->visit(&visitor);
    //return vec_to_sexp(visitor.result());
    CPPRCATCH
  }
}

