#include <map>
#include <stdexcept>

#include "bigram_converter.hpp"


namespace lime {
namespace converter {

BigramConverter::BigramConverter(shared_ptr<Bigram> bigram,
                                 shared_ptr<DictionaryInterface> dict)
  : bigram_(bigram), dict_(dict) {
}


void BigramConverter::Convert(const KkciString &input, TokenString *output) {
  const size_t input_size = input.size();
  vector<vector<Node> > table(input_size + 2);

  table[0].push_back(Node({kBOS, 0, nullptr}));

  for (size_t end = 1; end <= input_size; end++) {
    for (size_t start = 0; start < end; start++) {
      const vector<Token> &token_set = dict_->Lookup(
          input.substr(start, end - start));

      if (token_set.empty()) {
        InsertNode(kUNK, table[start], &table[end]);
      } else {
        for (size_t i = 0; i < token_set.size(); i++) {
          InsertNode(token_set[i], table[start], &table[end]);
        }
      }
    }
  }

  InsertNode(kBOS, table[input_size], &table[input_size + 1]);

  for (const Node *node =
         &table[input_size + 1][0]; node != nullptr;  node = node->prev) {
    if (node->token != kBOS) {
      output->insert(0, 1, node->token);
    }
  }
}

void BigramConverter::InsertNode(const Token token,
                                 const vector<Node> &prev_columns,
                                 vector<Node> *columns) {
  if (prev_columns.empty()) {
    throw runtime_error("internal error");
  }

  Cost min_cost = 0;
  const Node *min_cost_node = nullptr;
  for (size_t i = 0; i < prev_columns.size(); i++) {
    const Node &prev_node = prev_columns[i];
    const Cost new_cost = prev_node.cost_so_far
        + bigram_->GetCost(prev_node.token, token);
    if (min_cost_node == nullptr || new_cost < min_cost) {
      min_cost = new_cost;
      min_cost_node = &prev_node;
    }
  }

  // TODO: 未知語
  if (token == kUNK) {
    min_cost += 100;
  }

  columns->push_back(Node({token, min_cost, min_cost_node}));
}


} // converter
} // lime
