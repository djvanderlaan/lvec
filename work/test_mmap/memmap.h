#ifndef memmap_h
#define memmap_h

#include <string>
#include <boost/iostreams/device/mapped_file.hpp>

class MemMap {
  public:
    MemMap(const MemMap& mmap);
    MemMap(std::size_t size = 0, const std::string& filename = "");
    ~MemMap();

    std::size_t size() const;
    void size(std::size_t size);

    const char* data() const;
    char* data();

  private:
    std::size_t size_;
    std::string filename_;
    boost::iostreams::mapped_file file_;
};

#endif
