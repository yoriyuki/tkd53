#pragma once

#include <map>
#include <vector>
#include <fstream>

#include "dictionary_interface.hpp"


namespace lime {
namespace dictionary {

class MapDictionary : public DictionaryInterface {
public:
  MapDictionary();

  virtual void Clear() override;

  virtual void PushBack(Kkci kkci) override;

  virtual void Lookup(vector<const Entry*> *entries) override;

  void Init(ifstream &&is);

private:
  map<KkciString, vector<Entry> > mapping_;
  KkciString key_;
};

} // dictionary
} // lime
