#pragma once

#include <memory>

#include "dictionary/dictionary_interface.hpp"
#include "abstract_converter.hpp"
#include "bigram.hpp"
#include "lattice_builder.hpp"

namespace lime {
namespace converter {
using namespace std;
using namespace lime::base::kkci;
using namespace lime::base::token;
using namespace lime::dictionary;


class BigramConverter : public AbstractConverter {
public:
  BigramConverter(shared_ptr<Bigram> bigram,
                  shared_ptr<LatticeBuilder> lattice_builder);

  virtual void Convert(const KkciString &string,
                       Token begin_token,
                       Token end_token,
                       Segments *result) override;

private:
  void SetBestLeftNode(vector<shared_ptr<Node> > &lnodes,
                       shared_ptr<Node> rnode);

  shared_ptr<Bigram> bigram_;

  shared_ptr<LatticeBuilder> lattice_builder_;
};

} // converter
} // lime
