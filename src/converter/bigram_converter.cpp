#include <map>
#include <stdexcept>

#include "bigram_converter.hpp"


namespace lime {
namespace converter {

namespace {

shared_ptr<Node> FindMinCostNode(const vector<shared_ptr<Node> > &nodes) {
  Cost min_cost;
  shared_ptr<Node> node;
  for (vector<shared_ptr<Node> >::const_iterator it = nodes.begin();
       it != nodes.end(); ++it) {
    if (!node || (*it)->cost_so_far < min_cost) {
      min_cost = (*it)->cost_so_far;
      node = *it;
    }
  }
  return node;
}

} // namespace

BigramConverter::BigramConverter(shared_ptr<Bigram> bigram,
                                 shared_ptr<LatticeBuilder> lattice_builder)
  : bigram_(bigram), lattice_builder_(lattice_builder) {
}


void BigramConverter::Convert(const KkciString &str,
                              const Token begin_token,
                              const Token end_token,
                              Segments *result) {
  unique_ptr<Lattice> lattice =
    lattice_builder_->Build(str, begin_token, end_token);

  // MEMO: Latticeの処理は別クラスに切り出し、
  //   Convertは切り出した処理を叩くだけにするほうがスッキリするかも
  const size_t column_count = lattice->GetColumnCount();
  if (column_count < 3) {
    return;
  }
  for (size_t pos = 1; pos < column_count - 1; pos++) {
    vector<shared_ptr<Node> > &lnodes = lattice->GetEndNodes(pos);
    vector<shared_ptr<Node> > &rnodes = lattice->GetBeginNodes(pos);
    for (vector<shared_ptr<Node> >::const_iterator rnode_iter =
           rnodes.begin(); rnode_iter != rnodes.end(); ++rnode_iter) {
      SetBestLeftNode(lnodes, *rnode_iter);
    }
  }
  shared_ptr<Node> last_node =
    FindMinCostNode(lattice->GetEndNodes(column_count - 1));
  if (last_node == nullptr) {
    throw runtime_error("");
  }

  // begin_tokenとend_tokenは入れない
  for (shared_ptr<Node> node = last_node->prev; node->prev != nullptr;
       node = node->prev) {
     result->push_front(node);
  }
}

void BigramConverter::SetBestLeftNode(vector<shared_ptr<Node> > &lnodes,
                                      shared_ptr<Node> rnode) {
  Cost cost = 0;
  shared_ptr<Node> lnode;
  for (vector<shared_ptr<Node> >::iterator lnode_iter = lnodes.begin();
       lnode_iter != lnodes.end(); ++lnode_iter) {
    // MEMO:
    // - latticeにコストを持たせることもできそう
    // - コスト計算を切り出すこともできそう (3-gramへの拡張が簡単?)
    const Cost new_cost = (*lnode_iter)->cost_so_far
      + bigram_->GetCost((*lnode_iter)->token, rnode->token);
    if (!lnode || new_cost < cost) {
      cost = new_cost;
      lnode = *lnode_iter;
    }
  }
  rnode->prev = lnode;
  rnode->cost_so_far = cost;
}


} // converter
} // lime
