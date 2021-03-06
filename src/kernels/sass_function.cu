#include "kernels/sass_function.h"

#include <cuda.h>

namespace blitz {

scoped_ptr<CudaLoadModule> CudaModule::instance_(0);
boost::once_flag CudaModule::flag_ = BOOST_ONCE_INIT;

template<>
void BlitzSassGemm(const bool transa, const bool transb, const int M, const int N,
  const int K, float* A, float* B, float* C, float alpha, float beta) {
  CUfunction function;
  int lda, ldb, ldc = N;

#ifdef BLITZ_PERFORMANCE  // only valid for a single thread
  time_point<system_clock> start, end;
  duration<double> time = duration<double>::zero();
  start = system_clock::now();
#endif

  // create kernel
  string kernel;
  if (transa == true && transb == false) {
    lda = M * 32;
    ldb = N * 32;
    kernel = "sgemm_tn_128x128";
  } else if (transa == false && transb == true) {
    lda = K;
    ldb = K;
    kernel = "sgemm_nt_128x128";
  } else if (transa == false && transb == false) {
    lda = K;
    ldb = N * 32;
    kernel = "sgemm_nn_128x128";
  } else {
    LOG(FATAL) << "Not support both matrice transport!";
  }

  function = CudaModule::GetFunction(kernel);

#ifdef BLITZ_PERFORMANCE
    end = system_clock::now();
    time += end - start;
    LOG(INFO) << "Load kernel time: " << time.count();
#endif  // BLITZ_PERFORMANCE

  void* params[] = {&A, &B, &C, &alpha, &beta, &lda, &ldb, &ldc, 
    (void*)&M, (void*)&N, (void*)&K};

  // TODO(keren): multiple kernels
  int sizeA = 128, sizeB = 128;
  int gridA = M / sizeA + (N % sizeA !=0);
  int gridB = N / sizeB + (N % sizeB !=0);

  // TODO(keren): adjust number of threads
  int threads = 256;

  // kernel call, asynrhonize
  cuLaunchKernel(function, 1, gridA, gridB, threads, 1, 1, 0, 0, params, 0);
}

template<>
void BlitzSassGemm(const bool transa, const bool transb, const int M, const int N,
    const int K, double* A, double* B, double* C, double alpha, double beta) {
  LOG(FATAL) << "sass kernel dost not support double precision";
}

}  // namespace blitz

