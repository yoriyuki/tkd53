#pragma once

#include <deque>

#include "base/kkci.hpp"
#include "base/token.hpp"
#include "dictionary/dictionary_interface.hpp"


namespace lime {
namespace converter {
using namespace std;
using namespace lime::base::kkci;
using namespace lime::base::token;
using namespace lime::dictionary;


typedef double Cost;


struct Node {
  const Token token;
  const Node* const prev;
  const Cost cost_so_far;
  const Entry* const entry;
};


class Segments : public deque<Node> {};


class ConverterInterface {
public:
  virtual void Convert(KkciString &input, Segments *output) = 0;

protected:
  ConverterInterface() {}
  virtual ~ConverterInterface() {}
};

} // converter
} // lime
