#ifndef boolean_h
#define boolean_h

namespace cppr {

  class boolean {
    public:
      boolean() : val_(2) {}
      boolean(const boolean& bl) : val_(bl.val_) {}
      boolean(int val) : val_(val != 0) {
        if (cppr::is_na(val)) val_ = 2;
      }
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

  inline bool is_na(const boolean& x) { return x.na();}
  inline bool is_nan(const boolean& x) { return x.na();}

  inline double base_type(double v) { return double();}
  inline int base_type(int v) { return int();}
  inline int base_type(const boolean& v) { return int();}
  inline std::string base_type(const std::string& v) { return std::string();}
}

#endif
