include_directories($ENV{HOME}/opt/anaconda3/include)
find_package(pybind11 REQUIRED)
pybind11_add_module(shm_array shm_array.cpp ../SharedMemory.h ../SharedMemory.cpp ../ShmArray.h ../ShmArray.cpp)