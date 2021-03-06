#ifndef SRC_TRANSFORM_SOFTMAX_H_
#define SRC_TRANSFORM_SOFTMAX_H_

#include "transform/activation.h"

namespace blitz {

template<template <typename> class TensorType, typename DType>
class Softmax : public Activation<TensorType, DType> {
 public:
  explicit Softmax(const bool short_cut = true) : short_cut_(short_cut) {}
  // indicate pure virtual
  ~Softmax() {}

  virtual void Apply(const shared_ptr<TensorType<DType> > input,
    shared_ptr<TensorType<DType> > output);

  virtual void Derivative(const shared_ptr<TensorType<DType> > input,
    shared_ptr<TensorType<DType> > output);

 private:
  const bool short_cut_;
};

}  // namespace blitz

#endif  // SRC_TRANSFORM_SOFTMAX_H_
