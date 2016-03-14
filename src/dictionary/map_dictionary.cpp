#include <sstream>
#include <iostream>

#include "map_dictionary.hpp"

namespace lime {
namespace dictionary {

MapDictionary::MapDictionary()
  : mapping_(map<KkciString, vector<Token> >()),
    empty_vector_(vector<Token>()) {
}


const vector<Token> &MapDictionary::Lookup(const KkciString &input) {
  map<KkciString, vector<Token> >::const_iterator iter = mapping_.find(input);
  if (iter != mapping_.end()) {
    return iter->second;
  } else {
    return empty_vector_;
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
    mapping_[kkci_string].push_back(tok);
  }
}

} // dictionary
} // lime
