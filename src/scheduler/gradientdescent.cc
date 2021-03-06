#include "scheduler/gradientdescent.h"
#include "backend/backends.h"

namespace blitz {

template<template <typename> class TensorType, typename DType>
void Gradientdescent<TensorType, DType>::OptimizeImpl(
  const int epoch, const int batch_size,
  const DType learning_rate, LayerParamIterator layer_param_it) {
#ifdef BLITZ_DEVELOP
    LOG(INFO) << "Optimize: " << layer_param_it->first;
    LOG(INFO) << "batch_size: " << batch_size;
    LOG(INFO) << "epoch: " << epoch;
    LOG(INFO) << "learning_rate: " << learning_rate;
#endif
  shared_ptr<LayerParam> layer_param = layer_param_it->second;
  shared_ptr<TensorType<DType> > weight = layer_param->weight();
  shared_ptr<TensorType<DType> > gradient = layer_param->update();
  shared_ptr<TensorType<DType> > velocity = layer_param->state();

  Backend<TensorType, DType>::GradientdescentFunc(
    momentum_coef_, learning_rate, decay_, batch_size,
    weight.get(), gradient.get(), velocity.get());
}

INSTANTIATE_CLASS(Gradientdescent);

}  // namespace blitz

