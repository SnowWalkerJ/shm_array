#ifndef SHM_ARRAY__SHAREDMEMORY_H_
#define SHM_ARRAY__SHAREDMEMORY_H_
#include <string>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include "Common.h"
namespace shm_array {
namespace bi = boost::interprocess;

class SharedMemory {
 public:
  SharedMemory(const SharedMemory &) = delete;
  SharedMemory(SharedMemory &&) = default;
  static SharedMemory Create(const std::string &name, size_t size);
  static SharedMemory Open(const std::string &name, Permission permission);
  size_t Size() const;
  void *Address();
  const void *Address() const;
  SharedMemory &operator=(const SharedMemory &) = delete;
  SharedMemory &operator=(SharedMemory &&) = default;
 protected:
  SharedMemory(bi::shared_memory_object &&shm, bi::mapped_region &&region);
 private:
  bi::shared_memory_object shm_;
  bi::mapped_region region_;
};

} // shm_array

#endif //SHM_ARRAY__SHAREDMEMORY_H_
