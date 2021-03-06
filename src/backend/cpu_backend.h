#ifndef SRC_BACKEND_CPU_BACKEND_H_
#define SRC_BACKEND_CPU_BACKEND_H_

#include "backend/backend.h"

#include <omp.h>
#include <cmath>
#include <string>
#include <vector>
#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/uniform_real.hpp>

#include "util/blitz_cpu_function.h"
#include "util/blitz_cpu_avx.h"
#include "backend/cpu_tensor.h"

namespace blitz {

// default general CPU
template<typename DType>
class Backend<CPUTensor, DType> {
 public:
  static void RectlinApplyFunc(
    const CPUTensor<DType>* input,
    const DType slope,
    CPUTensor<DType>* output);

  static void RectlinDerivativeFunc(
    const CPUTensor<DType>* input,
    const DType slope,
    CPUTensor<DType>* output);

  static void LogisticApplyFunc(
    const CPUTensor<DType>* input,
    CPUTensor<DType>* output);

  static void LogisticDerivativeFunc(
    const CPUTensor<DType>* input,
    CPUTensor<DType>* output);

  static void SoftmaxApplyFunc(
    const CPUTensor<DType>* input,
    CPUTensor<DType>* output);

  static void SoftmaxDerivativeFunc(
    const CPUTensor<DType>* input,
    CPUTensor<DType>* output);

  static DType SquareMeanApplyFunc(
    const CPUTensor<DType>* input,
    const CPUTensor<DType>* target);

  static void SquareMeanDerivativeFunc(
    const CPUTensor<DType>* input, const CPUTensor<DType>* target,
    CPUTensor<DType>* output);

  static DType AbsMeanApplyFunc(
    const CPUTensor<DType>* input,
    const CPUTensor<DType>* target);

  static void AbsMeanDerivativeFunc(
    const CPUTensor<DType>* input, const CPUTensor<DType>* target,
    CPUTensor<DType>* output);

  static DType CrossEntropyBinaryApplyFunc(
    const CPUTensor<DType>* input,
    const CPUTensor<DType>* target);

  static void CrossEntropyBinaryDerivativeFunc(
    const CPUTensor<DType>* input, const CPUTensor<DType>* target,
    CPUTensor<DType>* output);

  static DType CrossEntropyMultiApplyFunc(
    const CPUTensor<DType>* input, const CPUTensor<DType>* target);

  static void CrossEntropyMultiDerivativeFunc(
    const CPUTensor<DType>* input, const CPUTensor<DType>* target,
    CPUTensor<DType>* output);

  static void BiasForwardFunc(
    const CPUTensor<DType>* input, const CPUTensor<DType>* bias,
    CPUTensor<DType>* output);

  static void BiasBackwardUpdateFunc(const CPUTensor<DType>* input,
    CPUTensor<DType>* update);

  static void BatchNormForwardFunc(
    const CPUTensor<DType>* input, const CPUTensor<DType>* gamma,
    const CPUTensor<DType>* beta, const DType epsilon,
    CPUTensor<DType>* input_var, CPUTensor<DType>* input_hat,
    CPUTensor<DType>* output);

  static void BatchNormBackwardFunc(
    const CPUTensor<DType>* backward_input,
    const CPUTensor<DType>* forward_input_hat,
    const CPUTensor<DType>* forward_input_var,
    const CPUTensor<DType>* gamma, const DType epsilon,
    CPUTensor<DType>* gamma_update, CPUTensor<DType>* beta_update,
    CPUTensor<DType>* output);

  static void GradientdescentFunc(
    const DType momentum_coef, const DType learning_rate,
    const DType decay, const int batch_size,
    CPUTensor<DType>* weight,
    CPUTensor<DType>* gradient,
    CPUTensor<DType>* velocity);

  static void MatrixDotFunc(
    const CPUTensor<DType>* left, const CPUTensor<DType>* right,
    const bool transa, const bool transb,
    const DType alpha, const DType beta,
    CPUTensor<DType>* output, const string& kernel = "blas");

  static void MaximumFunc(
    const CPUTensor<DType>* left, const CPUTensor<DType>* right,
    CPUTensor<DType>* output);

  static void MaximumFunc(
    const CPUTensor<DType>* left,
    const DType right,
    CPUTensor<DType>* output);

  static void MinusFunc(
    const CPUTensor<DType>* left, const CPUTensor<DType>* right,
    CPUTensor<DType>* output);

  static void MinusFunc(
    const CPUTensor<DType>* left,
    const DType right,
    CPUTensor<DType>* output);

  static DType SumFunc(const CPUTensor<DType>* input);

  static void AddFunc(
    const CPUTensor<DType>* left, const CPUTensor<DType>* right,
    CPUTensor<DType>* output);

  static void MultiplyFunc(
    const CPUTensor<DType>* left, const CPUTensor<DType>* right,
    CPUTensor<DType>* output);

  static void MultiplyFunc(
    const CPUTensor<DType>* left, const DType right,
    CPUTensor<DType>* output);

  static void Convolution2DForwardFunc(
    const CPUTensor<DType>* input, const CPUTensor<DType>* weight,
    const int padding_height, const int padding_width,
    const int stride_height, const int stride_width,
    CPUTensor<DType>* unpack, CPUTensor<DType>* output,
    const string& kernel = "blas");

  static void Convolution2DBackwardFunc(
    const CPUTensor<DType>* output, const CPUTensor<DType>* weight,
    const int padding_height, const int padding_width,
    const int stride_height, const int stride_width,
    CPUTensor<DType>* pack, CPUTensor<DType>* input,
    const string& kernel = "blas");

  static void Convolution2DUpdateFunc(
    const CPUTensor<DType>* input, const CPUTensor<DType>* output,
    const int padding_height, const int padding_width,
    const int stride_height, const int stride_width,
    CPUTensor<DType>* unpack, CPUTensor<DType>* update,
    const string& kernel = "blas");

  // batch parallel
  static void Convolution2DForwardFunc(
    const CPUTensor<DType>* input, const CPUTensor<DType>* weight,
    const int padding_height, const int padding_width,
    const int stride_height, const int stride_width,
    vector<shared_ptr<CPUTensor<DType> > >* unpack_batch,
    CPUTensor<DType>* output);

  static void Convolution2DBackwardFunc(
    const CPUTensor<DType>* output, const CPUTensor<DType>* weight,
    const int padding_height, const int padding_width,
    const int stride_height, const int stride_width,
    vector<shared_ptr<CPUTensor<DType> > >* pack_batch,
    CPUTensor<DType>* input);

  static void Convolution2DUpdateFunc(
    const CPUTensor<DType>* input, const CPUTensor<DType>* output,
    const int padding_height, const int padding_width,
    const int stride_height, const int stride_width,
    vector<shared_ptr<CPUTensor<DType> > >* unpack_batch,
    vector<shared_ptr<CPUTensor<DType> > >* update_batch,
    CPUTensor<DType>* update);

  static void MaxPooling2DForwardFunc(
    const CPUTensor<DType>* input,
    const int filter_height, const int filter_width,
    const int stride_width, const int stride_height,
    CPUTensor<int>* max_index, CPUTensor<DType>* output);

  static void MaxPooling2DBackwardFunc(
    const CPUTensor<DType>* output, const CPUTensor<int>* max_index,
    const int filter_height, const int filter_width,
    const int stride_height, const int stride_width,
    CPUTensor<DType>* input);

  // naive parallel
  static void Convolution2DForwardFunc(
    const CPUTensor<DType>* input, const CPUTensor<DType>* weight,
    const int stride_height, const int stride_width,
    CPUTensor<DType>* output);

  static void MakeBinaryMaskFunc(const DType low, const DType high,
    const DType keep, CPUTensor<DType>* output);

  static void ConstantDistributionFunc(
    const DType val, CPUTensor<DType>* output);

  static void NormalDistributionFunc(
    const DType loc, const DType scale,
    CPUTensor<DType>* output);

  static void UniformDistributionFunc(
    const DType low, const DType high,
    CPUTensor<DType>* output);

  static void HostCopyToFunc(const DType* source, const size_t size,
    DType* target);

  static float EvaluateClassifyFunc(
    const CPUTensor<DType>* output, const CPUTensor<DType>* target);

  static float EvaluateRegressFunc(
    const CPUTensor<DType>* output, const CPUTensor<DType>* target);

  static void Unpack2DParallelFunc(
    const DType* input, const int channel,
    const int input_height, const int input_width,
    const int filter_height, const int filter_width,
    const int output_height, const int output_width,
    const int padding_height, const int padding_width,
    const int stride_height, const int stride_width,
    DType* unpack);

  static void Pack2DParallelFunc(
    const DType* pack, const int channel,
    const int input_height, const int input_width,
    const int filter_height, const int filter_width,
    const int output_height, const int output_width,
    const int padding_height, const int padding_width,
    const int stride_height, const int stride_width,
    DType* input);

  static void Unpack2DFunc(
    const DType* input, const int channel,
    const int input_height, const int input_width,
    const int filter_height, const int filter_width,
    const int output_height, const int output_width,
    const int padding_height, const int padding_width,
    const int stride_height, const int stride_width,
    DType* unpack);

  static void Pack2DFunc(
    const DType* pack, const int channel,
    const int input_height, const int input_width,
    const int filter_height, const int filter_width,
    const int output_height, const int output_width,
    const int padding_height, const int padding_width,
    const int stride_height, const int stride_width,
    DType* input);
};

}  // namespace blitz

#endif  // SRC_BACKEND_CPU_BACKEND_H_
