#pragma once

#include <deque>

#include "base/kkci.hpp"
#include "base/token.hpp"
#include "lattice.hpp"


namespace lime {
namespace converter {
using namespace std;
using namespace lime::base::kkci;
using namespace lime::base::token;
using namespace lime::dictionary;


typedef deque<shared_ptr<const Node> > Segments;


class AbstractConverter {
public:
  virtual void Convert(const KkciString &string,
                       Token begin_token,
                       Token end_token,
                       Segments *result) = 0;

protected:
  AbstractConverter() {}
  virtual ~AbstractConverter() {}
};

} // converter
} // lime
