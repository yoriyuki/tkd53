#include <gtest/gtest.h>
#include <fstream>
#include "bigram.hpp"

using namespace std;
using namespace lime::base::token;
using namespace lime::converter;


TEST(Bigram, Init) {
  Bigram bigram;
  bigram.Init(ifstream("../../../etc/WordMarkov.freq", ios::in));
}
