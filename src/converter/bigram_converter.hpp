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


class BigramConverter : public ConverterInterface {
public:
  BigramConverter(shared_ptr<Bigram> bigram,
                  shared_ptr<DictionaryInterface> dict);

  virtual void Convert(KkciString &input, Segments *output) override;

private:
  void FindBestNode(const Token token,
                    const vector<Node> &left_nodes,
                    const Node** node,
                    Cost* cost);

  shared_ptr<Bigram> bigram_;
  shared_ptr<DictionaryInterface> dict_;
};

} // converter
} // lime
