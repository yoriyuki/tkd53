#pragma once

#include <vector>

#include "../base/kkci.hpp"
#include "../base/token.hpp"

namespace lime {
namespace dictionary {
using namespace std;
using namespace lime::base::kkci;
using namespace lime::base::token;

class DictionaryInterface {
public:
  virtual const vector<Token> &Lookup(const KkciString &input) = 0;

protected:
  DictionaryInterface() {}
  virtual ~DictionaryInterface() {}
};

} // dictionary
} // lime
