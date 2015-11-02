#ifndef boolean_h
#define boolean_h

namespace ldat {

  class boolean {
    public:
      boolean() : val_(2) {}
      boolean(const boolean& bl) : val_(bl.val_) {}
      boolean(bool val) : val_(val ? 1 : 0) {}
      
      bool na() const { return val_ == 2;}
      void na(bool na) { val_ = na ? 2 : 0;}
      void set(bool val) { val_ = val ? 1 : 0;}

      boolean& operator=(bool bl) {
	set(bl);
	return *this;
      }

      boolean& operator=(const boolean& bl) {
	val_ = bl.val_;
	return *this;
      }

      operator bool() const { return val_ == 1;}
      operator double() const { return val_ == 1 ? 1.0 : 0.0;}
      operator int() const { return val_ == 1 ? 1 : 0;}

    private:
      unsigned char val_;
  }; 

}

namespace cppr { 
  inline bool is_na(const ldat::boolean& x) { return x.na();}
  inline bool is_nan(const ldat::boolean& x) { return x.na();}
}

#endif
