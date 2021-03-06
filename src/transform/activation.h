#ifndef SRC_TRANSFORM_ACTIVATION_H_
#define SRC_TRANSFORM_ACTIVATION_H_

#include "util/common.h"
#include "backend/tensor.h"

namespace blitz {

template<template <typename> class TensorType, typename DType>
class Activation {
 public:
  Activation() {}  // indicate pure virtual
  virtual ~Activation() {}  // ensure pure virtual

  virtual void Apply(const shared_ptr<TensorType<DType> > input,
    shared_ptr<TensorType<DType> > output) = 0;
  virtual void Derivative(const shared_ptr<TensorType<DType> > input,
    shared_ptr<TensorType<DType> > output) = 0;
};


}  // namespace blitz

#endif  // SRC_TRANSFORM_ACTIVATION_H_
