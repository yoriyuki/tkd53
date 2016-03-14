#pragma once

#include <map>
#include <istream>

#include "base/token.hpp"


namespace lime {
namespace converter {
using namespace std;
using namespace lime::base::token;

typedef double Cost;

class Bigram {
public:
  Bigram();

  Cost GetCost(const Token prev_token, const Token curr_token) const;

  void Init(istream &&is);
private:

  int token_count_;

  map<TokenString, int> ngram_counts_;
};

} // converter
} // lime
