#include "memmap.h"
#include "filenamefactory.h"

#include <boost/filesystem.hpp>
#include <exception>
#include <cstring>

MemMap::MemMap(const MemMap& mmap) : size_(mmap.size_),
    filename_(FilenameFactory::tempfile()) {
  boost::filesystem::copy_file(mmap.filename_, filename_);
  file_.open(filename_);
}

MemMap::MemMap(std::size_t size, const std::string& filename) : size_(size),
    filename_(filename) {
  // mapped_file throws an error when the file size is equal to 0; set the 
  // minimum size to 1
  if (size_ == 0) size_ = 1;
  if (filename_ == "") filename_ = FilenameFactory::tempfile();
  boost::iostreams::mapped_file_params params;
  params.path = filename_;
  params.new_file_size = size_;
  params.flags = boost::iostreams::mapped_file::mapmode::readwrite;
  file_.open(params);
}

MemMap::~MemMap() {
  file_.close();
  // when file doesn't exist remove doesn't give an error and that's ok
  if (filename_ != "") boost::filesystem::remove(filename_);
}

std::size_t MemMap::size() const {
  return size_;
}

void MemMap::size(std::size_t size) {
  if (size == size_) return;
  // mapped_file throws an error when the file size is equal to 0; set the 
  // minimum size to 1
  if (size == 0) size = 1;
  file_.close();
  boost::system::error_code ec;
  boost::filesystem::resize_file(filename_, size, ec);
  if (!ec) size_ = size;
  file_.open(filename_);
  if (ec) throw std::runtime_error("Failed to resize memory map.");
}

const char* MemMap::data() const {
  return file_.data();
}

char* MemMap::data() {
  return file_.data();
}

MemMap& MemMap::operator=(const MemMap& other) {
  if (this != &other) {
    if (other.size_ != size_) {
      size(other.size_);
    }
    std::memcpy(data(), other.data(), size_);
  } 
  return *this;
}

MemMap& MemMap::operator=(MemMap&& other) {
  // open the file of other
  file_.close();
  filename_ = other.filename_;
  size_ = other.size_;
  file_.open(filename_);
  // make sure other doesn't delete the file
  other.filename_ = "";
  other.size_ = 0;
  other.file_.close();
  return *this;
}

