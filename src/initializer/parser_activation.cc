#include "initializer/parser.h"

#include "backend/backends.h"
#include "transform/logistic.h"
#include "transform/rectlin.h"
#include "transform/softmax.h"

namespace blitz {

template<template <typename> class TensorType, typename DType>
shared_ptr<Activation<TensorType, DType> >
  Parser::SetActivation(const YAML::Node& node) const {
  shared_ptr<Activation<TensorType, DType> > activation;
  string type = node["type"].as<string>();

  if (type == "Rectlin") {
    activation = static_pointer_cast<Activation<TensorType, DType> >(
        make_shared<Rectlin<TensorType, DType> >());
  } else if (type == "Logistic") {
    bool short_cut;
    if (node["short_cut"]) {
      short_cut = node["short_cut"].as<bool>();
    } else {
      short_cut = false;
      LOG(WARNING) << "'short_cut' parameter missing";
    }
    activation = static_pointer_cast<Activation<TensorType, DType> >(
        make_shared<Logistic<TensorType, DType> >(short_cut));
  } else if (type == "Softmax") {
    bool short_cut;
    if (node["short_cut"]) {
      short_cut = node["short_cut"].as<bool>();
    } else {
      short_cut = false;
      LOG(WARNING) << "'short_cut' parameter missing";
    }
    activation = static_pointer_cast<Activation<TensorType, DType> >(
        make_shared<Softmax<TensorType, DType> >(short_cut));
  } else {
    LOG(FATAL) << "Unkown activation type: " << type;
  }

  return activation;
}

INSTANTIATE_SETTER(Activation);

}  // namespace blitz
