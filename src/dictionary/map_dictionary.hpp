#pragma once

#include <map>
#include <vector>
#include <fstream>

#include "dictionary_interface.hpp"


namespace lime {
namespace dictionary {
using namespace std;
using namespace lime::base::token;
using namespace lime::base::kkci;

class MapDictionary : public DictionaryInterface {
public:
  MapDictionary();

  const vector<Token> &Lookup(const KkciString &input) override;

  void Init(ifstream &&is);

private:
  map<KkciString, vector<Token> > mapping_;

  const vector<Token> empty_vector_;
};

} // dictionary
} // lime
