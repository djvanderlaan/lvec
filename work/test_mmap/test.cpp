#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <exception>
#include <boost/filesystem.hpp>
#include <boost/iostreams/device/mapped_file.hpp>


class MemMap {
  public:
    MemMap(std::size_t size, const std::string& filename) : size_(size),
        data_(0), filename_(filename) {
      // create file
      std::ofstream tmp(filename, std::ios::out | std::ios::binary);
      tmp.close();
      // resize file to correct size
      std::size_t filesize = size * sizeof(int);
      boost::filesystem::resize_file(filename, filesize);
      // open memory map
      file_.open(filename);
      data_ = reinterpret_cast<int*>(file_.data());
    }

    std::size_t size() const {
      return size_;
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
    std::size_t size_;
    int* data_;
    std::string filename_;
    boost::iostreams::mapped_file file_;
};

int main(int argc, char** argv) {
  MemMap map{100, "test.mm"};
  for (int i = 0; i < map.size(); ++i) {
    map.set(i, i);
  }
  return 0;
}
