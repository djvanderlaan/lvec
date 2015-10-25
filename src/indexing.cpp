#include "ldat.h"
#include "lvec.h"

class indexing_visitor : public ldat::lvec_visitor {
  public: 
    indexing_visitor(ldat::vec& index) : index_(index), result_(0) {
    }

    template<typename T>
    void visit_template(ldat::lvec<T>& vec) {
      auto result = new ldat::lvec<T>(index_.size());
      for (ldat::vec::vecsize i = 0; i < index_.size(); ++i) {
        int index = index_.get_of_type(i, int());
        T value = vec.get(index - 1);
        result->set(i, value);
      }
      result_ = result;
    }

    void visit(ldat::lvec<double>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::lvec<int>& vec) {
      return visit_template(vec);
    }

    // TODO: following should also be templated; however currently the constructor
    // of string lvec's also expect a strlen parameter; we probably should define a
    // constructor that accepts an lvec and size; taking all properties except size
    // from the lvec passed to it. 
    void visit(ldat::lvec<std::string>& vec) {
      auto result = new ldat::lvec<std::string>(index_.size(), vec.strlen());
      for (ldat::vec::vecsize i = 0; i < index_.size(); ++i) {
        int index = index_.get_of_type(i, int());
        const std::string& value = vec.get(index - 1);
        result->set(i, value);
      }
      result_ = result;
    }

    ldat::vec* result() { 
      return result_;
    }

  private:
    ldat::vec& index_;
    ldat::vec* result_;
};

// TODO: range checking
extern "C" {
  SEXP get(SEXP rv, SEXP rindex) {
    CPPRTRY
    ldat::vec* index = sexp_to_vec(rindex);
    indexing_visitor visitor{*index};
    ldat::vec* v = sexp_to_vec(rv);
    v->visit(&visitor);
    return vec_to_sexp(visitor.result());
    CPPRCATCH
  }
}

