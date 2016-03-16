#include <iostream>
#include "lattice.hpp"


namespace lime {
namespace converter {

Lattice::Lattice(size_t column_count,
                 unique_ptr<vector<shared_ptr<Node> > > nodes,
                 unique_ptr<vector<vector<shared_ptr<Node> > > > begin_nodes,
                 unique_ptr<vector<vector<shared_ptr<Node> > > > end_nodes)
  : column_count_(column_count),
    nodes_(move(nodes)),
    begin_nodes_(move(begin_nodes)),
    end_nodes_(move(end_nodes)) {
}


size_t Lattice::GetColumnCount() const {
  return column_count_;
}

vector<shared_ptr<Node> > &Lattice::GetBeginNodes(size_t pos) {
  return (*begin_nodes_)[pos];
}

vector<shared_ptr<Node> > &Lattice::GetEndNodes(size_t pos) {
  return (*end_nodes_)[pos];
}


} // converter
} // lime
