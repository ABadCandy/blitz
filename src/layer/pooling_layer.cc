#include "layer/pooling_layer.h"

#include "backend/backends.h"

namespace blitz {

template<template <typename> class TensorType, typename DType>
void PoolingLayer<TensorType, DType>::InitImpl(const Shape& input_shape) {
  // input shape decode
  int batch_size = input_shape[0];
  int input_channel = input_shape[1];
  int input_height = input_shape[2];
  int input_width = input_shape[3];
  // output shape encode
  int output_channel = input_channel;
  int output_height = (input_height - filter_) / stride_ + 1;
  int output_width = (input_width - filter_) / stride_ + 1;

  Shape output_shape(4);
  output_shape[0] = batch_size;
  output_shape[1] = output_channel;
  output_shape[2] = output_height;
  output_shape[3] = output_width;

  // forward and backward output
  this->forward_output_ = make_shared<TensorType<DType> >(output_shape);
  this->backward_output_ = make_shared<TensorType<DType> >(input_shape);

  if (op_ == "max") {
    this->max_index_ = make_shared<TensorType<int> >(output_shape);
  } else {
    LOG(ERROR) << "Pooling type: " << op_ << " not exist";
  }

  LOG(INFO) << "Pooling Layer: " << this->name_;
  LOG(INFO) << "input shape: " << input_channel << " * " <<
    input_height << " * " << input_width;
  LOG(INFO) << "output shape: " << output_channel << " * " <<
    output_height << " * "<< output_width;
}

template<template <typename> class TensorType, typename DType>
void PoolingLayer<TensorType, DType>::ForwardPropImpl(
  shared_ptr<TensorType<DType> > forward_input) {
  if (op_ == "max") {
    Backend<TensorType, DType>::MaxPooling2DForwardFunc(
      forward_input.get(), filter_, filter_, stride_, stride_,
      max_index_.get(), (this->forward_output_).get());
  } else {
    LOG(ERROR) << "Pooling type: " << op_ << " not exist";
  }
}

template<template <typename> class TensorType, typename DType>
void PoolingLayer<TensorType, DType>::BackwardPropImpl(
  shared_ptr<TensorType<DType> > backward_input) {
  if (this->backward_prop_) {
    if (op_ == "max") {
      Backend<TensorType, DType>::MaxPooling2DBackwardFunc(
        backward_input.get(), max_index_.get(),
        filter_, filter_, stride_, stride_,
        (this->backward_output_).get());
    } else {
      LOG(ERROR) << "Pooling type: " << op_ << " not exist";
    }
  }
}

INSTANTIATE_CLASS(PoolingLayer);

}  // namespace blitz
