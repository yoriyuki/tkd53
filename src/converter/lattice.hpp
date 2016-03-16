#pragma once

#include <vector>
#include <memory>

#include "base/kkci.hpp"
#include "base/token.hpp"
#include "dictionary/dictionary_interface.hpp"

namespace lime {
namespace converter {
using namespace std;
using namespace lime::base::kkci;
using namespace lime::base::token;
using namespace lime::dictionary;

typedef double Cost;


struct Node {
  const Token token;
  const Entry* const entry;
  shared_ptr<Node> prev;
  Cost cost_so_far;
};


class Lattice {
public:
  size_t GetColumnCount() const;

  vector<shared_ptr<Node> > &GetBeginNodes(size_t pos);

  vector<shared_ptr<Node> > &GetEndNodes(size_t pos);

  Lattice(size_t column_count,
          unique_ptr<vector<shared_ptr<Node> > > nodes,
          unique_ptr<vector<vector<shared_ptr<Node> > > > begin_nodes,
          unique_ptr<vector<vector<shared_ptr<Node> > > > end_nodes);
private:
  const size_t column_count_;
  const unique_ptr<vector<shared_ptr<Node> > > nodes_;
  unique_ptr<vector<vector<shared_ptr<Node> > > > begin_nodes_;
  unique_ptr<vector<vector<shared_ptr<Node> > > > end_nodes_;
};


} // converter
} // lime
