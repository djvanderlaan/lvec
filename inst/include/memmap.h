#ifndef memmap_h
#define memmap_h

#include "tempfile.h"

#include <string>
#include <cstring>
#include <fstream>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

//#include <iostream>

const std::size_t MINMMAPSIZE = 1024*1024*2*8;

class MemMap {
  public:
    MemMap(const MemMap& mmap) : size_(mmap.size_), 
        filename_(""), buffer_(0) {
      if (size_ <= MINMMAPSIZE) {
        open_buffer();
      } else {
        open_file();
      }
      if (mmap.data()) std::memcpy(data(), mmap.data(), size_);
    }

    MemMap(std::size_t size = 0, const std::string& filename = "") : size_(size),
        filename_(filename), buffer_(0) {
      if (size_ <= MINMMAPSIZE) {
        open_buffer();
      } else {
        open_file();
      }
    }

    ~MemMap() {
      if (buffer_) {
        delete [] buffer_;
      } else {
        //std::cout << "CLOSEFILE '" << filename_ << "'\n";
        boost::interprocess::file_mapping::remove(filename_.c_str());
      }
    }

    std::size_t size() const {
      return size_;
    }

    void size(std::size_t size) {
      if (size == size_) return;

      if (buffer_) {

        if (size > MINMMAPSIZE) {
          // we now have to open a file and copy the data from the buffer to
          // the file
          // TODO make exception safe
          size_t old_size = size_;
          size_ = size;
          open_file();
          std::memcpy(reinterpret_cast<char*>(region_.get_address()), buffer_, old_size);
          delete [] buffer_;
          buffer_ = 0;
        } else {
          // resize the buffer
          // TODO make exception safe
          char* new_buffer = new char[size]();
          std::memcpy(new_buffer, buffer_, std::min(size_, size));
          delete [] buffer_;
          buffer_ = new_buffer;
          size_ = size;
        }
      } else {
        resize_file(filename_, size);
        size_ = size;
        region_ = boost::interprocess::mapped_region(mapping_, boost::interprocess::read_write, 0, size_);
      }
    }

    const char* data() const {
      if (buffer_) {
        return buffer_;
      } else {
        return reinterpret_cast<char*>(region_.get_address());
      }
    }

    char* data() {
      if (buffer_) {
        return buffer_;
      } else {
        return reinterpret_cast<char*>(region_.get_address());
      }
    }

    MemMap& operator=(const MemMap& other) {
      if (this != &other) {
        if (other.size_ != size_) size(other.size_);
        if (other.data()) std::memcpy(data(), other.data(), size_);
      } 
      return *this;
    }

    MemMap& operator=(MemMap&& other) {
      // remove current file
      boost::interprocess::file_mapping::remove(filename_.c_str());
      // open the file of other
      mapping_ = std::move(other.mapping_);
      region_ = std::move(other.region_);
      filename_ = other.filename_;
      size_ = other.size_;
      // make sure other doesn't delete the file
      other.filename_ = "";
      // move buffer
      buffer_ = other.buffer_;
      if (buffer_) delete [] buffer_;
      return *this;
    }

  protected: 
    void open_file() {
      if (filename_ == "") filename_ = tempfile();
      boost::interprocess::file_mapping::remove(filename_.c_str());
      resize_file(filename_, size_, true);
      //std::cout << "OPENFILE '" << filename_ << "'\n";
      mapping_ = boost::interprocess::file_mapping(filename_.c_str(), boost::interprocess::read_write);
      region_ = boost::interprocess::mapped_region(mapping_, boost::interprocess::read_write, 0, size_);
    }

    void open_buffer() {
      if (buffer_ != 0) delete [] buffer_;
      buffer_ = 0;
      buffer_ = new char[size_]();
    }

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
    // file buffer
    std::string filename_;
    boost::interprocess::file_mapping mapping_;
    boost::interprocess::mapped_region region_;
    // memory buffer
    char* buffer_;

};

#endif

