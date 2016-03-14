#pragma once

#include <memory>

#include "dictionary/dictionary_interface.hpp"
#include "converter_interface.hpp"
#include "bigram.hpp"

namespace lime {
namespace converter {
using namespace std;
using namespace lime::base::kkci;
using namespace lime::base::token;
using namespace lime::dictionary;


struct Node {
  const Token token;
  const Cost cost_so_far;
  const Node* const prev;
};


class BigramConverter : public ConverterInterface {
public:
  BigramConverter(shared_ptr<Bigram> bigram,
                  shared_ptr<DictionaryInterface> dict);

  void Convert(const KkciString &input, TokenString *output) override;

private:
  void InsertNode(const Token token,
                  const vector<Node> &prev_columns,
                  vector<Node> *columns);

  shared_ptr<Bigram> bigram_;
  shared_ptr<DictionaryInterface> dict_;
};

} // converter
} // lime
