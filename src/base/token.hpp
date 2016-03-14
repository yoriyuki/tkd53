#pragma once

#include <string>

namespace lime {
namespace base {
namespace token {

typedef int Token;

const Token kBOS = 0;

const Token kUNK = 1;

typedef std::basic_string<Token> TokenString;

} // token
} // base
} // lime
