#include <sstream>

#include "request.hpp"

namespace lime {
namespace server {

bool HandleRequest(const string &request_string,
                   Engine *engine,
                   string *response_string) {
  size_t pos = 0;
  while (request_string[pos] != ' ') {
    if (request_string.size() <= pos) {
      return false;
    }
    pos++;
  }

  if (pos == 7 && request_string.compare(pos, 7, "CONVERT")) {
    KkciString input;
    while (++pos < request_string.size()) {
      int from = pos;
      while (request_string[pos] != ' ') {
        pos++;
      }
      input.push_back(Kkci(stoi(request_string.substr(from, pos - from))));
    }

    TokenString output;
    engine->Convert(input, &output);

    stringstream ss;
    for (size_t i = 0; i < output.size(); i++) {
      ss << output[i] << " ";
    }
    ss << endl;

    *response_string = ss.str();

    return true;
  }

  return false;
}


} // server
} // lime
