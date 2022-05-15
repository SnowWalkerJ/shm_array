#ifndef SHM_ARRAY__SHMARRAY_H_
#define SHM_ARRAY__SHMARRAY_H_
#include <vector>
#include "SharedMemory.h"

namespace shm_array {

struct Header {
  char dtype;
  char dims;
  uint32_t shape[0];
};

unsigned int type_to_size(char dtype);

class ShmArray {
 public:
  explicit ShmArray(SharedMemory &&shm);
  ShmArray(const ShmArray &) = delete;
  ShmArray(ShmArray &&) noexcept;
  ShmArray &operator=(const ShmArray &) = delete;
  ShmArray &operator=(ShmArray &&) noexcept;
  static ShmArray Create(const std::string &name, char dtype, const std::vector<unsigned int> &dims);
  static ShmArray Open(const std::string &name, Permission permission);

  std::vector<uint32_t> Shape() const;
  char Dtype() const;
  void *Data();
 private:
  SharedMemory shm_;
  char dtype_;
  std::vector<uint32_t> shape_;
  void *address_;
};

} // shm_array

#endif //SHM_ARRAY__SHMARRAY_H_
