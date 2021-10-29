#include <SceneGraph.hpp>


SceneGraph::SceneGraph() :
    root_{std::make_shared<Node>(Node{nullptr, "root"})} {}


std::string SceneGraph::getName() const {
  return name_;
}

std::string SceneGraph::printGraph() const {
  std::string graph = "Graph: ";
  // for (auto const& node : root_->getChildren(name_)) {
  //   graph.append(node->getName());
  //   graph.append(", ");
  // }
  return graph; // TODO, not working yet
}

std::shared_ptr<Node> SceneGraph::getRoot() const {
  return root_;
}

void SceneGraph::setRoot(std::shared_ptr<Node> const& root) {
  root_ = root;
}

void SceneGraph::setName(std::string const& name) {
  name_ = name;
}

std::ostream& SceneGraph::print(std::ostream& os) const {
    root_->print(os);
    return os;
}

SceneGraph::~SceneGraph() {}

std::ostream& operator<<(std::ostream& os, SceneGraph const& s) {
    return s.print(os);
}
