#include <sstream>
#include <iostream>

#include "map_dictionary.hpp"

namespace lime {
namespace dictionary {

MapDictionary::MapDictionary()
  : mapping_(map<KkciString, vector<Entry> >()),
    key_(KkciString()) {
}


void MapDictionary::Clear() {
  key_.clear();
}


void MapDictionary::PushBack(const Kkci kkci) {
  key_.push_back(kkci);
}


void MapDictionary::Lookup(vector<const Entry*> *entries) {
  if (key_.empty()) {
    return;
  }

  for (size_t i = 0; i < key_.size(); i++) {
    map<KkciString, vector<Entry> >::const_iterator map_iter =
      mapping_.find(key_.substr(i, key_.size() - i));
    if (map_iter == mapping_.end()) {
      continue;
    }
    for (vector<Entry>::const_iterator iter = map_iter->second.begin();
         iter != map_iter->second.end(); ++iter) {
      const Entry &entry = *iter;
      entries->push_back(&entry);
    }
  }
}


void MapDictionary::Init(ifstream &&is) {
  string entry;
  while (getline(is, entry)) {
    istringstream iss(entry);
    Token tok;
    iss >> tok;

    KkciString kkci_string;
    string str;
    while (iss >> str) {
      kkci_string.push_back(Kkci(stoi(str)));
    }
    mapping_[kkci_string].push_back(Entry({ kkci_string, tok, IN_DICT }));
  }
}

} // dictionary
} // lime
