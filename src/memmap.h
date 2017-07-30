#ifndef memmap_h
#define memmap_h

#include "tempfile.h"

#include <string>
#include <cstring>
#include <fstream>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

class MemMap {
  public:
    MemMap(const MemMap& mmap) : size_(mmap.size_),
        file_size_(mmap.file_size_), filename_("") {
      if (filename_ == "") filename_ = tempfile();
      boost::interprocess::file_mapping::remove(filename_.c_str());
      // set minimum file size to 8 bytes; hopefully this avoids issues with zero length vectors on 
      // solaris
      if (file_size_ < 8) file_size_ = 8;
      resize_file(filename_, file_size_, true);
      mapping_ = boost::interprocess::file_mapping(filename_.c_str(), boost::interprocess::read_write);
      region_ = boost::interprocess::mapped_region(mapping_, boost::interprocess::read_write, 0, size_);
      std::memcpy(data(), mmap.data(), size_);
    }

    MemMap(std::size_t size = 0, const std::string& filename = "") : size_(size),
        file_size_(size), filename_(filename) {
      if (filename_ == "") filename_ = tempfile();
      boost::interprocess::file_mapping::remove(filename_.c_str());
      // set minimum file size to 8 bytes; hopefully this avoids issues with zero length vectors on 
      // solaris
      if (file_size_ < 8) file_size_ = 8;
      resize_file(filename_, file_size_, true);
      mapping_ = boost::interprocess::file_mapping(filename_.c_str(), boost::interprocess::read_write);
      region_ = boost::interprocess::mapped_region(mapping_, boost::interprocess::read_write, 0, size_);
    }

    ~MemMap() {
      if (filename_ != "")
        boost::interprocess::file_mapping::remove(filename_.c_str());
    }

    std::size_t size() const {
      return size_;
    }

    void size(std::size_t size) {
      if (size == size_) return;
      if (size > file_size_) {
        resize_file(filename_, size);
        file_size_ = size;
      }
      size_ = size;
      region_ = boost::interprocess::mapped_region(mapping_, boost::interprocess::read_write, 0, size_);
    }

    const char* data() const {
      return reinterpret_cast<char*>(region_.get_address());
    }

    char* data() {
      return reinterpret_cast<char*>(region_.get_address());
    }

    MemMap& operator=(const MemMap& other) {
      if (this != &other) {
        if (other.size_ != size_) {
          size(other.size_);
        }
        std::memcpy(data(), other.data(), size_);
      } 
      return *this;
    }

    MemMap& operator=(MemMap&& other) {
      // open the file of other
      mapping_ = std::move(other.mapping_);
      region_ = std::move(other.region_);
      filename_ = other.filename_;
      size_ = other.size_;
      file_size_ = other.file_size_;
      // make sure other doesn't delete the file
      other.filename_ = "";
      return *this;
    }

  protected: 
    void resize_file_win(const std::string& filename, long long int size, bool truncate = false) {
      std::fstream out(filename, std::ios::binary | std::ios::ate | std::ios::app);
      // deterimine current size of out
      std::fstream::pos_type current_size = out.tellp();
      if (current_size == std::fstream::pos_type(-1)) current_size = 0;
      // initialise buffer with data with which to fille file
      const long long int max_buffer_size = 1024*1024*1024;
      long long int buffer_size = std::min(size+20, max_buffer_size);
      char* buffer = new char[buffer_size];
      for (long long int i = 0; i < buffer_size; ++i) buffer[i] = 0;
      // fill file
      long long int remain = size - current_size + 1;
      while (remain > 0) {
        std::size_t to_write = std::min(buffer_size-1, remain);
        out.write((const char*)buffer, to_write);
        remain -= to_write;
      }
      out.close();
      delete [] buffer;
    }

    void resize_file_lin(const std::string& filename, std::size_t size, bool truncate = false) {
      std::filebuf fbuf;
      if (truncate) {
        fbuf.open(filename, std::ios_base::in | std::ios_base::out |
          std::ios_base::trunc | std::ios_base::binary);
      } else {
        fbuf.open(filename, std::ios_base::in | std::ios_base::out |
          std::ios_base::binary);
      }
      fbuf.pubseekoff(size-1, std::ios_base::beg);
      fbuf.sputc(0);
    }

    void resize_file(const std::string& filename, std::size_t size, bool truncate = false) {
#if defined(_WIN32) || defined(__CYGWIN__)
      return resize_file_win(filename, size, truncate);
#else
      return resize_file_lin(filename, size, truncate);
#endif
    }

  private:
    std::size_t size_;
    std::size_t file_size_;
    std::string filename_;

    boost::interprocess::file_mapping mapping_;
    boost::interprocess::mapped_region region_;
};

#endif

