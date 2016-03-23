#include <sstream>

#include <picojson/picojson.h>

#include "dictionary/dictionary_interface.hpp"
#include "request.hpp"

namespace lime {
namespace server {

namespace {

void ReadKkciString(picojson::array &kkci_seq, KkciString *str) {
  for (picojson::array::iterator it = kkci_seq.begin();
       it != kkci_seq.end(); ++it) {
    str->push_back(static_cast<Kkci>(it->get<double>()));
  }
}

void DoConvert(const KkciString &str,
               Engine *engine,
               string *response_string) {
  Segments segments;
  engine->Convert(str, &segments);

  stringstream ss;
  ss << "{\"result\":true,\"kkciSequence\":[";
  for (size_t i = 0; i < segments.size(); i++) {
    if (0 < i) {
      ss << ",";
    }
    ss << "{";
    ss << "\"token\":" << segments[i]->token;
    ss << ",\"origin\":" << segments[i]->entry->origin;
    ss << ",\"kkciSequence\":[";
    for (size_t j = 0; j < segments[i]->entry->kkci_string.size(); j++) {
      if (0 < j) {
        ss << ",";
      }
      ss << segments[i]->entry->kkci_string[j];
    }
    ss << "]}";
  }
  ss << "]}" << endl;
  *response_string = ss.str();
}

void DoListCandidates(const KkciString &str,
                      Engine *engine,
                      string *response_string) {
  
}


} // namespace

bool HandleRequest(const string &request_string,
                   Engine *engine,
                   string *response_string) {
  string err;
  picojson::value v;
  picojson::parse(v, request_string);

  if (!err.empty()) {
    *response_string = "{\"result\":false, \"message\":\"invalid json\"}";
      return true;
  }

  picojson::object &obj = v.get<picojson::object>();
  const string &command = obj["command"].get<string>();
  if (command == "CONVERT") {
    KkciString str;
    ReadKkciString(obj["kkciSequence"].get<picojson::array>(), &str);
    DoConvert(str, engine, response_string);
    return true;
  }

  if (command == "LIST_CANDIDATES") {
    KkciString str;
    ReadKkciString(obj["kkciSequence"].get<picojson::array>(), &str);
    DoListCandidates(str, engine, response_string);
    return true;
  }

  return false;
}


} // server
} // lime
