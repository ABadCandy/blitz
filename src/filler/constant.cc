#include "filler/constant.h"
#include "backend/backends.h"

namespace blitz {

template<template <typename> class TensorType, typename DType>
void Constant<TensorType, DType>::FillImpl(LayerWeightIterator
  layer_weight_it) {
#ifdef BLITZ_DEVELOP
  LOG(INFO) << "Constant Fill: " << layer_weight_it->first;
#endif
  shared_ptr<TensorType<DType> > weight = layer_weight_it->second;
  weight->Fill(val_);
}

INSTANTIATE_CLASS(Constant);

}  // namespace blitz
