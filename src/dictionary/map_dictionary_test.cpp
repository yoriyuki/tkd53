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

  vector<const Entry*> entries;

  dict.PushBack(94);
  dict.PushBack(45);
  dict.Lookup(&entries);

  ASSERT_EQ(entries.size(), 14);
  ASSERT_EQ( entries[0]->token,    11);
  ASSERT_EQ( entries[1]->token,  2748);
  ASSERT_EQ( entries[2]->token, 21802);
  ASSERT_EQ( entries[3]->token,  1237);
  ASSERT_EQ( entries[4]->token,  3669);
  ASSERT_EQ( entries[5]->token,  8223);
  ASSERT_EQ( entries[6]->token, 11528);
  ASSERT_EQ( entries[7]->token, 15167);
  ASSERT_EQ( entries[8]->token, 15727);
  ASSERT_EQ( entries[9]->token, 16386);
  ASSERT_EQ(entries[10]->token, 17967);
  ASSERT_EQ(entries[11]->token, 18441);
  ASSERT_EQ(entries[12]->token, 20588);
  ASSERT_EQ(entries[13]->token, 20958);

  dict.Clear();
  entries.clear();

  dict.PushBack(94);
  dict.PushBack(45);
  dict.Lookup(&entries);

  ASSERT_EQ( entries[0]->token,    11);
  ASSERT_EQ( entries[1]->token,  2748);
  ASSERT_EQ( entries[2]->token, 21802);
  ASSERT_EQ( entries[3]->token,  1237);
  ASSERT_EQ( entries[4]->token,  3669);
  ASSERT_EQ( entries[5]->token,  8223);
  ASSERT_EQ( entries[6]->token, 11528);
  ASSERT_EQ( entries[7]->token, 15167);
  ASSERT_EQ( entries[8]->token, 15727);
  ASSERT_EQ( entries[9]->token, 16386);
  ASSERT_EQ(entries[10]->token, 17967);
  ASSERT_EQ(entries[11]->token, 18441);
  ASSERT_EQ(entries[12]->token, 20588);
  ASSERT_EQ(entries[13]->token, 20958);
}
