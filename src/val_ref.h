#ifndef val_ref_h
#define val_ref_h

namespace ldat {

  template<typename T>
  class lvec;

  template<typename T>
  class lvec_val_ref {
    public:
      lvec_val_ref(lvec<T>& vec, unsigned int pos): vec_(vec), pos_(pos) {}
      lvec_val_ref(const lvec_val_ref& ref): vec_(ref.vec_), pos_(ref.pos_) {}

      lvec_val_ref& operator=(const T& val) { 
        vec_.set(pos_, val); 
        return *this;
      }

      operator T() const { 
        return vec_.get(pos_);
      }

      lvec_val_ref& operator=(const lvec_val_ref& ref) {
        vec_.set(pos_, ref);
        return *this;
      }

    private:
      lvec<T>& vec_;
      unsigned int pos_;
  };

  // TODO why is this not handled bu operator T() of val_rel?
  template<typename T>
  bool operator<(const lvec_val_ref<T>& lhs, const lvec_val_ref<T>& rhs) {
    return static_cast<T>(lhs) < static_cast<T>(rhs);
  }

  // TODO why is this not handled bu operator T() of val_rel?
  template<typename T>
  bool operator<(const lvec_val_ref<T>& lhs, const T& rhs) {
    return static_cast<T>(lhs) < rhs;
  }

  // TODO why is this not handled bu operator T() of val_rel?
  template<typename T>
  bool operator<(const T& lhs, const lvec_val_ref<T>& rhs) {
    return lhs < static_cast<T>(rhs);
  }


  // TODO: check if exception safe
  template<typename T>
  void swap(lvec_val_ref<T> a, lvec_val_ref<T> b) {
    lvec_val_ref<T> tmp = a;
    a = b;
    b = tmp;
  }

}

#endif
