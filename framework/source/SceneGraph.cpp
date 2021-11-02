#include <SceneGraph.hpp>

/* constructor for SceneGraph */
SceneGraph::SceneGraph() :
    root_{std::make_shared<Node>(Node{nullptr, "root"})} {}

/* return the name of the scene graph */
std::string SceneGraph::getName() const {
  return name_;
}

/* print the scene graph */
std::string SceneGraph::printGraph() const {
  std::string graph = "Graph: ";
  for (auto const& node : root_->getChildrenList()) {
    graph.append(node->getName());
    graph.append(", ");
  }
  return graph;
}

/* return pointer to the root node of the scene graph */
std::shared_ptr<Node> SceneGraph::getRoot() const {
  return root_;
}

/* set the root of the scene graph */
void SceneGraph::setRoot(std::shared_ptr<Node> const& root) {
  root_ = root;
}

/* set a name for the scene graph */
void SceneGraph::setName(std::string const& name) {
  name_ = name;
}

/* print the scene graph */
std::ostream& SceneGraph::print(std::ostream& os) const {
    root_->print(os);
    return os;
}

/* store traverse function for root node */
void SceneGraph::traverse(node_traverse_func func) {
  func(root_);
  root_->traverse(func);
}

/* destructor for SceneGraph */
SceneGraph::~SceneGraph() {}

/* output operator */
std::ostream& operator<<(std::ostream& os, SceneGraph const& s) {
    return s.print(os);
}
