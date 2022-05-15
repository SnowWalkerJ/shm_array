#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include "../SharedMemory.h"
#include "../ShmArray.h"
#include <boost/interprocess/shared_memory_object.hpp>

namespace py = pybind11;
using namespace shm_array;
using namespace pybind11::literals;


PYBIND11_MODULE(shm_array, m) {
  py::class_<SharedMemory>(m, "RawShm", py::buffer_protocol())
      .def_property_readonly_static("create", [](const py::object&) {
        return py::cpp_function(&SharedMemory::Create, "name"_a, "size"_a);
      })
      .def_property_readonly_static("open", [](const py::object&) {
        return py::cpp_function([](const std::string &name, bool read_only) {
          Permission permission = read_only ? shm_array::READONLY : shm_array::READWRITE;
          return SharedMemory::Open(name, permission);
        }, "name"_a, "read_only"_a = false);
      })
      .def("size", &SharedMemory::Size)
      .def_buffer([](SharedMemory &shm) {
        return py::buffer_info(
            shm.Address(),
            1,
            py::format_descriptor<char>::format(),
            1,
            {shm.Size()},
            {1}
            );
      })
      ;
  py::class_<ShmArray>(m, "ShmArray", py::buffer_protocol())
      .def_property_readonly_static("create", [](const py::object&) {
        return py::cpp_function(&ShmArray::Create, "name"_a, "dtype"_a, "shape"_a);
      })
      .def_property_readonly_static("open", [](const py::object&) {
        return py::cpp_function([](const std::string &name, bool read_only) {
          Permission permission = read_only ? shm_array::READONLY : shm_array::READWRITE;
          return ShmArray::Open(name, permission);
        }, "name"_a, "read_only"_a = false);
      })
      .def_property_readonly("shape", &ShmArray::Shape)
      .def_property_readonly("itemsize", [](const ShmArray &shm) { return type_to_size(shm.Dtype()); })
      .def_property_readonly("format", [](const ShmArray &shm) { return std::string(1, shm.Dtype()); })
      .def_buffer([](ShmArray &shm) {
        auto shape32 = shm.Shape();
        std::vector<py::ssize_t> shape(shape32.begin(), shape32.end());
        std::vector<py::ssize_t> stride(shape.size());
        size_t item_size = type_to_size(shm.Dtype());
        py::ssize_t n = item_size;
        for (long i = shape.size() - 1; i >= 0; i--) {
          stride[i] = n;
          n *= shape[i];
        }
        std::string dtype(1, shm.Dtype());
        return py::buffer_info(
            shm.Data(),
            static_cast<py::ssize_t>(item_size),
            dtype,
            static_cast<py::ssize_t>(shape.size()),
            shape,
            stride
            );
      })
      ;
  m.def("unlink", [](const std::string &name) {
    boost::interprocess::shared_memory_object::remove(name.c_str());
  });
}