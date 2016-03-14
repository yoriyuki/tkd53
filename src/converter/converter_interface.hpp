#pragma once

#include "base/kkci.hpp"
#include "base/token.hpp"

namespace lime {
namespace converter {
using namespace std;
using namespace lime::base::kkci;
using namespace lime::base::token;

class ConverterInterface {
public:
  virtual void Convert(const KkciString &input, TokenString *output) = 0;

protected:
  ConverterInterface() {}
  virtual ~ConverterInterface() {}
};

} // converter
} // lime
