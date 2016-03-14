#include <iostream>
#include <cmath>
#include <sstream>
#include <stdexcept>

#include "bigram.hpp"


namespace lime {
namespace converter {

Bigram::Bigram()
  : token_count_(0), ngram_counts_(map<TokenString, int>()) {
}


void Bigram::Init(istream &&is) {
  string entry;
  while (getline(is, entry)) {
    int count;
    istringstream iss(entry);
    iss >> count;

    if (count < 0) {
      throw runtime_error(entry);
    }

    TokenString token_string;
    Token tok;
    while (iss >> tok) {
      token_string.push_back(tok);
    }

    if (token_string.empty()) {
      token_count_ = count;
    } else {
      ngram_counts_.insert(pair<TokenString, int>(token_string, count));
    }
  }
}


Cost Bigram::GetCost(const Token prev_token, const Token curr_token) const {
  map<TokenString, int>::const_iterator joint_iter
    = ngram_counts_.find(TokenString({prev_token, curr_token}));

  map<TokenString, int>::const_iterator prev_iter
    = ngram_counts_.find(TokenString({prev_token}));

  map<TokenString, int>::const_iterator curr_iter
    = ngram_counts_.find(TokenString({curr_token}));

  if (prev_iter == ngram_counts_.end() || curr_iter == ngram_counts_.end()) {
    throw runtime_error("GetCost");
  }

  const double prob_unigram =
    static_cast<double>(curr_iter->second) / token_count_;

  const double prob_bigram = joint_iter != ngram_counts_.end() ?
    static_cast<double>(joint_iter->second) / prev_iter->second : 0;

  return Cost(-log(0.2 * prob_unigram + 0.8 * prob_bigram));
}


} // converter
} // lime
