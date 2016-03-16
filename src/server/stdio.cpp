#include <iostream>

#include "engine/engine.hpp"
#include "converter/bigram_converter.hpp"
#include "dictionary/map_dictionary.hpp"

#include "request.hpp"

using namespace std;
using namespace lime::base::kkci;
using namespace lime::base::token;
using namespace lime::converter;
using namespace lime::dictionary;
using namespace lime::server;
using namespace lime::engine;

const string kNgramFreqFile = "../var/WordMarkov.freq";

const string kWordKkciMappingFile = "../var/WordKkci.text";

int main(void) {
  shared_ptr<Bigram> bigram(new Bigram());
  bigram->Init(ifstream(kNgramFreqFile, ios::in));

  shared_ptr<MapDictionary> dict(new MapDictionary());
  dict->Init(ifstream(kWordKkciMappingFile, ios::in));

  Engine engine(shared_ptr<AbstractConverter>(
      new BigramConverter(bigram,
          shared_ptr<LatticeBuilder>(new LatticeBuilder(dict)))));

  string request_string;
  string response_string;
  while (getline(cin, request_string)) {
    cerr << "<" << request_string << ">" << endl;
    if (HandleRequest(request_string, &engine, &response_string)) {
      cout << response_string;
    }
  }
}
