#ifndef boolean_h
#define boolean_h

#include "cppr.h"

namespace ldat {

  class boolean {
    public:
      boolean() : val_(2) {}
      boolean(const boolean& bl) : val_(bl.val_) {}
      boolean(int val) : val_(val != 0) {
        if (is_na(val)) val_ = 2;
      }
      boolean(bool val) : val_(val ? 1 : 0) {}

      bool na() const { return val_ == 2;}
      void na(bool na) { val_ = na ? 2 : 0;}
      void set(bool val) { val_ = val ? 1 : 0;}

      boolean& operator=(bool bl) {
        set(bl);
        return *this;
      }

      boolean& operator=(int val) {
        if (is_na(val)) {
          val_ = 2;
        } else val_ = val != 0;
        return *this;
      }

      boolean& operator=(const boolean& bl) {
        val_ = bl.val_;
        return *this;
      }

      operator bool() const { return val_ == 1;}

      operator double() const { 
        switch (val_) {
          case 1: return 1.0;
          case 2: return ldat::na<double>();
          default: return  0.0;
        }
      }

      operator int() const { 
        switch (val_) {
          case 1: return 1;
          case 2: return ldat::na<int>();
          default: return  0;
        }
      }

    private:
      unsigned char val_;
  };

  inline bool is_na(const boolean& x) { return x.na();}
  inline bool is_nan(const boolean& x) { return x.na();}
  inline int base_type(const boolean& v) { return int();}

  inline bool operator<(const boolean& lhs, const boolean& rhs)  {
    if (lhs.na()) return false;
    if (rhs.na()) return true;
    return static_cast<bool>(lhs) < static_cast<bool>(rhs);
  }

  inline bool operator==(const boolean& lhs, const boolean& rhs)  {
    if (lhs.na() && rhs.na()) return true;
    if (lhs.na() || rhs.na()) return false;
    return static_cast<bool>(lhs) == static_cast<bool>(rhs);
  }


}

#endif
