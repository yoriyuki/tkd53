#include <gtest/gtest.h>
#include "map_dictionary.hpp"

using namespace std;
using namespace lime::base::kkci;
using namespace lime::base::token;
using namespace lime::dictionary;

TEST(MapDictionary, InitAndLookup) {
  // Set Up
  MapDictionary dict;
  dict.Init(ifstream("../../../var/WordKkci.text", ios::in));

  // Exercise
  vector<Token> tokens = dict.Lookup(KkciString({94, 45}));

  // Verify
  ASSERT_EQ(tokens.size(), 3);
  ASSERT_EQ(tokens[0],    11);
  ASSERT_EQ(tokens[1],  2748);
  ASSERT_EQ(tokens[2], 21802);
}
