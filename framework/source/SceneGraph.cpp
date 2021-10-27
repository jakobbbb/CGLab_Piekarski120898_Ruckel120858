#include <SceneGraph.hpp>


SceneGraph::SceneGraph() :
    root_{std::make_shared<Node>(Node{nullptr, "root"})} {}


std::string SceneGraph::getName() const {
  return name_;
}

std::string SceneGraph::printGraph() const {
  return 0; // TODO
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

SceneGraph::~SceneGraph() {}
