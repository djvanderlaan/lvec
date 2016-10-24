#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <exception>
#include <boost/filesystem.hpp>
#include <boost/iostreams/device/mapped_file.hpp>


class FilenameFactory {
  public:
    static void tempdir(const std::string& tempdir) {
      tempdir_ = tempdir;
    }

    static std::string tempdir() {
      return tempdir_;
    }

    static std::string tempfile() {
      for (unsigned int i = 0; i < 20; ++i) {
        boost::filesystem::path path = tempdir();
        boost::filesystem::path temp = boost::filesystem::unique_path();
        path /= temp;
        if (!exists(path)) return path.string();
      }
      throw 1;
    }


  private:
    static std::string tempdir_;
};

std::string FilenameFactory::tempdir_ = "";

class MemMap {
  public:
    MemMap() : size_(0), filename_() {
    }

    // TODO: for this we need a way to get temporary filenames;
    //MemMap(const MemMap& mmap) {
    //}

    MemMap(std::size_t size, const std::string& filename = "") : size_(size),
        filename_(filename) {
      // create file
      if (filename_ == "") filename_ = FilenameFactory::tempfile();
      // TODO: what is file creation or resize fails
      std::ofstream tmp(filename_, std::ios::out | std::ios::binary);
      tmp.close();
      boost::filesystem::resize_file(filename_, size_);
      file_.open(filename_);
    }

    ~MemMap() {
      boost::filesystem::remove(filename_);
    }

    std::size_t size() const {
      return size_;
    }

    void size(std::size_t size) {
      file_.close();
      boost::filesystem::resize_file(filename_, size);
      size_ = size;
      file_.open(filename_);
    }

    const void* data() const {
      return size_ ? file_.data() : 0;
    }

    void* data() {
      return size_ ? file_.data() : 0;
    }

  private:
    std::size_t size_;
    std::string filename_;
    boost::iostreams::mapped_file file_;
};

class IntMemMap {
  public:
    IntMemMap(std::size_t size, const std::string& filename = "") :
        mmap_(size * sizeof(int), filename), size_(size), data_(0) {
      data_ = reinterpret_cast<int*>(mmap_.data());
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
  //FilenameFactory::tempdir("foo");
  std::cout << "tempdir ='" << FilenameFactory::tempfile() << "'" << std::endl;
  return 0;
}
