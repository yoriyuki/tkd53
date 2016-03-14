#include "engine/engine.hpp"

namespace lime {
namespace server {
using namespace std;
using namespace lime::engine;

bool HandleRequest(
    const string &request_string, Engine *engine, string *response_string);


} // server
} // lime
