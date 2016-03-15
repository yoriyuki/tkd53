#include <map>
#include <stdexcept>

#include "bigram_converter.hpp"


namespace lime {
namespace converter {

BigramConverter::BigramConverter(shared_ptr<Bigram> bigram,
                                 shared_ptr<DictionaryInterface> dict)
  : bigram_(bigram), dict_(dict) {
}


void BigramConverter::Convert(KkciString &input, Segments *output) {
  dict_->Clear();  // 念のためClearする
  const size_t input_size = input.size();
  vector<vector<Node> > table(input_size + 1);

  table[0].push_back(Node({kBOS, nullptr, 0, nullptr}));

  for (size_t end_pos = 1; end_pos <= input_size; end_pos++) {
    vector<const Entry*> entries;
    dict_->PushBack(input[end_pos-1]);
    dict_->Lookup(&entries);

    // end_posで終わる単語を辞書引きしてノードを挿入
    for (vector<const Entry*>::const_iterator entry = entries.begin();
         entry != entries.end(); ++entry) {
      if (end_pos < (*entry)->kkci_string.length()) {
        throw runtime_error("internal error");
      }
      const vector<Node> &left_nodes =
        table[end_pos - (*entry)->kkci_string.length()];
      const Node *prev;
      Cost cost_so_far;
      FindBestNode((*entry)->token, left_nodes, &prev, &cost_so_far);
      table[end_pos].push_back(
          Node({(*entry)->token, prev, cost_so_far, *entry}));
    }

    // 未知語ノードを挿入
    for (size_t i = 0; i < end_pos; i++) {
      const Node *prev;
      Cost cost_so_far;
      FindBestNode(kUNK, table[i], &prev, &cost_so_far);
      cost_so_far += 100;  // TODO: 未知語のコスト計算
      table[end_pos].push_back(Node({kUNK, prev, cost_so_far, nullptr}));
    }
  }

  const Node *prev;
  Cost not_used_cost;  // 使わない
  FindBestNode(kBOS, table[input_size], &prev, &not_used_cost);
  for (const Node *node = prev; node->prev != nullptr; node = node->prev) {
    output->push_front(*node);
  }

  // Tear down
  dict_->Clear();
}

void BigramConverter::FindBestNode(const Token token,
                                   const vector<Node> &left_nodes,
                                   const Node** node,
                                   Cost* cost) {
  *cost = 0;
  *node = nullptr;
  for (vector<Node>::const_iterator left_node = left_nodes.begin();
       left_node != left_nodes.end(); ++left_node) {
    const Cost new_cost = left_node->cost_so_far
      + bigram_->GetCost(left_node->token, token);
    if (*node == nullptr || new_cost < *cost) {
      *cost = new_cost;
      *node = &(*left_node);
    }
  }
  if (*node == nullptr) {
    throw runtime_error("internal error");
  }
}


} // converter
} // lime
