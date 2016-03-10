#include <iostream>
#include <gtest/gtest.h>
#include "dictionary/map_dictionary.hpp"
#include "bigram_converter.hpp"

using namespace std;
using namespace lime::base::kkci;
using namespace lime::base::token;
using namespace lime::converter;
using namespace lime::dictionary;


BigramConverter *CreateBigramConverter() {
  shared_ptr<Bigram> bigram(new Bigram());
  bigram->Init(ifstream("../../../var/WordMarkov.freq", ios::in));

  shared_ptr<MapDictionary> dict(new MapDictionary());
  dict->Init(ifstream("../../../var/WordKkci.text", ios::in));

  return new BigramConverter(bigram, dict);
}


class BigramConverterTest : public ::testing::Test {
protected:
  void SetUp() override {
    // し ょ う ぎ の ほ ん
    inputs_.push_back(KkciString({58, 106, 41, 49, 81, 94, 118}));
    // 将棋 の 本
    expected_outputs_.push_back(TokenString({14686, 2461, 21867}));

    // こ う じ ち ゅ う
    inputs_.push_back(KkciString({54, 41, 59, 68, 104, 41}));
    // 工事 中
    expected_outputs_.push_back(TokenString({11753, 18060}));

    converter_ = CreateBigramConverter();
  }

  vector<KkciString> inputs_;

  vector<TokenString> expected_outputs_;

  BigramConverter *converter_;
};

TEST_F(BigramConverterTest, Convert) {
  for (size_t i = 0; i < inputs_.size(); i++) {
    Segments actual;
    converter_->Convert(inputs_[i], &actual);
    const TokenString &expected = expected_outputs_[i];

    ASSERT_EQ(actual.size(), expected.size());
    for (size_t j = 0; j < actual.size(); j++) {
      ASSERT_EQ(actual[j].token, expected[j]);
    }
  }
}
