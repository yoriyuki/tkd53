#pragma once

#include <memory>

#include "converter/converter_interface.hpp"

namespace lime {
namespace engine {
using namespace std;
using namespace lime::base::kkci;
using namespace lime::base::token;
using namespace lime::converter;


class Engine {
public:
  Engine(shared_ptr<ConverterInterface> converter);

  void Convert(const KkciString &input, TokenString *output);

private:
  shared_ptr<ConverterInterface> converter_;
};


} // engine
} // lime
