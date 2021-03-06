#ifndef SRC_KERNERLS_SASS_FUNCTION_H_
#define SRC_KERNERLS_SASS_FUNCTION_H_

#include <utility>
#include <cuda.h>
#include <map>

#include <boost/thread/once.hpp>
#include <boost/noncopyable.hpp>

#include "util/common.h"

namespace blitz {

class CudaLoadModule {
 public:
  CudaLoadModule() {
    cuInit(0);
    cuDeviceGet(&device_, 0);
    cuCtxCreate(&context_, CU_CTX_BLOCKING_SYNC, device_);

    // init kernels list
    const size_t kernel_size = 3;
    const string kernel_name[kernel_size] = {
      "sgemm_nn_128x128", 
      "sgemm_nt_128x128", 
      "sgemm_tn_128x128"}; 
   
    for (size_t i = 0; i < kernel_size; ++i) {
      const string& name = kernel_name[i];
      const string path = "./cubin/" + name + ".cubin";

      CUmodule module;
      CUfunction function;
      CUresult res;

      // load module
      res = cuModuleLoad(&module, path.c_str());
      if (res != CUDA_SUCCESS) {
        LOG(FATAL) << "Failed to load module: " << name << std::endl;
      }

      // load function
      res = cuModuleGetFunction(&function, module, name.c_str());
      if (res != CUDA_SUCCESS) {
        LOG(FATAL) << "Failed to load function: " << name << std::endl;
      }

      functions_[name] = function;
      modules_.push_back(module);
    }
  }

  ~CudaLoadModule() {
    typedef vector<CUmodule>::iterator ModuleIterator;
    for (ModuleIterator it = modules_.begin(); it != modules_.end(); ++it) {
      cuModuleUnload(*it);
    }
    cuCtxDestroy(context_);
  }

  CUfunction GetFunction(const string& name) {
    if (functions_.find(name) == functions_.end()) {
      LOG(FATAL) << "Cannot find kernel: " << name;
    }

    return functions_[name];
  }

 private:
  map<string, CUfunction> functions_;
  vector<CUmodule> modules_;

  CUcontext context_;
  CUdevice device_;
};

class CudaModule {
 public:
  static CudaLoadModule& GetInstance() {
    // thread safe
    boost::call_once(&CudaModule::Create, flag_);
    return *(CudaModule::instance_);
  }

  static void Create() {
    CudaModule::instance_.reset(new CudaLoadModule());
  }

  static CUfunction GetFunction(const string& name) {
    CudaLoadModule& cuda_load_module = CudaModule::GetInstance();
    return cuda_load_module.GetFunction(name);
  }
  
  virtual ~CudaModule();

 private:
  CudaModule();
  CudaModule(const CudaModule& cuda_module);
  CudaModule& operator=(const CudaModule& rhs);

  static scoped_ptr<CudaLoadModule> instance_;
  static boost::once_flag flag_;
};

template<typename DType>
void BlitzSassGemm(const bool transa, const bool transb, const int M, const int N,
    const int K, DType* A, DType* B, DType* C, DType alpha, DType beta);
}  // namespace blitz

#endif  // SRC_KERNERLS_SASS_FUNCTION_H_
