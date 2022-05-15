#include "ShmArray.h"

namespace shm_array {
unsigned int type_to_size(char dtype) {
  switch(dtype) {
    case 'b':     // bool
    case 'B':     // bool
    case 'c':     // bool
    case '?':     // bool
      return 1;
    case 'h':     // short
    case 'H':     // unsigned short
      return 2;
    case 'i':     // int
    case 'I':     // unsigned int
    case 'l':     // long
    case 'L':     // unsigned long
    case 'f':     // float
      return 4;
    case 'q':     // long long
    case 'Q':     // unsigned long long
    case 'd':     // double
      return 8;
    default:
      throw std::runtime_error("unknown dtype");
  }
}

ShmArray::ShmArray(ShmArray &&) noexcept = default;
ShmArray &ShmArray::operator=(ShmArray &&) noexcept = default;

ShmArray ShmArray::Create(const std::string &name, char dtype, const std::vector<unsigned int> &dims) {
  size_t header_size = 4 * (dims.size() + 1);
  size_t item_size = type_to_size(dtype);
  if (header_size % item_size != 0) {
    header_size += item_size - header_size % item_size;
  }
  size_t data_count = 1;
  for (auto dim : dims) {
    data_count *= dim;
  }
  size_t total_size = header_size + data_count * item_size;
  SharedMemory shm = SharedMemory::Create(name, total_size);
  void *addr = shm.Address();
  static_cast<char *>(addr)[0] = dtype;
  static_cast<char *>(addr)[1] = dims.size();
  uint32_t *shapes = static_cast<uint32_t  *>(addr) + 1;
  for (size_t i = 0; i < dims.size(); i++) {
    shapes[i] = dims[i];
  }
  return ShmArray(std::move(shm));
}

ShmArray ShmArray::Open(const std::string &name, Permission permission) {
  SharedMemory shm = SharedMemory::Open(name, permission);
  return ShmArray(std::move(shm));
}

ShmArray::ShmArray(SharedMemory &&shm) : shm_(std::move(shm)) {
  const auto *header = static_cast<const Header *>(shm_.Address());
  dtype_ = header->dtype;
  shape_.resize(header->dims);
  for (size_t i = 0; i < header->dims; i++) {
    shape_[i] = header->shape[i];
  }
  size_t header_size = 4 * (header->dims + 1);
  size_t item_size = type_to_size(dtype_);
  if (header_size % item_size != 0) {
    header_size += item_size - header_size % item_size;
  }
  address_ = static_cast<char *>(shm_.Address()) + header_size;
}
std::vector<uint32_t> ShmArray::Shape() const {
  return shape_;
}
char ShmArray::Dtype() const {
  return dtype_;
}

void *ShmArray::Data() {
  return address_;
}
} // shm_array