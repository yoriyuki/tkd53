#pragma once

#include <memory>

#include "converter/abstract_converter.hpp"

namespace lime {
namespace engine {
using namespace std;
using namespace lime::base::kkci;
using namespace lime::base::token;
using namespace lime::converter;


class Engine {
public:
  Engine(shared_ptr<AbstractConverter> converter);

  void Convert(const KkciString &input, Segments *output);

private:
  shared_ptr<AbstractConverter> converter_;
};


} // engine
} // lime
