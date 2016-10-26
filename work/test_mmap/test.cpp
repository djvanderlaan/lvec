#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <exception>
#include "memmap.h"

class IntMemMap {
  public:
    IntMemMap(std::size_t size, const std::string& filename = "") :
        mmap_(size * sizeof(int), filename), size_(size), data_(0) {
      data_ = reinterpret_cast<int*>(mmap_.data());
    }

    std::size_t size() const {
      return size_;
    }

    void size(std::size_t size) {
      mmap_.size(size * sizeof(int));
      size_ = size;
      data_ = reinterpret_cast<int*>(mmap_.data());
    }

    int get(std::size_t pos) const {
      if (pos < 0 || pos >= size_)
        throw std::runtime_error("Index out of bounds");
      return data_[pos];
    }

    void set(std::size_t pos, int val)  {
      if (pos < 0 || pos >= size_)
        throw std::runtime_error("Index out of bounds");
      data_[pos] = val;
    }


  private:
    MemMap mmap_;
    std::size_t size_;
    int* data_;
};

int main(int argc, char** argv) {
  IntMemMap map{1000001};
  int even = 1;
  for (int i = 0; i < map.size(); ++i) {
    map.set(i, even);
    even = -even;
  }
  int sum = 0;
  for (int i = 0; i < map.size(); ++i) {
    sum += map.get(i);
  }
  std::cout << sum << std::endl;

  IntMemMap map2(map);
  map2.size(200);
  sum = 0;
  for (int i = 0; i < map2.size(); ++i) {
    sum += map2.get(i);
  }
  std::cout << sum << std::endl;
  sum = 0;
  for (int i = 0; i < map.size(); ++i) {
    sum += map.get(i);
  }
  std::cout << sum << std::endl;

  return 0;
}
